#include <iostream>

#include "FibonacciHeap.hpp"
#include "HumanResources.hpp"

using namespace std;

//Інтерактивний режим
FibonacciHeap<Employee>* employees;


void printHelp()
{
    std::cout << "Available commands: insert, peek, pop, search, decrease, delete, print, clear, help, exit" << std::endl;
}

int main()
{
    employees = new FibonacciHeap<Employee>();
    printHelp();

    while(true)
    {
        try
        {
            std::cout << "> ";
            std::string command;
            std::cin >> command;

            if(command == "clear")
            {
                std::cout << "Deleting tree..." << std::endl;
                delete employees;
                employees = new FibonacciHeap<Employee>();
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
            else if(command == "search")
            {
                std::cout << "Enter employee name: ";
                std::string name;
                std::cin >> name;

                Employee employee(name);
                bool contains = employees->contains(employee);

                std::cout << "Heap contains " << name << "? " << std::boolalpha << contains << std::endl;
            }
            else if(command == "decrease")
            {
                std::cout << "Enter employee name: ";
                std::string name;
                std::cin >> name;

                std::cout << "Enter new name (lower alphabetically): ";
                std::string newName;
                std::cin >> newName;

                Employee employee(name);
                Employee newEmployee(newName);
                employees->decreaseKey(employee, newEmployee);

                std::cout << "Decreased key" << std::endl;
            }
            else if(command == "delete")
            {
                std::cout << "Enter employee name: ";
                std::string name;
                std::cin >> name;

                Employee employee(name);
                employees->deleteKey(employee);

                std::cout << "Deleted key" << std::endl;
            }
            else if(command == "peek")
            {
                try
                {
                    Employee minEmployee = employees->peek();
                    std::cout << "First employee is " << minEmployee.getName() << std::endl;
                }
                catch(std::out_of_range& e)
                {
                    std::cout << "Heap is empty" << std::endl;
                }
            }
            else if(command == "pop")
            {
                try
                {
                    Employee minEmployee = employees->pop();
                    std::cout << "Popped " << minEmployee.getName() << std::endl;
                }
                catch(std::out_of_range& e)
                {
                    std::cout << "Heap is empty" << std::endl;
                }
            }
            else if(command == "print")
            {
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
        catch(const std::exception& e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    delete employees;
    return 0;
}
