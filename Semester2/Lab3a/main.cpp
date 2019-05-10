#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "Point.h"
#include "Sort.h"
#include "Utils.h"
#include "DemoMode.h"
#include "Benchmark.h"


int main()
{
    char input = 0;
    while(input != 'd' && input != 'c')
    {
        std::cout << "Print out coordinates for points (c) or only print the distance from origin (d)?" << std::endl;
        std::cin >> input;
    }
    if(input == 'd')
        sort::Point::print_distance = true;

    std::srand(std::time(0));
//    sort::Benchmark::run();
    sort::DemoMode::run();

//    sort::Point* points = new sort::Point[5]
//    {
//        sort::Point(2, 0, 0),
//        sort::origin,
//        sort::Point(3, 2, 1),
//        sort::Point(1, 2, 3),
//        sort::origin
//    };
//    sort::print_array(points, 0, 5);
//    sort::insertion_sort(points, 5, true);
////    sort::quick_sort(points, 0, 4);
//    std::cout << std::endl;
//    sort::print_array(points, 0, 5);
//
//    delete[] points;
//
//    points = new sort::Point[30];
//    for(int i = 0; i < 30; i++)
//    {
//        points[i] = sort::Point::get_random();
//    }
//
//    sort::print_array(points, 0, 30);
//    std::cout << std::endl;
//    sort::merge_sort_optimized(points, 30, 10, true);
//    sort::print_array(points, 0, 30);
//    std::cout << std::endl << "Is sorted? " << std::boolalpha << std::is_sorted(points, points + 30)
//            << std::endl << std::endl;
//
//    delete[] points;
//
//    points = new sort::Point[30];
//    for(int i = 0; i < 30; i++)
//    {
//        points[i] = sort::Point::get_random();
//    }
//
//    sort::print_array(points, 0, 30);
//    std::cout << std::endl;
//    std::sort(points, points + 30);
//    sort::print_array(points, 0, 30);
//    std::cout << std::endl << "Is sorted? " << std::boolalpha << std::is_sorted(points, points + 30)
//            << std::endl << std::endl;
//
//    delete[] points;
//
//    return 0;
}
