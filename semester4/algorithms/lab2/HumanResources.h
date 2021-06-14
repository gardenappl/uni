#ifndef HUMANRESOURCES_H
#define HUMANRESOURCES_H

#include <vector>
#include <string>
#include <iostream>

//Предметна область: відділ кадрів

class Employee
{
public:
    Employee();
    Employee(std::string name);

    const std::string& getName() const;

    bool operator==(const Employee& otherEmployee) const;
    bool operator!=(const Employee& otherEmployee) const;
    bool operator<(const Employee& otherEmployee) const;

    friend std::ostream& operator<<(std::ostream& out, const Employee& employee);

private:
    std::string name;
};

class Department
{
public:
    Department(std::string name);

    const std::vector<Employee>& getEmployees() const;
    void addEmployee(const Employee& employee);

    const std::string& getName() const;

    bool operator==(const Department& otherDepartment) const;
    bool operator!=(const Department& otherDepartment) const;

private:
    std::string name;
    std::vector<Employee> employees;
};

#endif // HUMANRESOURCES_H
