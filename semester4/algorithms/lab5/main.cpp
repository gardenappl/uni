#include <iostream>
#include <string>
#include <stdexcept>
#include "PersistentTree.hpp"
#include "HumanResources.hpp"

using namespace std;

void printHelp()
{
    std::cout << "Available commands: add, remove, print, print-old, search, search-old, revert, help, exit" << std::endl;
}

int main()
{
    printHelp();
    PersistentTree<Employee>* tree = new PersistentTree<Employee>();
    while(true)
    {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if(command == "add")
        {
            std::cout << "Enter employee name: ";;
            std::string name;
            std::cin >> name;
            tree->insert(Employee(name));

            tree->printInOrder();
            std::cout << std::endl;
        }
        else if(command == "remove")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);
            bool result = tree->remove(employee);
            if(!result)
            {
                std::cout << "Tree does not contain " << name << std::endl;
            }
            else
            {
                tree->printInOrder();
                std::cout << std::endl;
            }
        }
        else if(command == "print")
        {
            tree->printStructure();
        }
        else if(command == "print-old")
        {
            std::cout << "Enter version number (enter 0 to print latest version): ";
            size_t version;
            std::cin >> version;

            try
            {
                tree->printStructure(version);
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        else if(command == "search")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);

            bool result = tree->contains(employee);
            std::cout << "Tree contains " << name << "? " << std::boolalpha << result << std::endl;
        }
        else if(command == "revert")
        {
            std::cout << "Enter version: ";
            size_t version;
            std::cin >> version;

            tree->revert(version);
            std::cout << "Reverted to version " << version << ".\n";
            tree->printInOrder();
            std::cout << std::endl;
        }
        else if(command == "search-old")
        {
            std::cout << "Enter version (enter 0 to print latest version): ";
            size_t version;
            std::cin >> version;

            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);

            try
            {
                bool result = tree->contains(employee, version);
                std::cout << "Tree contains " << name << "? " << std::boolalpha << result << std::endl;
            }
            catch (const std::invalid_argument& e)
            {
                std::cout << "Error: " << e.what() << std::endl;
            }
        }
        else if(command == "exit")
        {
            break;
        }
        else
        {
            if(command != "help")
                std::cout << "Unknown command.\n";
            printHelp();
        }
    }

    delete tree;

    return 0;
}
