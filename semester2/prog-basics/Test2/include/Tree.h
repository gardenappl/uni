#ifndef TREE_H
#define TREE_H

#include <forward_list>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

namespace test
{
    template<typename T>
    struct TreeNode
    {
        T value;
        std::forward_list<TreeNode*> children;
        int children_count = 0;
        TreeNode* parent = nullptr;

        TreeNode(T value, TreeNode* parent = nullptr)
        : value(value), parent(parent)
        {}

        ~TreeNode();


        TreeNode* add(T value);
        void attach(TreeNode* node);
        void remove(TreeNode* node);

        int get_sub_height(int current_height = 0) const;
        int get_sub_count() const;
        double get_min_value() const;
        double get_max_value() const;
        double get_average_value() const;
        std::vector<TreeNode*> get_nodes_at_level(int level);

        void print(int level = 0) const;

    private:

        void get_total(std::pair<double, int>& current_total) const;
        void get_nodes_at_level(int levels_left, std::vector<TreeNode*>& current_vector);
    };

    template<typename T>
    TreeNode<T>::~TreeNode()
    {
        for(TreeNode* child : children)
            delete child;
    }

    template<typename T>
    void TreeNode<T>::print(int level) const
    {
        for(int i = 0; i < level; i++)
            std::cout << " - ";

        std::cout << value << '\n';
        for(TreeNode<T>* child : children)
            child->print(level + 1);
    }

    template<typename T>
    TreeNode<T>* TreeNode<T>::add(T value)
    {
        TreeNode<T>* node = new TreeNode<T>(value, this);
        children.push_front(node);
        children_count++;
        return node;
    }

    template<typename T>
    void TreeNode<T>::attach(TreeNode* node)
    {
        children.push_front(node);
        children_count++;
        node->parent = this;
    }

    template<typename T>
    void TreeNode<T>::remove(TreeNode* node)
    {
        children.remove(node);
        children_count--;
        node->parent = nullptr;
    }

    template<typename T>
    int TreeNode<T>::get_sub_height(int current_height) const
    {
        current_height += 1;
        for(TreeNode<T>* node : children)
            current_height = std::max(current_height, node->get_sub_height(current_height));
        return current_height;
    }

    template<typename T>
    double TreeNode<T>::get_min_value() const
    {
        double current_min = value.get_value();
        for(TreeNode<T>* node : children)
            current_min = std::min(current_min, node->get_min_value());
        return current_min;
    }

    template<typename T>
    double TreeNode<T>::get_max_value() const
    {
        double current_max = value.get_value();
        for(TreeNode<T>* node : children)
            current_max = std::max(current_max, node->get_max_value());
        return current_max;
    }

    template<typename T>
    void TreeNode<T>::get_total(std::pair<double, int>& current_total) const
    {
        current_total.first += value.get_value();
        current_total.second++;
        for(TreeNode<T>* node : children)
            node->get_total(current_total);
    }

    template<typename T>
    double TreeNode<T>::get_average_value() const
    {
        std::pair<double, int> total;
        get_total(total);
        return total.first / total.second;
    }

    template<typename T>
    void TreeNode<T>::get_nodes_at_level(int levels_left, std::vector<TreeNode<T>*>& current_vector)
    {
        if(levels_left == 1)
        {
            for(TreeNode* node : children)
                current_vector.push_back(node);
            return;
        }
        for(TreeNode* node : children)
            node->get_nodes_at_level(levels_left - 1, current_vector);
    }

    template<typename T>
    std::vector<TreeNode<T>*> TreeNode<T>::get_nodes_at_level(int level)
    {
        if(level == 0)
            return std::vector<TreeNode<T>*>({ this });
        std::vector<TreeNode<T>*> vector;
        get_nodes_at_level(level, vector);
        return vector;
    }

    template<typename T>
    int TreeNode<T>::get_sub_count() const
    {
        int total = 1;
        for(TreeNode* node : children)
            total += node->get_sub_count();
        return total;
    }
}

#endif // TREE_H
