#include "HumanResources.h"

Employee::Employee()
: name("")
{}

Employee::Employee(std::string name)
: name(name)
{}

const std::string& Employee::getName() const
{
    return name;
}

bool Employee::operator==(const Employee& otherEmployee) const
{
    return name == otherEmployee.name;
}

bool Employee::operator!=(const Employee& otherEmployee) const
{
    return name != otherEmployee.name;
}

bool Employee::operator<(const Employee& otherEmployee) const
{
    return name < otherEmployee.name;
}

std::ostream& operator<<(std::ostream& out, const Employee& employee)
{
    out << "{ Name: " << employee.name << " }";
    return out;
}



Department::Department(std::string name)
: name(name)
{}

const std::string& Department::getName() const
{
    return name;
}

const std::vector<Employee>& Department::getEmployees() const
{
    return employees;
}

void Department::addEmployee(const Employee& employee)
{
    employees.push_back(employee);
}

bool Department::operator==(const Department& otherDepartment) const
{
    return this->name == otherDepartment.name &&
            this->employees == otherDepartment.employees;
}

bool Department::operator!=(const Department& otherDepartment) const
{
    return this->name != otherDepartment.name ||
            this->employees != otherDepartment.employees;
}
