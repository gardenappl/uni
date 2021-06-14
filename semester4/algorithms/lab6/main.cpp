#include <iostream>

#include "BPlusTree.hpp"
#include "HumanResources.hpp"

using namespace std;

//≤нтерактивний режим
BPlusTree<Employee>* employees;


void printHelp()
{
    std::cout << "Available commands: insert, delete, clear, print, find-by-name, help, exit" << std::endl;
}

size_t askT()
{
    while(true)
    {
        std::cout << "Enter value for t: ";
        size_t t;
        std::cin >> t;
        if(t > 0)
            return t;
    }
}

int main()
{
    employees = new BPlusTree<Employee>(askT());
    printHelp();

    //«читуЇмо команди
    while(true)
    {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if(command == "clear")
        {
            std::cout << "Deleting tree..." << std::endl;
            delete employees;
            employees = new BPlusTree<Employee>(askT());
        }
        else if(command == "insert")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;

            Employee employee(name);
            employees->insert(employee);

            std::cout << "Inserted " << name << '.' << std::endl;
        }
        else if(command == "delete")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;

            Employee employee(name);
            employees->remove(employee);

            std::cout << "Removed " << name << '.' << std::endl;
        }
        else if(command == "find-by-name")
        {
            std::cout << "Enter employee name: ";
            std::string name;
            std::cin >> name;

            Employee employee(name);
            bool result = employees->contains(employee);

            std::cout << "Tree contains " << name << "? " << std::boolalpha << result << std::endl;
        }
        else if(command == "print")
        {
            employees->printInOrder(std::cout);
            std::cout << "\n\n == Tree ==" << std::endl;
            employees->printStructure(std::cout);
        }
        else if(command == "exit")
        {
            break;
        }
        else
        {
            if(command != "help")
                std::cout << "Unknown command.";
            printHelp();
        }
    }

    delete employees;
    return 0;
}
