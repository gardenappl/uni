#include <iostream>
#include <string>
#include <stdexcept>
#include "RedBlackTree.h"
#include "HumanResources.h"

using namespace std;

int main()
{
    RedBlackStatTree<Employee>* tree = new RedBlackStatTree<Employee>();
    std::cout << "Available commands: add, remove, find-by-name, exit, get-rank, find-by-rank " << std::endl;
    while(true)
    {
        tree->printInOrder();
        std::cout << std::endl << "> ";
        std::string command;
        std::cin >> command;

        if(command == "add")
        {
            std::cout << "Enter employee name: ";;
            std::string name;
            std::cin >> name;
            tree->insert(Employee(name));
        }
        else if(command == "remove")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);
            bool result = tree->remove(employee);
            if(!result)
                std::cout << "Tree does not contain " << name << std::endl;
        }
        else if(command == "find-by-name")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);
            bool result = tree->contains(employee);
            std::cout << "Tree contains " << name << "? " << std::boolalpha << result << std::endl;
        }
        else if(command == "get-rank")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;
            Employee employee(name);
            try
            {
                int rank = tree->getRank(employee);
                std::cout << name << " is rank #" << rank << std::endl;
            }
            catch(std::invalid_argument& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(command == "find-by-rank")
        {
            std::cout << "Enter employee rank: ";
            int rank;
            std::cin >> rank;
            try
            {
                Employee employee = tree->findByRank(rank);
                std::cout << "Employee #" << rank << " is " << employee.getName() << std::endl;
            }
            catch(std::invalid_argument& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        else if(command == "exit")
        {
            break;
        }
    }

    delete tree;

    return 0;
}
