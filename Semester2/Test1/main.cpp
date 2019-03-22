#include <iostream>
#include <cmath>
#include <cstdlib>
#include <utility>

const int SUB_VARIANT = 1;




struct Point
{
    float x, y;
    Point(float x, float y);
};

Point::Point(float x, float y)
: x(x), y(y)
{}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "(x: " << point.x << ", y: " << point.y << ")";
    return os;
}




struct Triangle
{
    Point a, b, c;
    Triangle(Point a, Point b, Point c);
    float get_area() const;
    float get_perimeter() const;
    int get_hash() const; //not sure if this should be part of this struct but whatever

    static Triangle make_random();
};

Triangle::Triangle(Point a, Point b, Point c)
: a(a), b(b), c(c)
{}

Triangle Triangle::make_random()
{
    return Triangle(
            Point(((float)rand() / (float)RAND_MAX - 0.5f) * 100, ((float)rand() / (float)RAND_MAX - 0.5f) * 100),
            Point(((float)rand() / (float)RAND_MAX - 0.5f) * 100, ((float)rand() / (float)RAND_MAX - 0.5f) * 100),
            Point(((float)rand() / (float)RAND_MAX - 0.5f) * 100, ((float)rand() / (float)RAND_MAX - 0.5f) * 100)
    );
}

float get_distance(Point p1, Point p2)
{
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

float Triangle::get_perimeter() const
{
    return get_distance(this->a, this->b) + get_distance(this->b, this->c) + get_distance(this->a, this->c);
}

float Triangle::get_area() const
{
    float ab = get_distance(this->a, this->b);
    float bc = get_distance(this->b, this->c);
    float ac = get_distance(this->c, this->a);
    float half_perimeter = (ab + bc + ac) / 2;

    return std::sqrt(half_perimeter * (half_perimeter - ab) * (half_perimeter - bc) * (half_perimeter - ac));
}

int Triangle::get_hash() const
{
    return (int)std::floor(this->get_area() + (5 * SUB_VARIANT + 3) * this->get_perimeter());
}

std::ostream& operator<<(std::ostream& os, const Triangle& triangle)
{
    os << "{a: " << triangle.a << ", b: " << triangle.b << ", c: " << triangle.c << "}, hash: " << triangle.get_hash();
    return os;
}




//T must be printable to cout
template<typename T>
struct LinkedNode
{
    T value;
    LinkedNode<T>* next;
    LinkedNode<T>* prev;

    LinkedNode(T value, LinkedNode<T>* next = nullptr, LinkedNode<T>* prev = nullptr);
};

template<typename T>
LinkedNode<T>::LinkedNode(T value, LinkedNode<T>* next, LinkedNode<T>* prev)
: value(value), next(next), prev(prev)
{}




template<typename T>
struct LinkedList
{
    LinkedNode<T>* start = nullptr;
    LinkedNode<T>* end = nullptr;

    void insert_at_end(const T& value);
    void print();
};

template<typename T>
void LinkedList<T>::insert_at_end(const T& value)
{
    LinkedNode<T>* new_node = new LinkedNode<T>(value);

    if(this->end == nullptr)
    {
        this->start = new_node;
        this->end = new_node;
    }
    else
    {
        this->end->next = new_node;
        new_node->prev = this->end;
    }
}

template<typename T>
void LinkedList<T>::print()
{
    LinkedNode<T>* current_element = this->start;
    while(current_element)
    {
        std::cout << current_element->value << std::endl;
        current_element = current_element->next;
    }
}




template<typename T>
using LinkedKeyNode = LinkedNode<std::pair<int, T>>;

//T must implement int getHash() const;
//T must implement static T make_random();
template<typename T>
struct CycleKeyList
{
    LinkedNode<std::pair<int, T>>* end = nullptr;
    int count;

    void insert_at_end(const T& value);
    void print() const;

    void modify();

    static int get_hash(const T& value);
};

template<typename T>
int CycleKeyList<T>::get_hash(const T& value)
{
    return value.get_hash() % 11;
}

template<typename T>
void CycleKeyList<T>::insert_at_end(const T& value)
{
    if(this->end == nullptr)
    {
        LinkedKeyNode<T>* new_node = new LinkedKeyNode<T>(std::pair<int, T>(CycleKeyList::get_hash(value), value));
        new_node->next = new_node;
        new_node->prev = new_node;
        this->end = new_node;
    }
    else
    {
        LinkedKeyNode<T>* start = this->end->next;

        LinkedKeyNode<T>* new_node = new LinkedKeyNode<T>(std::pair<int, T>(CycleKeyList::get_hash(value), value), start, this->end);

        start->prev = new_node;
        this->end->next = new_node;

        this->end = new_node;
    }
    count++;
}

template<typename T>
void CycleKeyList<T>::print() const
{
    if(this->end == nullptr)
    {
        std::cout << "Empty list!" << std::endl;
    }
    else
    {
        std::cout << this->count << " elements:" << std::endl;
        LinkedKeyNode<T>* current_node = this->end;
        do
        {
            std::cout << current_node->value.second << " (key = " << current_node->value.first << ")" << std::endl;
            current_node = current_node->next;
        }
        while(current_node != this->end);
    }
}


template<typename T>
void CycleKeyList<T>::modify()
{
    const int TOTAL_DELETED_MAX = 1327;
    int total_deleted = 0;

    LinkedKeyNode<T>* sequence_start = this->end; //nullptr means we're not inside a sequence right now
    LinkedKeyNode<T>* sequence_end = this->end; //start and end are inclusive
    int sequence_length = 1;

    while(true)
    {
        bool deleted_this_cycle = false;
        if(sequence_start != nullptr)
        {
            if(std::abs(sequence_end->next->value.first - sequence_end->value.first) <= 1)
            {
                sequence_end = sequence_end->next;
                sequence_length++;
            }
            else //sequence is done, replacing/deleting
            {
                LinkedKeyNode<T>* insert_start_node = sequence_start->prev;
                LinkedKeyNode<T>* insert_end_node = sequence_end->next;

                //deleting
                LinkedKeyNode<T>* current_node = sequence_start;
                while(current_node != sequence_end)
                {
                    LinkedKeyNode<T>* next_node = current_node->next;
                    delete current_node;
                    current_node = next_node;
                }
                delete sequence_end;

                total_deleted = sequence_length;

                //adding new data
                LinkedKeyNode<T>* new_node;
                for(int i = 0; i < sequence_length / 3; i++)
                {
                    T value = T::make_random();
                    new_node = new LinkedKeyNode<T>(std::pair<int, T>(CycleKeyList::get_hash(value), value));

                    insert_start_node->next = new_node;
                    new_node->prev = insert_start_node;
                }
                insert_end_node->prev = new_node;
            }
        }
    }
}

int main()
{
    //TASK 1
    LinkedList<Triangle> list1;

    list1.insert_at_end(Triangle::make_random());
    list1.insert_at_end(Triangle::make_random());

    list1.print();


    //TASK 2
    std::cout << std::endl;

    CycleKeyList<Triangle> list2;
    list2.insert_at_end(Triangle::make_random());
    list2.insert_at_end(Triangle::make_random());
    list2.insert_at_end(Triangle::make_random());
    list2.insert_at_end(Triangle::make_random());
    list2.print();

    //list2.modify(); //DOES NOT WORK, has no end condition

    return 0;
}
