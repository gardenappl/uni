#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "SortedList.h"

#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <stack>

namespace lists
{
    template<typename T, CompareFuncType<T> CompareFunc>
    struct BinarySearchTree : public SortedList<T, CompareFunc>
    {
        BinarySearchTree()
        {}
        ~BinarySearchTree() override;

        void add(const T& element) override;
        void print_all(std::ostream& os) const override;
        bool remove(const T& element) override;
        bool contains(const T& element) const override;
        std::vector<T> find_all(const T& min, const T& max) const override;
        void for_each(std::function<void(const T&)> func) const override;

        void print_as_tree(std::ostream& os) const;

    private:
        struct Node
        {
            Node* left = nullptr;
            Node* right = nullptr;
            T value;

            Node(const T& value)
            : value(value)
            {}

            void print(std::ostream& os, bool& first_element) const;
            void print_as_tree(std::ostream& os, int levels) const;
            void for_each(std::function<void(const T&)> func) const;
        };

        Node* root = nullptr;
        void remove(Node** node);
    };


    template<typename T, CompareFuncType<T> CompareFunc>
    BinarySearchTree<T, CompareFunc>::~BinarySearchTree()
    {
        std::queue<Node*> queue;
        if(root)
            queue.push(root);
        while(!queue.empty())
        {
            Node* current_node = queue.front();
            queue.pop();
            if(current_node->left)
                queue.push(current_node->left);
            if(current_node->right)
                queue.push(current_node->right);
            delete current_node;
        }
    }


    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::add(const T& element)
    {
        if(!root)
        {
            root = new Node(element);
            return;
        }
        Node** current_node = &root;
        while(true)
        {
            if(*current_node == nullptr)
            {
                *current_node = new Node(element);
                return;
            }
            if(CompareFunc(element, (*current_node)->value) >= 0)
                current_node = &(*current_node)->right;
            else
                current_node = &(*current_node)->left;
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::Node::print(std::ostream& os, bool& first_element) const
    {
        if(left)
            left->print(os, first_element);

        if(first_element)
            first_element = false;
        else
            os << ", ";
        os << value;

        if(right)
            right->print(os, first_element);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::print_all(std::ostream& os) const
    {
        os << "{ ";
        if(root)
        {
            bool first_element = true;
            root->print(os, first_element);
        }
        os << " }" << std::endl;
//        print_as_tree(os);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::Node::print_as_tree(std::ostream& os, int levels) const
    {
        for(int i = 0; i < levels; i++)
            os << " - ";
        os << value << std::endl;
        if(left)
        {
            for(int i = 0; i < levels; i++)
                os << " - ";
            os << "Left:\n";
            left->print_as_tree(os, levels + 1);
        }
        if(right)
        {
            for(int i = 0; i < levels; i++)
                os << " - ";
            os << "Right:\n";
            right->print_as_tree(os, levels + 1);
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::print_as_tree(std::ostream& os) const
    {
        std::queue<Node*> queue;
        if(root)
            root->print_as_tree(os, 0);
        else
            os << "Empty tree!\n";
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool BinarySearchTree<T, CompareFunc>::remove(const T& element)
    {
//        std::cout << "Removing " << element << "...\n";
        if(!root)
        {
            return false;
        }
        Node** current_node = &root;
        while(current_node)
        {
            int compare_result = CompareFunc(element, (*current_node)->value);
            if(compare_result < 0 && (*current_node)->left)
                current_node = &(*current_node)->left;
            else if(compare_result > 0 && (*current_node)->right)
                current_node = &(*current_node)->right;
            else if(compare_result == 0)
            {
                remove(current_node);
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::remove(Node** node)
    {
        if((*node)->left)
        {
            if(!(*node)->right)
            {
                Node* temp_node = *node;
                *node = (*node)->left;
                delete temp_node;
            }
            else
            {
                Node** min_node = &(*node)->right;
                while((*min_node)->left)
                {
                    min_node = &(*min_node)->left;
                }
                (*node)->value = (*min_node)->value;
//                std::cout << "Min: " << (*min_node)->value << std::endl;
                remove(min_node);
            }
        }
        else
        {
            if((*node)->right)
            {
                Node* temp_node = *node;
                *node = (*node)->right;
                delete temp_node;
            }
            else
            {
                Node* temp_node = *node;
                *node = nullptr;
                delete temp_node;
            }
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::Node::for_each(std::function<void(const T&)> func) const
    {
        if(left)
            left->for_each(func);
        func(value);
        if(right)
            right->for_each(func);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void BinarySearchTree<T, CompareFunc>::for_each(std::function<void(const T&)> func) const
    {
        if(root)
            root->for_each(func);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool BinarySearchTree<T, CompareFunc>::contains(const T& element) const
    {
//        int searches = 0;
        std::vector<T> result;
        std::queue<Node*> bfs_queue;
        if(root)
            bfs_queue.push(root);
        while(!bfs_queue.empty())
        {
//            searches++;
            Node* current_node = bfs_queue.front();
            bfs_queue.pop();
            int compare_result = CompareFunc(current_node->value, element);
            if(compare_result == 0)
            {
//                std::cout << "(searches: " << searches << ")\n";
                return true;
            }
            if(compare_result > 0 && current_node->left)
                bfs_queue.push(current_node->left);
            else if(compare_result < 0 && current_node->right)
                bfs_queue.push(current_node->right);
        }
        return false;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> BinarySearchTree<T, CompareFunc>::find_all(const T& min, const T& max) const
    {
        std::vector<T> result;
        std::queue<Node*> bfs_queue;
        if(root)
            bfs_queue.push(root);
        while(!bfs_queue.empty())
        {
            Node* current_node = bfs_queue.front();
            bfs_queue.pop();
            if(CompareFunc(current_node->value, min) >= 0 && current_node->left)
                bfs_queue.push(current_node->left);
            if(CompareFunc(current_node->value, max) <= 0)
            {
                if(current_node->right)
                    bfs_queue.push(current_node->right);
                if(CompareFunc(current_node->value, min) >= 0)
                    result.push_back(current_node->value);
            }
        }
        return result;
    }
}

#endif //BINARYSEARCHTREE_H
