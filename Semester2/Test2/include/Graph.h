#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <stdexcept>
#include <functional>
#include <iostream>
#include <utility>
#include <queue>

namespace test
{
    //bidirectional
    template<typename T, int N>
    struct MatrixGraph
    {
        static constexpr double NOT_CONNECTED = -1;
        int count = 0;
        double distance[N][N];
        std::map<T, int> value_to_index;
        std::map<int, T> index_to_value;

    //    void add(const T& value1, const T& value2);

        template<typename Iterator>
        static MatrixGraph* construct(Iterator start, Iterator end,
                std::function<double(T, T)> get_distance, double min_distance);

        void print(std::ostream& os);

        int get_component_count();
    };

//    template<typename T, int N>
//    double MatrixGraph<T, N>::NOT_CONNECTED = -1;

//template<typename T>
//void MatrixGraph::add(const T& value1, const T& value2, std::function<double(T, T)> get_distance)
//{
//    auto it1 = value_to_index.find(value1);
//    if(it1 == value_to_index.end())
//    {
//        if(count >= N)
//            throw std::out_of_range();
//        value_to_index.insert(it1, ++count);
//        index_to_value[count, value1];
//    }
//    auto it2 = value_to_index.find(value2);
//    if(it2 == value_to_index.end())
//    {
//        if(count >= N)
//            throw std::out_of_range();
//        value_to_index.insert(it2, ++count);
//        index_to_value[count, value2];
//    }
//
//    double distance = get_distance();
//    distance[*it1, *it2] = distance;
//    distance[*it2, *it1] = distance;
//}

    template<typename T, int N>
    template<typename Iterator>
    MatrixGraph<T, N>* MatrixGraph<T, N>::construct(Iterator start, Iterator end,
            std::function<double(T, T)> get_distance, double min_distance)
    {
        MatrixGraph<T, N>* graph = new MatrixGraph<T, N>;

        graph->count = end - start;
        for(int i = 0; i < graph->count; i++)
        {
            T value = *(start + i);

            graph->index_to_value[i] = value;
            graph->value_to_index[value] = i;

            graph->distance[i][i] = NOT_CONNECTED;

            for(int i2 = 0; i2 < i; i2++)
            {
                T value2 = *(start + i2);
                double calculated_distance = get_distance(value, value2);
                if(calculated_distance >= min_distance)
                {
                    graph->distance[i][i2] = calculated_distance;
                    graph->distance[i2][i] = calculated_distance;
                }
                else
                {
                    graph->distance[i][i2] = NOT_CONNECTED;
                    graph->distance[i2][i] = NOT_CONNECTED;
                }
            }
        }
        return graph;
    }

    template<typename T, int N>
    void MatrixGraph<T, N>::print(std::ostream& os)
    {
        for(int i = 0; i < count; i++)
        {
            for(int i2 = 0; i2 < i; i2++)
            {
                if(distance[i][i2] != -1)
                    os << "{" << index_to_value[i] << ", " << index_to_value[i2]
                            << ", distance: " << distance[i][i2] << "}\n";
            }
        }
    }

    template<typename T, int N>
    int MatrixGraph<T, N>::get_component_count()
    {
        int* components = new int[count];
        for(int i = 0; i < count; i++)
            components[i] = -1;

        int next_component_id = 0;

        for(int i = 0; i < count; i++)
        {
            if(components[i] != -1)
                continue;

            //Breadth-first search

            std::queue<int> bfs_queue;
            bfs_queue.push(i);

            while(!bfs_queue.empty())
            {
                int current_sub_node = bfs_queue.front();
                bfs_queue.pop();
                components[current_sub_node] = next_component_id;
                for(int i2 = 0; i2 < count; i2++)
                {
                    if(distance[current_sub_node][i2] != NOT_CONNECTED && components[i2] == -1)
                    {
                        bfs_queue.push(i2);
                    }
                }
            }
            next_component_id++;
        }
        return next_component_id - 1;
    }
}
#endif // GRAPH_H
