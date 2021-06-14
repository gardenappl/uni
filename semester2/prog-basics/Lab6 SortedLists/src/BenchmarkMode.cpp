#include "BenchmarkMode.h"

#include "SortedList.h"
#include "SortedArrayList.h"
#include "SortedLinkedList.h"
#include "AVLTree.h"
#include "BinarySearchTree.h"
#include "misc.h"

#include <chrono>
#include <random>


namespace lists
{
    void BenchmarkMode::run(int element_count, bool only_trees, bool insert_sorted)
    {
        static std::random_device rd;
        static std::mt19937 rng(rd());


        static const int LISTS_COUNT = 4;
        static std::string list_names[LISTS_COUNT] = { "Linked List", "Array List", "Binary Search Tree", "AVL Tree" };
        SortedList<std::string, str_compare>* lists[LISTS_COUNT] =
                {
                    new SortedLinkedList<std::string, str_compare>(),
                    new SortedArrayList<std::string, str_compare>(),
                    new BinarySearchTree<std::string, str_compare>(),
                    new AVLTree<std::string, str_compare>()
                };


        std::vector<std::string> generated_values;
        generated_values.reserve(element_count);
        std::vector<size_t> remove_order;
        remove_order.reserve(element_count);
        std::vector<std::string> search_values;
        search_values.reserve(element_count);

        std::cout << "Generating values..." << std::endl;
        for(int i = 0; i < element_count; i++)
        {
            if(insert_sorted)
                generated_values.push_back("aaa" + std::to_string(i));
            else
            {
                static std::uniform_int_distribution<std::string::size_type> str_length_dist(5, 20);
                generated_values.push_back(make_random_alphanumeric_string(str_length_dist(rng)));
            }

        }
        for(int i = 0; i < element_count; i++)
        {
            static std::uniform_int_distribution<size_t> index_dist(0, element_count - 1);
            remove_order.push_back(index_dist(rng));
        }
        for(int i = 0; i < element_count; i++)
        {
            static std::uniform_int_distribution<int> dist(0, 10);
            if(dist(rng) < 3)
                search_values.push_back(make_random_alphanumeric_string(6 + dist(rng)));
            else
                search_values.push_back(generated_values[remove_order[i]]);
        }

        static const int FUNCTIONS_COUNT = 3;
        static std::string function_names[FUNCTIONS_COUNT] = { "Adding", "Search", "Removing" };
        std::function<void(SortedList<std::string, str_compare>*)> functions[FUNCTIONS_COUNT] =
                {
                    //adding
                    [&](SortedList<std::string, str_compare>* list) {
                        for(int i = 0; i < element_count; i++)
                        {
                            list->add(generated_values[i]);
                        }
                        std::cout << "Added " << element_count << " elements.";
                    },
                    //search
                    [&](SortedList<std::string, str_compare>* list) {
                        int found = 0;
                        for(int i = 0; i < element_count; i++)
                        {
                            if(list->contains(search_values[i]))
                                found++;
                        }
                        std::cout << "Retrieved random elements " << element_count << " times (" << found << " hits).";
                    },
                    //removing
                    [&](SortedList<std::string, str_compare>* list) {
                        for(int i = 0; i < element_count; i++)
                        {
                            #if DEBUG
                            std::cout << "removing " << generated_values[remove_order[i]] << '\n'
                                    << "before: " << std::endl;
                            list->print_all(std::cout);
                            #endif // DEBUG

                            list->remove(generated_values[remove_order[i]]);
                            #if DEBUG
                            std::cout << "after: " << std::endl;
                            list->print_all(std::cout);
                            #endif // DEBUG
                        }
                        std::cout << "Removed random elements " << element_count << " times.";
                    }
                };

        for(int i = only_trees ? 2 : 0; i < LISTS_COUNT; i++)
        {
            std::cout << list_names[i] << ":\n";
            for(int i2 = 0; i2 < FUNCTIONS_COUNT; i2++)
            {
                std::cout << function_names[i2] << "... ";
                std::cout.flush();

                auto timer_start = std::chrono::system_clock::now();
                functions[i2](lists[i]);
//                lists[i]->print_all(std::cout);
                auto timer_end = std::chrono::system_clock::now();
                std::cout << ' '
                        << std::chrono::duration_cast<std::chrono::milliseconds>(timer_end - timer_start).count()
                        << " ms" << std::endl;
            }
            delete lists[i];
        }
    }
}
