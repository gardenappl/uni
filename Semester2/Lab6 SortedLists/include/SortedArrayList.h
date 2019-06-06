#ifndef SORTEDARRAYLIST_H
#define SORTEDLINKEDLIST_H

#include "SortedList.h"

#include <iostream>
#include <functional>
#include <vector>


namespace lists
{
    template<typename T, CompareFuncType<T> CompareFunc>
    struct SortedArrayList : public SortedList<T, CompareFunc>
    {
        SortedArrayList();
        ~SortedArrayList() override;

        void add(const T& element) override;
        void print_all(std::ostream& os) const override;
        bool remove(const T& element) override;
        std::vector<T> find_all(const T& element) const override;
        std::vector<T> find_all(const T& min, const T& max) const override;
        void for_each(std::function<void(const T&)> func) const override;

    private:
        std::vector<T>* data;
    };

    template<typename T, CompareFuncType<T> CompareFunc>
    SortedArrayList<T, CompareFunc>::SortedArrayList()
    : data(new std::vector<T>())
    {}

    template<typename T, CompareFuncType<T> CompareFunc>
    SortedArrayList<T, CompareFunc>::~SortedArrayList()
    {
        delete data;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::add(const T& element)
    {
        for(auto it = data->begin(); it != data->end(); it++)
        {
            if(CompareFunc(element, *it) < 0)
            {
                data->insert(it, element);
                return;
            }
        }
        data->push_back(element);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::print_all(std::ostream& os) const
    {
        os << "{ ";
        if(data->size() > 0)
            os << (*data)[0];
        for(auto it = data->begin() + 1; it != data->end(); it++)
        {
            os << ", " << *it;
        }
        os << " }\n";
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool SortedArrayList<T, CompareFunc>::remove(const T& element)
    {
        for(auto it = data->begin(); it != data->end(); it++)
        {
            int compare_result = CompareFunc(element, *it);
            if(compare_result == 0)
            {
                data->erase(it);
                return true;
            }
            else if(compare_result < 0)
                return false;
        }
        return false;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::for_each(std::function<void(const T&)> func) const
    {
        for(const T& value : *data)
            func(value);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> SortedArrayList<T, CompareFunc>::find_all(const T& element) const
    {
        std::vector<T> result;
        for(const T& value : *data)
        {
            int compare_result = CompareFunc(element, value);
            if(compare_result == 0)
                result.push_back(value);
            else if(compare_result < 0)
                break;
        }
        return result;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> SortedArrayList<T, CompareFunc>::find_all(const T& min, const T& max) const
    {
        std::vector<T> result;
        for(const T& value : *data)
        {
            if(CompareFunc(value, max) > 0)
                break;
            else if(CompareFunc(value, min) >= 0)
                result.push_back(value);
        }
        return result;
    }
}

#endif // SORTEDARRAYLIST_H
