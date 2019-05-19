#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <utility>
#include <functional>
#include <iostream>
#include "misc.h"

namespace test
{
    template<typename T>
    using CompareFuncType = bool(*)(const T&, const T&);

    template<typename T, CompareFuncType<T> CompareFunc>
    struct BinarySearchNode
    {
        T value;
        BinarySearchNode* left = nullptr;
        BinarySearchNode* right = nullptr;

        BinarySearchNode(const T& value)
        : value(value)
        {}

        void print(std::function<void(const T&)> print_function) const;
    };

    template<typename T>
    using PairCompareFuncType = bool(*)(const std::pair<const T&, const T&>&, const std::pair<const T&, const T&>&);

    //compares ALL possible pairs

    template<typename T, PairCompareFuncType<T> PairCompareFunc>
    struct BinaryPairSearchTree
    {
        using node_type = BinarySearchNode<std::pair<const T&, const T&>, PairCompareFunc>;
        std::vector<T> values;
        node_type* root = nullptr;

        void insert(const T& value);
        void print(std::function<void(const std::pair<const T&, const T&>&)> pair_print_function) const;
    };

    template<typename T, PairCompareFuncType<T> PairCompareFunc>
    void BinaryPairSearchTree<T, PairCompareFunc>::insert(const T& new_value)
    {
        values.push_back(new_value);
        for(int i = 0; i < values.size() - 1; i++)
        {
            T value = values[i];
            std::pair<const T&, const T&> new_pair(value, values.back());

            node_type** current_node = &root;
            while(*current_node)
            {
                std::cout << "current_node: " << (*current_node) << std::endl;
                if(PairCompareFunc(new_pair, (*current_node)->value))
                    current_node = &(*current_node)->left;
                else
                    current_node = &(*current_node)->right;
            }
            *current_node = new node_type(new_pair);
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchNode<T, CompareFunc>::print(std::function<void(const T&)> print_function) const
    {
        if(left)
            left->print(print_function);
        print_function(value);
        if(right)
            right->print(print_function);
    }

    template<typename T, PairCompareFuncType<T> PairCompareFunc>
    void BinaryPairSearchTree<T, PairCompareFunc>::print(
            std::function<void(const std::pair<const T&, const T&>&)> pair_print_function) const
    {
        std::cout << "Values: ";
        print_array(values.data(), values.size());
        std::cout << '\n';
        if(!root)
        {
            std::cout << "Empty tree!\n";
            return;
        }
        root->print(pair_print_function);
    }
}
#endif
