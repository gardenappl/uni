#include "FileSystem.h"

#include <random>
#include <string>
#include <list>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <queue>
#include <utility>
#include "misc.h"

namespace trees
{
    FileSystemElement::FileSystemElement(const std::string& name, time_t creation_time, bool directory, size_t file_size)
    : name(name), last_modified(creation_time), is_directory(directory), file_size(file_size)
    {}

    FileSystemElement::~FileSystemElement()
    {}

    FileSystemElement FileSystemElement::make_random(bool directory)
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());
        static std::uniform_int_distribution<int> name_length_distrubution(10, 20);
        static std::uniform_int_distribution<size_t> size_distribution(0, 100000);
        static std::uniform_int_distribution<time_t> time_distribution(0, 10000000000);

        return FileSystemElement(make_random_alphanumeric_string(name_length_distrubution(rng)), time_distribution(rng),
                directory, directory ? 0 : size_distribution(rng));
    }

    std::ostream& operator<<(std::ostream& os, const FileSystemElement& element)
    {
        os << element.name;
        if(!element.is_directory)
            os << ", file size: " << element.file_size << " bytes";
        else
            os << ", directory";
        return os << ", last modified: " << time_to_string(element.last_modified);
    }


    FileSystemPtr::FileSystemPtr(Tree<FileSystemElement>::Node* node)
    : node(node)
    {}

    FileSystemElement& FileSystemPtr::get()
    {
        return node->value;
    }

    std::ostream& operator<<(std::ostream& os, const FileSystemPtr& ptr)
    {
        return os << ptr.node->value;
    }






    FileSystem::FileSystem()
    {
        size_t empty_path;
        files.set(&empty_path, &empty_path, FileSystemElement("root", 0, true));
    }

    void FileSystem::add(const FileSystemPtr& dir, const FileSystemElement& value)
    {
        dir.node->add(value);
    }

    void FileSystem::set(const FileSystemPtr& fs_ptr, const FileSystemElement& value)
    {
        if((fs_ptr.node->value.is_directory && value.is_directory) || (!fs_ptr.node->value.is_directory && !value.is_directory))
            fs_ptr.node->value = value;
    }

    void FileSystem::parse_path(const std::string& path_string, FileSystem::Node* relative_to,
            bool get_full_path, FileSystemPtr& return_ptr, std::list<size_t>*& return_path)
    {
        std::istringstream input_stream(path_string);
        if(get_full_path)
            return_path = new std::list<size_t>();

//        FileSystem::Node* prev_node = nullptr;
        FileSystem::Node* current_node = relative_to ? relative_to : files.root;

        while(!input_stream.eof())
        {
            const size_t name_length_limit = 255;
            char name_buffer[name_length_limit];

            for(size_t i = 0; i < name_length_limit; i++)
                name_buffer[i] = ' ';

            input_stream.get(name_buffer, name_length_limit, '/');
            input_stream.ignore();
            std::string name(name_buffer);

            if(current_node->children.size() == 0)
                throw std::invalid_argument("file/dir not found: " + name);

            for(size_t i = 0; i < current_node->children.size(); i++)
            {
                if(current_node->children[i]->value.name == name)
                {
                    if(get_full_path)
                        return_path->push_back(i);
//                    prev_node = current_node;
                    current_node = current_node->children[i];
                    break;
                }
                throw std::invalid_argument("file/dir not found: " + name);
            }
        }
        if(!get_full_path)
            return_ptr = FileSystemPtr(current_node);
    }

    FileSystemPtr FileSystem::get_file(const std::string& path_string, FileSystem::Node* relative_to)
    {
        FileSystemPtr return_ptr(nullptr);
        std::list<size_t>* return_path;
        parse_path(path_string, relative_to, false, return_ptr, return_path);
        return return_ptr;
    }

    std::list<size_t>* FileSystem::parse_path(const std::string& path_string, FileSystem::Node* relative_to)
    {
        FileSystemPtr return_ptr(nullptr);
        std::list<size_t>* return_path;
        parse_path(path_string, relative_to, true, return_ptr, return_path);
        return return_path;
    }

    void FileSystem::print(std::ostream& os)
    {
        files.print(os);
    }


    size_t FileSystem::get_file_count_recursive(const FileSystemPtr& folder) const
    {
        if(!folder.node->value.is_directory)
            throw std::invalid_argument("not a directory");
        size_t count = 0;
        get_file_count_recursive(folder.node, count);
        return count;
    }

    void FileSystem::get_file_count_recursive(const FileSystem::Node* node, size_t& current_count)
    {
        for(const auto* child : node->children)
        {
            if(!child->value.is_directory)
                current_count++;
            else
                FileSystem::get_file_count_recursive(child, current_count);
        }
    }


    size_t FileSystem::get_subfolder_count_recursive(const FileSystemPtr& folder) const
    {
        if(!folder.node->value.is_directory)
            throw std::invalid_argument("not a directory");
        size_t count = 0;
        get_subfolder_count_recursive(folder.node, count);
        return count;
    }

    void FileSystem::get_subfolder_count_recursive(const FileSystem::Node* node, size_t& current_count)
    {
        current_count++;
        for(const auto* child : node->children)
        {
            if(child->value.is_directory)
                FileSystem::get_file_count_recursive(child, current_count);
        }
    }


    time_t FileSystem::get_last_modified(const FileSystemPtr& folder) const
    {
        if(!folder.node->value.is_directory)
            throw std::invalid_argument("not a directory");
        time_t last_modified = 0;
        get_last_modified(folder.node, last_modified);
        return last_modified;
    }

    void FileSystem::get_last_modified(const FileSystem::Node* node, time_t& current_last_modified)
    {
        if(node->value.last_modified > current_last_modified)
            current_last_modified = node->value.last_modified;
        for(const auto* child : node->children)
        {
            if(child->value.is_directory)
                FileSystem::get_last_modified(child, current_last_modified);
            else if(child->value.last_modified > current_last_modified)
                current_last_modified = child->value.last_modified;
        }
    }


    size_t FileSystem::get_folder_size(const FileSystemPtr& folder) const
    {
        if(!folder.node->value.is_directory)
            throw std::invalid_argument("not a directory");
        size_t size = 0;
        get_folder_size(folder.node, size);
        return size;
    }

    void FileSystem::get_folder_size(const FileSystem::Node* node, size_t& current_size)
    {
        for(const auto* child : node->children)
        {
            if(!child->value.is_directory)
                current_size += child->value.file_size;
            else
                FileSystem::get_folder_size(child, current_size);
        }
    }

    Tree<FileSystemPtr> FileSystem::filter(bool directories, std::function<bool(const FileSystemElement&)> predicate) const
    {
        Tree<FileSystemPtr> tree;
        //pair: (node in the file system tree; its parent in the return tree)
        using node_pair = std::pair<FileSystem::Node*, Tree<FileSystemPtr>::Node*>;
        std::queue<node_pair> bfs_queue;

        if(!directories || predicate(files.root->value))
        {
            std::vector<size_t> empty_path;
            tree.set(empty_path.begin(), empty_path.end(), FileSystemPtr(files.root));
            bfs_queue.push(node_pair(files.root, nullptr));
        }

        while(!bfs_queue.empty())
        {
            auto [ current_node, parent_ptr ] = bfs_queue.front();
            bfs_queue.pop();
            Tree<FileSystemPtr>::Node* return_tree_node =
                    parent_ptr ? parent_ptr->add(FileSystemPtr(current_node)) : tree.root;

            for(FileSystem::Node* child : current_node->children)
            {
                if(child->value.is_directory)
                {
                    if(!directories || predicate(child->value))
                        bfs_queue.push(node_pair(child, return_tree_node));
                }
                else
                {
                    if(directories || predicate(child->value))
                        return_tree_node->add(FileSystemPtr(child));
                }
            }
        }
        return tree;
    }
}
