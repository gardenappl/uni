#ifndef SORTEDARRAYLIST_H
#define SORTEDARRAYLIST_H

#include "SortedList.h"

#include <iostream>
#include <functional>
#include <vector>


namespace lists
{
    template<typename T, CompareFuncType<T> CompareFunc>
    struct SortedArrayList : public SortedList<T, CompareFunc>
    {
        SortedArrayList()
        {}

        void add(const T& element) override;
        void print_all(std::ostream& os) const override;
        bool remove(const T& element) override;
        bool contains(const T& element) const override;
        std::vector<T> find_all(const T& min, const T& max) const override;
        void for_each(std::function<void(const T&)> func) const override;

    private:
        std::vector<T> data;
        int binary_search(const T& element, int start, int end) const;
        void binary_search(const T& min, const T& max, int start, int end, std::vector<T>& result) const;
    };

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::add(const T& element)
    {
        for(auto it = data.begin(); it != data.end(); it++)
        {
            if(CompareFunc(element, *it) < 0)
            {
                data.insert(it, element);
                return;
            }
        }
        data.push_back(element);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::print_all(std::ostream& os) const
    {
        os << "{ ";
        if(data.size() > 0)
            os << data[0];
        for(auto it = data.begin() + 1; it != data.end(); it++)
        {
            os << ", " << *it;
        }
        os << " }\n";
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool SortedArrayList<T, CompareFunc>::remove(const T& element)
    {
        int remove_index = binary_search(element, 0, data.size() - 1);
        if(remove_index == -1)
            return false;

        for(size_t i = remove_index + 1; i < data.size(); i++)
            data[i - 1] = data[i];

        data.pop_back();
        return true;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::for_each(std::function<void(const T&)> func) const
    {
        for(const T& value : data)
            func(value);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    bool SortedArrayList<T, CompareFunc>::contains(const T& element) const
    {
        return binary_search(element, 0, data.size() - 1) != -1;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    int SortedArrayList<T, CompareFunc>::binary_search(const T& element, int start, int end) const
    {
        if(end <= start)
            return CompareFunc(element, data[start]) == 0 ? start : -1;

//        std::cout << "start: " << start << " end: " << end <<std::endl;
        size_t middle_index = start + (end - start) / 2;
//        std::cout << "middle: " << middle_index << std::endl;

        int compare_result = CompareFunc(element, data[middle_index]);
        if(compare_result == 0)
            return middle_index;
        else if(compare_result < 0)
            return binary_search(element, start, middle_index - 1);
        else
            return binary_search(element, middle_index + 1, end);
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    std::vector<T> SortedArrayList<T, CompareFunc>::find_all(const T& min, const T& max) const
    {
        std::vector<T> result;
        binary_search(min, max, 0, data.size() - 1, result);
        return result;
    }

    template<typename T, CompareFuncType<T> CompareFunc>
    void SortedArrayList<T, CompareFunc>::binary_search(const T& min, const T& max, int start, int end, std::vector<T>& result) const
    {
        if(end < start)
            return;

        if(CompareFunc(data[start], min) >= 0 && CompareFunc(data[end], max) <= 0)
        {
            result.insert(result.end(), std::begin(data) + start, std::begin(data) + end + 1);
            return;
        }

        if(start == end)
            return;

//        std::cout << "start: " << start << " end: " << end <<std::endl;
        size_t middle_index = start + (end - start) / 2;
//        std::cout << "middle: " << middle_index << std::endl;

        if(CompareFunc(min, data[middle_index]) <= 0)
        {
            binary_search(min, max, start, middle_index, result);
        }
        if(CompareFunc(max, data[middle_index]) >= 0)
        {
            binary_search(min, max, middle_index + (end - start) % 2, end, result);
        }
    }
}

#endif // SORTEDARRAYLIST_H
