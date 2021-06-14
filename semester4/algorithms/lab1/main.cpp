#include <iostream>
#include "PerfectHashSet.h"
#include "HumanResources.h"

using namespace std;




int main()
{
    Employee john("John");
    Employee david("David");
    Employee yurii("Yurii");
    Employee markus("Markus");
    Employee vasilii("Vasilii");

    std::vector<Employee> myEmployees = { john, david, markus, vasilii,
            Employee("Petro"), Employee("Pavel"), Employee("Pasha") };

    PerfectHashSet<Employee> hashSet(myEmployees);
    //cout << "Created" << endl;
    hashSet.printInfo();
    cout << "Hash set contains " << yurii.getName() << "? " << std::boolalpha << hashSet.contains(yurii) << std::endl;
    cout << "Hash set contains " << markus.getName() << "? " << std::boolalpha << hashSet.contains(markus) << std::endl;
    cout << "Hash set contains " << vasilii.getName() << "? " << std::boolalpha << hashSet.contains(vasilii) << std::endl;
    int32_t vasiliiHash = getHashValue(vasilii);
    cout << "Element with value " << vasiliiHash << " = " << hashSet.getByHash(vasiliiHash) << std::endl;

    return 0;
}
