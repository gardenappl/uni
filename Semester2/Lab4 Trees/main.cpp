#include <iostream>
#include "Tree.h"
#include "misc.h"
#include "BinaryTree.h"
#include "InteractiveMode.h"


namespace trees
{
    void DemoMode()
    {
        Tree<int> tree;

        std::vector<int> path;

        tree.set(path.begin(), path.end(), 0);
        tree.add(path.begin(), path.end(), 1);
        tree.add(path.begin(), path.end(), 2);
        path.push_back(0);
        tree.add(path.begin(), path.end(), 3);
        tree.add(path.begin(), path.end(), 4);
        path.push_back(0);
        tree.add(path.begin(), path.end(), 5);

        tree.print(std::cout);

        int find_queries[4] = {4, 3, 0, 9999};

        for(int query : find_queries)
        {
            std::cout << "path to " << query << ':' << std::endl;
            std::list<size_t>* found_path = tree.find(query);
            if(found_path == nullptr)
                std::cout << "not found" << std::endl;
            else
                print_collection(std::cout, found_path->begin(), found_path->end());
        }

        std::cout << "\nConvert to binary tree: " << std::endl;
        BinaryTree<int>* bin_tree = BinaryTree<int>::from_normal_tree(&tree);
        bin_tree->print(std::cout);

        std::cout << "Convert binary tree to linear form: " << std::endl;
        BinaryTreeLinear<int>* bin_tree_linear = bin_tree->to_linear();
        bin_tree_linear->print(std::cout);
        BinaryTreeLinear<int>::verbose_print = true;
        bin_tree_linear->print(std::cout);

        std::cout << "\nRemoving 3..." << std::endl;
        std::list<size_t>* found_path = tree.find(3);
        tree.remove(found_path->begin(), found_path->end())->print(std::cout);

        std::cout << "\nLeftover: " << std::endl;
        tree.print(std::cout);
    }
}

//tasks: 2 3 8 13 19 21 26
int main()
{
    trees::InteractiveMode::main();
//    trees::DemoMode();
    return 0;
}
