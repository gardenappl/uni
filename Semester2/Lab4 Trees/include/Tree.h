#ifndef TREE_H
#define TREE_H

#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <list>

namespace trees
{
    template<typename T>
    struct Tree
    {
        struct Node
        {
            friend Tree;
            T value;
            std::vector<Node*> children;

            Node(T value)
            : value(value)
            {}

            ~Node();

            Node* add(const T& value);

        private:
            std::list<size_t>* find(const T& search_value, size_t current_index) const;

            void print(std::ostream& os, int current_level) const;
        };
        Node* root = nullptr;

        Tree(Node* root)
        : root(root)
        {}
        Tree() = default;
        ~Tree();

        template<typename Iterator>
        T& get(const Iterator& begin, const Iterator& end) const;
        template<typename Iterator>
        void add(const Iterator& begin, const Iterator& end, const T& value);
        template<typename Iterator>
        void set(const Iterator& begin, const Iterator& end, const T& value);
        template<typename Iterator>
        Tree* remove(const Iterator& begin, const Iterator& end);
        //result of find() is usable as path for insert(), set(), etc.
        //unless the element wasn't found, in which case find() returns nullptr
        std::list<size_t>* find(const T& value) const;

        void print(std::ostream& os) const;


    private:
        template<typename Iterator>
        Node* get_node(Iterator begin, const Iterator& end) const;
    };

    template<typename T>
    Tree<T>::~Tree()
    {
        if(root)
            delete root;
    }

    template<typename T>
    Tree<T>::Node::~Node()
    {
        for(Node* child : children)
            delete child;
    }

    template<typename T>
    typename Tree<T>::Node* Tree<T>::Node::add(const T& value)
    {
        Node* new_node = new Node(value);
        children.push_back(new_node);
        return new_node;
    }

    template<typename T>
    template<typename Iterator>
    T& Tree<T>::get(const Iterator& begin, const Iterator& end) const
    {
        return get_node(begin, end)->value;
    }

    template<typename T>
    template<typename Iterator>
    void Tree<T>::add(const Iterator& begin, const Iterator& end, const T& value)
    {
        get_node(begin, end)->add(value);
    }

    template<typename T>
    template<typename Iterator>
    void Tree<T>::set(const Iterator& begin, const Iterator& end, const T& value)
    {
        if(!root)
        {
            if(begin != end)
                throw std::invalid_argument("invalid path");
            root = new Node(value);
            return;
        }
        get_node(begin, end)->value = value;
    }

    template<typename T>
    template<typename Iterator>
    typename Tree<T>::Node* Tree<T>::get_node(Iterator begin, const Iterator& end) const
    {
        Node* current_value = root;
        while(begin != end)
        {
            size_t index = *begin;
            if(index >= current_value->children.size() || index < 0)
                throw std::invalid_argument("invalid path");
            current_value = current_value->children[*begin];
            begin++;
        }
        return current_value;
    }

    template<typename T>
    void Tree<T>::Node::print(std::ostream& os, int current_level) const
    {
        for(int i = 0; i < current_level; i++)
            os << "- ";
        os << value << '\n';
        for(Node* node : children)
            node->print(os, current_level + 1);
    }

    template<typename T>
    void Tree<T>::print(std::ostream& os) const
    {
        if(!root)
            os << "[Empty]\n";
        else
            root->print(os, 0);
    }

    template<typename T>
    template<typename Iterator>
    Tree<T>* Tree<T>::remove(const Iterator& begin, const Iterator& end)
    {
        if(begin == end)
        {
            Tree* new_tree = new Tree(root);
            root = nullptr;
            return new_tree;
        }
        Iterator it = end;
        Node* parent = get_node(begin, --it);
        size_t child_index = *it;
        if(child_index < 0 || child_index >= parent->children.size())
            throw std::invalid_argument("invalid path");
        Node* node = parent->children[child_index];
        parent->children.erase(parent->children.begin() + child_index);
        return new Tree(node);
    }

    template<typename T>
    std::list<size_t>* Tree<T>::find(const T& search_value) const
    {
        if(!root)
            return nullptr;
        if(root->value == search_value)
            return new std::list<size_t>();
        return root->find(search_value, -1);
    }

    template<typename T>
    std::list<size_t>* Tree<T>::Node::find(const T& search_value, size_t current_index) const
    {
        if(value == search_value)
        {
            std::list<size_t>* new_list = new std::list<size_t>();
            new_list->push_front(current_index);
            return new_list;
        }
        for(size_t i = 0; i < children.size(); i++)
        {
            std::list<size_t>* result = children[i]->find(search_value, i);
            if(result)
            {
                if(current_index != -1)
                    result->push_front(current_index);
                return result;
            }
        }
        return nullptr;
    }
}

#endif // TREE_H
