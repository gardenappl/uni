#include <iostream>
#include "Tree.h"
#include "misc.h"
#include "BinaryTree.h"
#include "InteractiveMode.h"
#include "FileSystem.h"



//Tasks: 2 3 8 13 19 21 26
//26 includes additional functionality: FileSystem::parse_path()
//function which parses a file path string, and returns a path object which is compatible with task 2


namespace trees
{
    void DemoMode()
    {
        Tree<int> tree;

        std::vector<int> path;

        tree.set(path.begin(), path.end(), 0);
        tree.add(path.begin(), path.end(), 1);
        tree.add(path.begin(), path.end(), 2);
        path.push_back(0);
        tree.add(path.begin(), path.end(), 3);
        tree.add(path.begin(), path.end(), 4);
        path.push_back(0);
        tree.add(path.begin(), path.end(), 5);

        tree.print(std::cout);

        int find_queries[4] = {4, 3, 0, 9999};

        for(int query : find_queries)
        {
            std::cout << "path to " << query << ':' << std::endl;
            std::list<size_t>* found_path = tree.find(query);
            if(found_path == nullptr)
                std::cout << "not found" << std::endl;
            else
                print_collection(std::cout, found_path->begin(), found_path->end());
        }

        std::cout << "\nConvert to binary tree: " << std::endl;
        BinaryTree<int>* bin_tree = BinaryTree<int>::from_normal_tree(&tree);
        bin_tree->print(std::cout);

        std::cout << "Convert binary tree to linear form: " << std::endl;
        BinaryTreeLinear<int>* bin_tree_linear = bin_tree->to_linear();
        bin_tree_linear->print(std::cout);
        BinaryTreeLinear<int>::verbose_print = true;
        bin_tree_linear->print(std::cout);

        std::cout << "\nRemoving 3..." << std::endl;
        std::list<size_t>* found_path = tree.find(3);
        tree.remove(found_path->begin(), found_path->end())->print(std::cout);

        std::cout << "\nLeftover: " << std::endl;
        tree.print(std::cout);


        std::cout << "\n\n\nFile system demo!\n\n\n";
        path.clear();
        FileSystem* fs = new FileSystem();
        fs->add(path.begin(), path.end(), FileSystemElement("Dir1", 0, true));
        fs->add(path.begin(), path.end(), FileSystemElement::make_random(true));
        fs->add(path.begin(), path.end(), FileSystemElement::make_random(false));
        fs->add(path.begin(), path.end(), FileSystemElement::make_random(false));
        path.push_back(0);
        fs->add(path.begin(), path.end(), FileSystemElement("File1.txt", 0, false, 9000));
        fs->add(path.begin(), path.end(), FileSystemElement::make_random(false));

        fs->print(std::cout);

        std::cout << "Building path for Dir1/File1.txt\n";
        found_path = fs->parse_path("Dir1/File1.txt");
        std::cout << "Found path: ";
        print_collection(std::cout, found_path->begin(), found_path->end());

        std::cout << "\nReplacing...\n";
        fs->set(found_path->begin(), found_path->end(), FileSystemElement("File2.txt", 0, false, 9000));

        fs->print(std::cout);

        std::cout << "\n\nLooking for Dir1/File2.txt\n";
        FileSystemPtr file = fs->get_file("Dir1/File2.txt");
        std::cout << "Found: " << file.get() << '\n';
        std::cout << "Replacing...\n";
        fs->set(file, FileSystemElement("File3.txt", 0, false, 9000));

        fs->print(std::cout);

        path.clear();
        FileSystemPtr root = fs->get_file(path.begin(), path.end());
        std::cout << "Folder size: " << fs->get_folder_size(root) << " bytes" << std::endl;
        std::cout << "Subfolder count: " << fs->get_subfolder_count_recursive(root) << std::endl;
        std::cout << "File count: " << fs->get_file_count_recursive(root) << std::endl;
        std::cout << "Last modified: " << time_to_string(fs->get_last_modified(root)) << std::endl;


        std::cout << "Filter tree: Directories: name does not contain \"Dir\"\n";
        Tree<FileSystemPtr> result = fs->filter(true, [](const FileSystemElement& element)
                {
                    return element.name.find("Dir") == std::string::npos;
                });
        result.print(std::cout);


        std::cout << "Filter tree: Files: size > 40 kb\n";
        Tree<FileSystemPtr> result2 = fs->filter(false, [](const FileSystemElement& element)
                {
                    return element.file_size > 40000;
                });

        result2.print(std::cout);

        delete fs;
    }
}

int main()
{
//    trees::InteractiveMode::main();
    trees::DemoMode();
    return 0;
}
