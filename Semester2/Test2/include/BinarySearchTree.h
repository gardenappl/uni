#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <utility>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include "misc.h"

namespace test
{
    template<typename T>
    struct BinarySearchNode
    {
        T value;
        double numeric_value;
        BinarySearchNode* left = nullptr;
        BinarySearchNode* right = nullptr;

        BinarySearchNode(const T& value, double numeric_value)
        : value(value), numeric_value(numeric_value)
        {}

        void print(std::function<void(const T&)> print_function) const;
        void print_as_tree(std::function<void(const T&)> print_function, int current_level = 0) const;
    };

    template<typename T>
    using GetPairValueFuncType = double(*)(const std::pair<const T*, const T*>&);


    //compares ALL possible pairs

    template<typename T, GetPairValueFuncType<T> GetPairValueFunc>
    struct BinaryPairSearchTree
    {
        using pair_type = std::pair<const T*, const T*>;
        using node_type = BinarySearchNode<pair_type>;
        T* values;
        node_type* root = nullptr;
        int max_size;
        int current_size = 0;

        BinaryPairSearchTree(int max_size);

        void insert(const T& value);
        void print(bool as_tree, std::function<void(const pair_type&)> pair_print_function) const;
        std::vector<pair_type> find_all(double value, double precision) const;
    };

    template<typename T, GetPairValueFuncType<T> GetPairValueFunc>
    BinaryPairSearchTree<T, GetPairValueFunc>::BinaryPairSearchTree(int max_size)
    : max_size(max_size), values(new T[max_size])
    {}

    template<typename T, GetPairValueFuncType<T> GetPairValueFunc>
    void BinaryPairSearchTree<T, GetPairValueFunc>::insert(const T& new_value)
    {
        if(current_size == max_size)
            throw std::out_of_range("new_value");
        values[current_size] = new_value;
        for(int i = 0; i < current_size - 1; i++)
        {
            std::pair<const T*, const T*> new_pair(&values[i], &values[current_size]);
            double new_numeric_value = GetPairValueFunc(new_pair);

            node_type** current_node = &root;
            while(*current_node)
            {
                if(new_numeric_value < (*current_node)->numeric_value)
                    current_node = &(*current_node)->left;
                else
                    current_node = &(*current_node)->right;
            }
            *current_node = new node_type(new_pair, new_numeric_value);
        }
        current_size++;
    }

    template<typename T>
    void BinarySearchNode<T>::print(std::function<void(const T&)> print_function) const
    {
        if(left)
            left->print(print_function);
        print_function(value);
        if(right)
            right->print(print_function);
    }

    template<typename T>
    void BinarySearchNode<T>::
            print_as_tree(std::function<void(const T&)> print_function, int current_level) const
    {
        print_function(value);
        if(left)
        {
            for(int i = 0; i < current_level + 1; i++)
                std::cout << " - ";
            std::cout << "Left:\n";
            for(int i = 0; i < current_level + 1; i++)
                std::cout << " - ";
            left->print_as_tree(print_function, current_level + 1);
        }
        if(right)
        {
            for(int i = 0; i < current_level + 1; i++)
                std::cout << " - ";
            std::cout << "Right:\n";
            for(int i = 0; i < current_level + 1; i++)
                std::cout << " - ";
            right->print_as_tree(print_function, current_level + 1);
        }
    }

    template<typename T, GetPairValueFuncType<T> GetPairValueFunc>
    void BinaryPairSearchTree<T, GetPairValueFunc>::print(bool as_tree,
            std::function<void(const std::pair<const T*, const T*>&)> pair_print_function) const
    {
        std::cout << "Values: ";
        print_array(values, current_size);
        std::cout << '\n';
        if(!root)
        {
            std::cout << "Empty tree!\n";
            return;
        }
        if(as_tree)
            root->print_as_tree(pair_print_function);
        else
            root->print(pair_print_function);
    }

    template<typename T, GetPairValueFuncType<T> GetPairValueFunc>
    std::vector<std::pair<const T*, const T*>> BinaryPairSearchTree<T, GetPairValueFunc>::
        find_all(double search_value, double precision) const
    {
        std::stack<node_type*> dfs_stack;
        std::vector<pair_type> result;
        dfs_stack.push(root);
        while(!dfs_stack.empty())
        {
            node_type* current_node = dfs_stack.top();
            dfs_stack.pop();
            double current_value = GetPairValueFunc(current_node->value);
            if(std::abs(current_value - search_value) < precision)
                result.push_back(current_node->value);
            if(current_node->left && current_value > search_value - precision)
                dfs_stack.push(current_node->left);
            if(current_node->right && current_value < search_value + precision)
                dfs_stack.push(current_node->right);
        }
        return result;
    }
}
#endif
