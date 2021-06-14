#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Tree.h"
#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <list>
#include <functional>


//Unlike Tree and BinaryTree, the FileSystem is encapsulated

namespace trees
{
    struct FileSystemElement
    {
        std::string name;
        time_t last_modified;
        size_t file_size = 0;
        bool is_directory = false;

        static FileSystemElement make_random(bool directory);
        FileSystemElement(const std::string& name, time_t creation_time, bool directory, size_t file_size = 0);
        ~FileSystemElement();
    };

    struct FileSystem;

    struct FileSystemPtr
    {
        friend FileSystem;

        FileSystemElement& get();

        friend std::ostream& operator<<(std::ostream& os, const FileSystemPtr& ptr);

    private:
        FileSystemPtr(Tree<FileSystemElement>::Node* node = nullptr);
        Tree<FileSystemElement>::Node* node;
    };


    struct FileSystem
    {
        typedef Tree<FileSystemElement>::Node Node;

        FileSystem();

        std::list<size_t>* parse_path(const std::string& path_string, Node* node = nullptr);
        FileSystemPtr get_file(const std::string& path_string, Node* relative_to = nullptr);

        template<typename Iterator>
        FileSystemPtr get_file(const Iterator& begin, const Iterator& end) const;
        template<typename Iterator>
        void add(const Iterator& begin, const Iterator& end, const FileSystemElement& value);
        void add(const FileSystemPtr& dir, const FileSystemElement& value);
        template<typename Iterator>
        void set(const Iterator& begin, const Iterator& end, const FileSystemElement& value);
        void set(const FileSystemPtr& fs_ptr, const FileSystemElement& value);

        size_t get_folder_size(const FileSystemPtr& folder) const;
        size_t get_subfolder_count_recursive(const FileSystemPtr& folder) const;
        size_t get_file_count_recursive(const FileSystemPtr& folder) const;
        time_t get_last_modified(const FileSystemPtr& folder) const;

        Tree<FileSystemPtr> filter(bool directories, std::function<bool(const FileSystemElement&)> predicate) const;

        void print(std::ostream& os);

    private:
        Tree<FileSystemElement> files;

        void parse_path(const std::string& path_string, FileSystem::Node* relative_to,
                bool get_full_path, FileSystemPtr& return_ptr, std::list<size_t>*& return_path);

        static void get_folder_size(const Node* current_node, size_t& current_size);
        static void get_subfolder_count_recursive(const Node* current_node, size_t& current_count);
        static void get_file_count_recursive(const Node* current_node, size_t& current_count);
        static void get_last_modified(const Node* current_node, time_t& current_last_modified);
    };

    std::ostream& operator<<(std::ostream& os, const FileSystemElement& element);


    template<typename Iterator>
    FileSystemPtr FileSystem::get_file(const Iterator& begin, const Iterator& end) const
    {
        return FileSystemPtr(files.get_node(begin, end));
    }

    template<typename Iterator>
    void FileSystem::add(const Iterator& begin, const Iterator& end, const FileSystemElement& value)
    {
        add(FileSystemPtr(files.get_node(begin, end)), value);
    }

    template<typename Iterator>
    void FileSystem::set(const Iterator& begin, const Iterator& end, const FileSystemElement& value)
    {
        if(!files.root)
        {
            if(begin != end)
                throw std::invalid_argument("invalid path");
            files.root = new Node(value);
            return;
        }
        set(FileSystemPtr(files.get_node(begin, end)), value);
    }
}

#endif // FILESYSTEM_H
