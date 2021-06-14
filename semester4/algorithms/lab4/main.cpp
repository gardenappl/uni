#include <iostream>

#include <vector>
#include <utility>
#include <algorithm>
#include "OptimalBST.hpp"
#include "HumanResources.hpp"

using namespace std;


//≤нтерактивний режим
OptimalBST<Employee>* employees;
std::vector<Employee> employeesVec;
std::vector<float> freqs;
std::vector<float> auxFreqs;


void printHelp()
{
    std::cout << "Available commands: create, print, find-by-name, help, exit" << std::endl;
}

OptimalBST<Employee>* createTree()
{
    employeesVec.clear();
    freqs.clear();
    auxFreqs.clear();

    float cost;

    while(true)
    {
        std::cout << "Enter employee name (or \"stop\"): ";;
        std::string name;
        std::cin >> name;
        if(name == "stop")
            break;
        employeesVec.emplace_back(name);
    }
    if(employeesVec.empty())
    {
        return new OptimalBST<Employee>(employeesVec, freqs, auxFreqs);
    }

    //≈лементи мають бути в≥дсортован≥
    std::sort(employeesVec.begin(), employeesVec.end());


    std::cout << "Enter cost of employees before " << employeesVec[0].getName() << ": ";
    std::cin >> cost;
    auxFreqs.push_back(cost);

    for(size_t i = 0; i < employeesVec.size() - 1; i++)
    {
        std::cout << "Enter cost for " << employeesVec[i].getName() << ": ";
        std::cin >> cost;
        freqs.push_back(cost);

        std::cout << "Enter cost for employees between " << employeesVec[i].getName()
                 << " and " << employeesVec[i + 1].getName() << ": ";
        std::cin >> cost;
        auxFreqs.push_back(cost);
    }
    std::cout << "Enter cost for " << employeesVec.back().getName() << ": ";
    std::cin >> cost;
    freqs.push_back(cost);

    std::cout << "Enter cost of employees after " << employeesVec.back().getName() << ": ";
    std::cin >> cost;
    auxFreqs.push_back(cost);

    std::cout << "Creating tree..." << std::endl;

    return new OptimalBST<Employee>(employeesVec, freqs, auxFreqs);
}

void print()
{
    for(size_t i = 0; i < employeesVec.size(); i++)
    {
        std::cout << "(aux. values)\t\t - cost: " << auxFreqs[i] << std::endl;
        std::cout << employeesVec[i].getName() << "\t\t - cost: " << freqs[i] << std::endl;
    }
    std::cout << "(aux. values)" << "\t\t - cost: " << auxFreqs.back() << std::endl;
    std::cout << "\n== Tree ==" << std::endl;
    employees->printStructure();
}

int main()
{
    employees = createTree();
    print();
    printHelp();

    //«читуЇмо команди
    while(true)
    {
        std::cout << "> ";
        std::string command;
        std::cin >> command;

        if(command == "create")
        {
            std::cout << "Deleting tree..." << std::endl;
            delete employees;
            createTree();
            print();
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
            print();
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
