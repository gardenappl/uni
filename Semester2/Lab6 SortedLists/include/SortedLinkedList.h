#ifndef SORTEDLINKEDLIST_H
#define SORTEDLINKEDLIST_H

#include "SortedList.h"

#include <iostream>
#include <functional>
#include <vector>

namespace lists
{
    template<typename T, CompareFuncType<T> CompareFunc>
    struct SortedLinkedList : public SortedList<T, CompareFunc>
    {
        SortedLinkedList()
        {}
        ~SortedLinkedList() override;

        void add(const T& element) override;
        void print_all(std::ostream& os) const override;
        bool remove(const T& element) override;
        std::vector<T> find_all(const T& element) const override;
        std::vector<T> find_all(const T& min, const T& max) const override;
        void for_each(std::function<void(const T&)> func) const override;

    private:
        struct Node
        {
            Node* next;
            T value;

            Node(const T& value, Node* next = nullptr)
            : next(next), value(value)
            {}
        };

        Node* root = nullptr;
    };


    template<typename T, CompareFuncType<T> CompareFunc>
    SortedLinkedList<T, CompareFunc>::~SortedLinkedList()
    {
        Node* current_node = root;
        while(current_node)
        {
            Node* temp_node = current_node->next;
            delete current_node;
            current_node = temp_node;
        }
    }


    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedLinkedList<T, CompareFunc>::add(const T& element)
    {
        if(!root || CompareFunc(element, root->value) < 0)
        {
            root = new Node(element, root);
            return;
        }
        Node* current_node = root;
        while(current_node)
        {
            if(!current_node->next || CompareFunc(element, current_node->next->value) < 0)
            {
                current_node->next = new Node(element, current_node->next);
                return;
            }
            current_node = current_node->next;
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedLinkedList<T, CompareFunc>::print_all(std::ostream& os) const
    {
        Node* current_node = root;
        os << "{ ";
        while(current_node)
        {
            os << current_node->value;
            if(current_node->next)
                os << ", ";
            current_node = current_node->next;
        }
        os << " }" << std::endl;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool SortedLinkedList<T, CompareFunc>::remove(const T& element)
    {
        if(!root)
            return false;

        int compare_result = CompareFunc(root->value, element);
        if(compare_result == 0)
        {
            Node* temp_node = root->next;
            delete root;
            root = temp_node;
            return true;
        }
        else if(compare_result > 0)
            return false;

        Node* current_node = root;

        while(true)
        {
            if(!current_node->next)
                return false;

            compare_result = CompareFunc(current_node->next->value, element);
            if(compare_result == 0)
            {
                Node* temp_node = current_node->next->next;
                delete current_node->next;
                current_node->next = temp_node;
                return true;
            }
            else if(compare_result > 0)
                return false;

            current_node = current_node->next;
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedLinkedList<T, CompareFunc>::for_each(std::function<void(const T&)> func) const
    {
        Node* current_node = root;
        while(current_node)
        {
            func(current_node->value);
            current_node = current_node->next;
        }
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> SortedLinkedList<T, CompareFunc>::find_all(const T& element) const
    {
        std::vector<T> result;
        Node* current_node = root;
        while(current_node)
        {
            int compare_result = CompareFunc(current_node->value, element);
            if(compare_result > 0)
                return result;
            else if(compare_result == 0)
                result.push_back(current_node->value);

            current_node = current_node->next;
        }
        return result;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> SortedLinkedList<T, CompareFunc>::find_all(const T& min, const T& max) const
    {
        std::vector<T> result;
        Node* current_node = root;
        while(current_node)
        {
            if(CompareFunc(current_node->value, max) > 0)
                return result;
            else if(CompareFunc(current_node->value, min) >= 0)
                result.push_back(current_node->value);

            current_node = current_node->next;
        }
        return result;
    }
}


#endif // SORTEDLINKEDLIST_H
