#include "InteractiveMode.h"

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include "misc.h"
#include "Tree.h"
#include "BinaryTree.h"

namespace trees
{
    std::list<size_t>* InteractiveMode::clipboard_path = nullptr;
    Tree<int>* InteractiveMode::current_tree = new Tree<int>();

    void InteractiveMode::reset()
    {
        clipboard_path = nullptr;
        current_tree = new Tree<int>();
    }

    void InteractiveMode::main()
    {
        reset();

        std::cout << "Welcome to the Trees Interactive Experience.\n"
                << "Available commands: set, append, remove, find, print, print-bin, print-bin-linear, reset, quit\n";

        std::string input;
        while(true)
        {
            std::cout << "Enter command: ";
            std::cin >> input;

            if(input == "append")
            {
                std::cout << "Enter value: ";
                int num_input;
                std::cin >> num_input;
                std::list<size_t>* path = get_path();
                current_tree->add(path->begin(), path->end(), num_input);
                std::cout << "Element appended.\n";
                delete path;
            }
            else if(input == "set")
            {
                std::cout << "Enter value: ";
                int num_input;
                std::cin >> num_input;
                std::list<size_t>* path = get_path();
                current_tree->set(path->begin(), path->end(), num_input);
                std::cout << "Element set.\n";
                delete path;
            }
            else if(input == "remove")
            {
                std::list<size_t>* path = get_path();
                Tree<int>* removed_tree = current_tree->remove(path->begin(), path->end());
                std::cout << "Node removed.\n";
                removed_tree->print(std::cout);
                delete removed_tree;
                delete path;
            }
            else if(input == "find")
            {
                std::cout << "Enter value: ";
                int num_input;
                std::cin >> num_input;
                clipboard_path = current_tree->find(num_input);
                if(clipboard_path)
                {
                    std::cout << "Element found, path saved to \"clipboard\": ";
                    print_collection(std::cout, clipboard_path->begin(), clipboard_path->end());
                }
                else
                {
                    std::cout << "Element not found.\n";
                }
            }
            else if(input == "print")
            {
                current_tree->print(std::cout);
            }
            else if(input == "print-bin")
            {
                BinaryTree<int>* bin_tree = BinaryTree<int>::from_normal_tree(current_tree);
                bin_tree->print(std::cout);
                delete bin_tree;
            }
            else if(input == "print-bin-linear")
            {
                BinaryTree<int>* bin_tree = BinaryTree<int>::from_normal_tree(current_tree);
                bin_tree->print(std::cout);
                BinaryTreeLinear<int>* bin_tree_linear = bin_tree->to_linear();
                BinaryTreeLinear<int>::verbose_print = true;
                bin_tree_linear->print(std::cout);
                BinaryTreeLinear<int>::verbose_print = false;
                bin_tree_linear->print(std::cout);
                delete bin_tree;
                delete bin_tree_linear;
            }
            else if(input == "reset")
            {
                std::cout << "Reset current tree and \"clipboard\"\n";
                reset();
            }
            else if(input == "quit")
            {
                std::exit(0);
            }
        }
    }

    std::list<size_t>* InteractiveMode::get_path()
    {
        if(clipboard_path != nullptr)
        {
            std::cout << "Using path from \"clipboard\": \n";
            print_collection(std::cout, clipboard_path->begin(), clipboard_path->end());
            std::list<size_t>* list = clipboard_path;
            clipboard_path = nullptr;
            return list;
        }

        std::cout << "Enter path: \n"
                << "(array of child indexes starting with 0, enter -1 to stop)\n"
                << "(empty array means \"root\")" << std::endl;
        std::list<size_t>* new_list = new std::list<size_t>();

        size_t input = 0;
        while(true)
        {
            std::cin >> input;
            if(input == -1)
                break;
            new_list->push_back(input);
        }
        std::cout << "Path: ";
        print_collection(std::cout, new_list->begin(), new_list->end());
        return new_list;
    }
}
