#include <iostream>
#include <string>

#include "SortedLinkedList.h"
#include "SortedArrayList.h"
#include "BinarySearchTree.h"
#include "misc.h"

namespace lists
{
    void demo_mode()
    {
        SortedList<std::string, str_compare>* lists[] =
                {
                    new SortedLinkedList<std::string, str_compare>(),
                    new SortedArrayList<std::string, str_compare>(),
                    new BinarySearchTree<std::string, str_compare>()
                };
        for(auto* list : lists)
        {
            list->add("Hello!");
            list->add("Hell");
            list->add("Hell3");
            list->add("AAAA");
            list->add("Hell");
            list->add("Hello World!");
            list->add("Hellooooooo!");
            list->add("Hell2");
            list->add("Hell4");
            list->add("Hell");
            list->print_all(std::cout);
            list->remove("AAAA");
            list->remove("Hell2");
            list->remove("Hello!");
            list->remove("Hellooooooo!");
            list->print_all(std::cout);
            list->for_each([](const std::string& str)
                    {
                        std::cout << "Haha, yes, \"" << str << "\"\n";
                    });
            std::cout << "Found " << list->find_all("Hell").size() << " elements equal to \"Hell\"\n";
            std::cout << "Found " << list->find_all("Hell", "Hell3").size()
                    << " elements between \"Hell\" and \"Hell3\"\n";
        }
    }
}

int main()
{
//    std::cout << lists::str_compare("Hellooooooo!", "Hello World!");
//    std::cout << lists::str_compare("Hello World!", "Hellooooooo!");
    lists::demo_mode();
    return 0;
}
