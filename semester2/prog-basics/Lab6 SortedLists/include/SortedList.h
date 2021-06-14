#ifndef SORTEDLIST_H
#define SORTEDLIST_H

#include <iostream>
#include <functional>
#include <vector>

namespace lists
{
    template<typename T>
    using CompareFuncType = int(*)(const T&, const T&);

    template<typename T, CompareFuncType<T> CompareFunc>
    struct SortedList
    {
        SortedList() {}
        virtual ~SortedList() {}

        virtual void add(const T& element) = 0;
        virtual void print_all(std::ostream& os) const = 0;
        virtual bool remove(const T& element) = 0;
        virtual bool contains(const T& value) const = 0;
        virtual std::vector<T> find_all(const T& min, const T& max) const = 0;
        virtual void for_each(std::function<void(const T&)> func) const = 0;
    };
}

#endif // SORTEDLIST_H
