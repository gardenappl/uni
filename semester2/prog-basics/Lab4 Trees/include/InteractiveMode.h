#ifndef BENCHMARKMODE_H
#define BENCHMARKMODE_H

#include "Tree.h"
#include <list>
#include <cstdlib>

namespace trees
{
    class InteractiveMode
    {
        static Tree<int>* current_tree;
        static std::list<size_t>* clipboard_path;

        static std::list<size_t>* get_path();
        static void reset();
    public:
        static void main();
    };
}

#endif // BENCHMARKMODE_H
