#include "Point.h"
#include <cmath>
#include <iostream>
#include <cstdlib>

namespace sort
{
    Point::Point(float x, float y, float z) :
        x(x), y(y), z(z)
    {}

    Point::Point() :
        x(0), y(0), z(0)
    {}

    bool Point::print_distance = false;

    float Point::get_distance(const Point& point1, const Point& point2)
    {
        float x = (point1.x - point2.x);
        float y = (point1.y - point2.y);
        float z = (point1.z - point2.z);
        return std::sqrt(x * x + y * y + z * z);
    }

    bool operator==(const Point& point1, const Point& point2)
    {
        return point1.x == point2.x && point1.y == point2.y && point1.z == point2.z;
    }

    bool operator<(const Point& point1, const Point& point2)
    {
        float distance1 = Point::get_distance(point1, origin);
        float distance2 = Point::get_distance(point2, origin);

        if(distance1 != distance2)
            return distance1 < distance2;
        if(point1.x != point2.x)
            return point1.x < point2.x;
        if(point1.y != point2.y)
            return point1.y < point2.y;
        return point1.z < point2.z;
    }

    bool operator>(const Point& point1, const Point& point2)
    {
        return point2 < point1;
    }

    bool operator<=(const Point& point1, const Point& point2)
    {
        return !(point1 > point2);
    }

    bool operator>=(const Point& point1, const Point& point2)
    {
        return !(point1 < point2);
    }

    std::ostream& operator<<(std::ostream& os, const Point& point)
    {
        if(Point::print_distance)
        {
            return os << Point::get_distance(point, sort::origin);
        }
        else
        {
            return os << '(' << point.x << ", " << point.y << ", " << point.z << ')';
        }
    }

    Point Point::get_random()
    {
        float x = (float)std::rand() / (float)(RAND_MAX / 50);
        float y = (float)std::rand() / (float)(RAND_MAX / 20);
        float z = (float)std::rand() / (float)(RAND_MAX / 100);
        return Point(x, y, z);
    }
}
