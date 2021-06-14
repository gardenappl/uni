#include <iostream>
#include <string>

#include "SortedLinkedList.h"
#include "SortedArrayList.h"
#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "misc.h"
#include "BenchmarkMode.h"

namespace lists
{
    void demo_mode()
    {
        std::string names[] = { "Linked List", "Array List", "Binary Search Tree", "AVL Tree" };
        SortedList<std::string, str_compare>* lists[] =
                {
                    new SortedLinkedList<std::string, str_compare>(),
                    new SortedArrayList<std::string, str_compare>(),
                    new BinarySearchTree<std::string, str_compare>(),
                    new AVLTree<std::string, str_compare>()
                };
        for(int i = 0; i < 4; i++)
        {
            SortedList<std::string, str_compare>* list = lists[i];
            std::cout << "\n\n ====== " << names[i] << " ======\n";
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
            if(list->contains("Hell"))
                std::cout << "List contains \"Hell\"\n";
            else
                std::cout << "List does not contain \"Hell\"\n";
            if(list->contains("Hell2"))
                std::cout << "List contains \"Hell2\"\n";
            else
                std::cout << "List does not contain \"Hell2\"\n";
            std::cout << "Found " << list->find_all("Hell", "Hell3").size()
                    << " elements between \"Hell\" and \"Hell3\"\n";

            delete list;
        }
    }
}

int main()
{
//    std::cout << lists::str_compare("Hellooooooo!", "Hello World!");
//    std::cout << lists::str_compare("Hello World!", "Hellooooooo!");

    std::cout << "8000 elements, inserted randomly:\n";
    lists::BenchmarkMode::run(8000, false, false);
    std::cout << "\n\n\n8000 elements, inserted in order:\n";
    lists::BenchmarkMode::run(8000, true, true);
    std::cout << "\n\n\n2000000 elements, inserted randomly:\n";
    lists::BenchmarkMode::run(2000000, true, false);
//    lists::demo_mode();
    return 0;
}
