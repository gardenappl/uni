#ifndef POINT_H
#define POINT_H

#include <ostream>

namespace sort
{
    struct Point
    {
    public:
        static bool print_distance;

        float x;
        float y;
        float z;
        Point(float x, float y, float z);
        Point();
        static Point get_random();



        static inline float get_distance(const Point& point1, const Point& point2);
    };

    const Point origin = Point(0, 0, 0);


    bool operator==(const Point& point1, const Point& point2);
    bool operator<(const Point& point1, const Point& point2);
    bool operator>(const Point& point1, const Point& point2);
    bool operator<=(const Point& point1, const Point& point2);
    bool operator>=(const Point& point1, const Point& point2);
    std::ostream& operator<<(std::ostream& os, const Point& point);
}

#endif // POINT_H
