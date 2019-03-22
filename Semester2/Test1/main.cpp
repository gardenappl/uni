#include <iostream>
#include <cmath>
#include <cstdlib>
#include <utility>
#include <ctime>

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
struct DoubleNode
{
    T value;
    DoubleNode<T>* next;
    DoubleNode<T>* prev;

    DoubleNode(T value, DoubleNode<T>* next = nullptr, DoubleNode<T>* prev = nullptr);
};

template<typename T>
DoubleNode<T>::DoubleNode(T value, DoubleNode<T>* next, DoubleNode<T>* prev)
: value(value), next(next), prev(prev)
{}




template<typename T>
struct DoubleLinkedList
{
    int count = 0;
    DoubleNode<T>* start = nullptr;
    DoubleNode<T>* end = nullptr;
    ~DoubleLinkedList();

    void insert_at_end(const T& value);
    void remove(DoubleNode<T>* node, bool delete_node);
    void print();
};

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList()
{
    DoubleNode<T>* current_node = this->start;
    while(current_node)
    {
        DoubleNode<T>* next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }
}

template<typename T>
void DoubleLinkedList<T>::insert_at_end(const T& value)
{
    DoubleNode<T>* new_node = new DoubleNode<T>(value);

    if(this->end == nullptr)
    {
        this->start = new_node;
        this->end = new_node;
    }
    else
    {
        this->end->next = new_node;
        new_node->prev = this->end;
        this->end = new_node;
    }
    this->count++;
}

template<typename T>
void DoubleLinkedList<T>::remove(DoubleNode<T>* node, bool delete_node)
{
    if(node == this->start)
        this->start = node->next;
    else
        node->prev->next = node->next;

    if(node == this->end)
        this->end = node->prev;
    else
        node->next->prev = node->prev;

    if(delete_node)
        delete node;
    this->count--;
}

template<typename T>
void DoubleLinkedList<T>::print()
{
    DoubleNode<T>* current_element = this->start;
    while(current_element)
    {
        std::cout << current_element->value << std::endl;
        current_element = current_element->next;
    }
}




template<typename T>
using DoubleKeyNode = DoubleNode<std::pair<int, T>>;

//T must implement int getHash() const;
//T must implement static T make_random();
template<typename T>
struct CycleKeyList
{
    DoubleKeyNode<T>* end = nullptr;
    int count = 0;
    ~CycleKeyList();

    void insert_at_end(const T& value);
    void remove(DoubleKeyNode<T>* node, bool delete_node);

    void print(bool only_keys = false) const;

    static int get_hash(const T& value);
};

template<typename T>
CycleKeyList<T>::~CycleKeyList()
{
    DoubleKeyNode<T>* current_node = this->end->next;
    while(current_node != this->end)
    {
        DoubleKeyNode<T>* next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }
    delete this->end;
}

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
        DoubleKeyNode<T>* new_node = new DoubleKeyNode<T>(std::pair<int, T>(CycleKeyList::get_hash(value), value));
        new_node->next = new_node;
        new_node->prev = new_node;
        this->end = new_node;
    }
    else
    {
        DoubleKeyNode<T>* start = this->end->next;

        DoubleKeyNode<T>* new_node = new DoubleKeyNode<T>(std::pair<int, T>(CycleKeyList::get_hash(value), value), start, this->end);

        start->prev = new_node;
        this->end->next = new_node;

        this->end = new_node;
    }
    count++;
}

template<typename T>
void CycleKeyList<T>::remove(DoubleKeyNode<T>* node, bool delete_node)
{
    if(node->next == node)
    {
        if(delete_node)
            delete node;
        this->end = nullptr;
    }
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        if(this->end == node)
            this->end = node->next;
        if(delete_node)
            delete node;
    }
    count--;
}

template<typename T>
void CycleKeyList<T>::print(bool only_keys) const
{
    if(this->end == nullptr)
    {
        std::cout << "Empty list!" << std::endl;
    }
    else
    {
        std::cout << this->count << " elements:" << std::endl;
        DoubleKeyNode<T>* current_node = this->end;
        do
        {
            if(only_keys)
            {
                std::cout << current_node->value.first << " ";
            }
            else
            {
                std::cout << current_node->value.second << " (key = " << current_node->value.first << ")" << std::endl;
            }
            current_node = current_node->next;
        }
        while(current_node != this->end);
        if(only_keys)
            std::cout << std::endl;
    }
}

template<typename T>
bool valid_sequence_neighbours(const DoubleKeyNode<T>& node, const DoubleKeyNode<T>& next_node)
{
    return std::abs(node.value.first - next_node.value.first) <= 1;
}

template<typename T>
void task2(CycleKeyList<T>& list)
{
    const int TOTAL_DELETED_MAX = 1327;
    int total_deleted = 0;

    //skip first sequence on the first run
    bool first_sequence = true;

    DoubleKeyNode<T>* current_node = list.end;
    DoubleKeyNode<T>* last_sequence_start = current_node;

    while(true)
    {
        if(current_node->next == last_sequence_start)
        {
            //We just looped through the entire list
            //This means there are no more sequences left
            std::cout << "No more sequences left; return." << std::endl;
            return;
        }

        if(valid_sequence_neighbours(*current_node, *current_node->next))
        {
            if(first_sequence)
            {
                current_node = current_node->next;
                continue;
            }

            //we're in a sequence; delete next element until sequence is over
            list.print(true);

            last_sequence_start = current_node->prev;
            int sequence_length = 2;
            //temporary storage to compare to the element after it
            DoubleKeyNode<T>* current_sequence_end = current_node->next;
            list.count--;
            while(true)
            {
                //remove current_sequence_end but not delete
                list.remove(current_sequence_end, false);
                /*current_node->next = current_sequence_end->next;
                current_sequence_end->next->prev = current_node;*/
                if(current_node->next == current_node) //infinite loop
                {
                    std::cout << "List is an infinite loop; return." << std::endl;
                    return;
                }
                if(valid_sequence_neighbours(*current_sequence_end, *current_node->next))
                {
                    delete current_sequence_end;
                    current_sequence_end = current_node->next;
                    sequence_length++;
                    list.count--;
                    continue;
                }
                break;
            }
            delete current_sequence_end;
            //delete current node (which is the sequence start)
            list.remove(current_node, true);
            /*current_node->prev->next = current_node->next;
            current_node->next->prev = current_node->prev;
            delete current_node;
            this->count--;*/
            std::cout << "Deleted " << sequence_length << " elements." << std::endl;
            list.print(true);

            total_deleted += sequence_length;

            current_node = last_sequence_start;
            for(int i = 0; i < sequence_length / 3; i++)
            {
                T value = T::make_random();
                DoubleKeyNode<T>* new_node =
                        new DoubleKeyNode<T>(std::pair<int, T>(CycleKeyList<T>::get_hash(value), value), current_node->next, current_node);
                current_node->next->prev = new_node;
                current_node->next = new_node;

                current_node = current_node->next;
                list.count++;
            }
            std::cout << "Inserted " << sequence_length / 3 << " elements. " << std::endl;
            list.print(true);
            std::cout << std::endl;
            //std::cin.get();
            if(total_deleted > TOTAL_DELETED_MAX)
            {
                std::cout << "Reached max deletions" << std::endl;
                return;
            }

        }
        else
        {
            first_sequence = false;
        }
        current_node = current_node->next;
    }
}





template<typename T>
struct Node
{
    T value;
    Node<T>* next;
    Node(T value, Node<T>* next = nullptr);
};

template<typename T>
Node<T>::Node(T value, Node<T>* next)
: value(value), next(next)
{}




template<typename T>
struct Stack
{
private:
    Node<T>* top = nullptr;

public:
    void push(const T& value);
    T pop();
    void print();
    ~Stack();
};

template<typename T>
Stack<T>::~Stack()
{
    Node<T>* current_node = this->top;
    while(current_node)
    {
        Node<T>* next_node = this->top->next;
        delete current_node;
        current_node = current_node->next;
    }
}

template<typename T>
void Stack<T>::push(const T& value)
{
    Node<T>* new_node = new Node<T>(value, this->top);
    this->top = new_node;
}

template<typename T>
T Stack<T>::pop()
{
    T value = this->top->value;
    Node<T>* next_node = this->top->next;
    delete this->top;
    this->top = next_node;
    return value;
}

template<typename T>
void Stack<T>::print()
{
    Node<T>* current_node = this->top;
    while(current_node != nullptr)
    {
        std::cout << current_node->value << std::endl;
        current_node = current_node->next;
    }
}



template<typename T>
void task3(DoubleLinkedList<T>& list)
{
    Stack<T> stack;

    int step_1_count = list.count / 4;
    for(int i = 0; i < step_1_count; i++)
    {
        T value = list.start->value;
        list.remove(list.start, true);
        stack.push(value);
    }
    std::cout << "List: " << std::endl;
    list.print();
    std::cout << std::endl;
    std::cout << "Stack: " << std::endl;
    stack.print();
    std::cout << std::endl;

    DoubleNode<T>* current_node = list.start;
    int steps = 0;
    while(current_node != nullptr)
    {
        T value1 = current_node->value;
        list.remove(current_node, false);
        T value2 = stack.pop();
        T value3 = T::make_random();
        int hashes[3] = { value1.get_hash() % 4, value2.get_hash() % 4, value3.get_hash() % 4 };

        std::cout << "List element: " << value1 << std::endl;
        std::cout << "Stack element: " << value2 << std::endl;
        std::cout << "Random element: " << value3 << std::endl;

        //quick & dirty check
        T push_value = value1;
        if(hashes[0] == hashes[1] && hashes[1] != hashes[2])
            push_value = value3;
        else if(hashes[0] == hashes[2] && hashes[1] != hashes[2])
            push_value = value2;
        else
            push_value = T::make_random();

        stack.push(push_value);

        std::cout << "Pushed element: " << push_value << std::endl << std::endl;
        steps++;
        if(steps % 10 == 0)
        {
            std::cout << "List: " << std::endl;
            list.print();
            std::cout << std::endl;
            std::cout << "Stack: " << std::endl;
            stack.print();
            std::cout << std::endl;
        }


        DoubleNode<T>* next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }

    std::cout << "Stack: " << std::endl;
    stack.print();
    std::cout << std::endl;
}




template<typename T>
struct LinkedList
{
    Node<T>* start = nullptr;
    Node<T>* end = nullptr;
    void insert_at_end(const T& value);
    void print() const;

    ~LinkedList();
};

template<typename T>
LinkedList<T>::~LinkedList()
{
    Node<T>* current_node = this->start;
    while(current_node)
    {
        Node<T>* next_node = current_node->next;
        delete current_node;
        current_node = next_node;
    }

}

template<typename T>
void LinkedList<T>::insert_at_end(const T& value)
{
    Node<T>* new_node = new Node<T>(value);

    if(this->end == nullptr)
    {
        this->end = new_node;
        this->start = new_node;
    }
    else
    {
        this->end->next = new_node;
        this->end = new_node;
    }
}

template<typename T>
void LinkedList<T>::print() const
{
    Node<T>* current_node = this->start;
    while(current_node)
    {
        std::cout << current_node->value << std::endl;
        current_node = current_node->next;
    }
}



template<typename T>
struct HashTable
{
private:
    const static int BUCKETS_COUNT = 101;
    LinkedList<std::pair<int, T>> buckets[BUCKETS_COUNT];

public:
    void add(const T& value);
    LinkedList<T> find_values(int hash) const;
};

template<typename T>
void HashTable<T>::add(const T& value)
{
    int hash = value.get_hash();
    this->buckets[hash % BUCKETS_COUNT].insert_at_end(std::pair<int, T>(hash, value));
}

template<typename T>
LinkedList<T> HashTable<T>::find_values(int hash) const
{
    LinkedList<T> result_list;

    Node<std::pair<int, T>>* current_node = buckets[hash % BUCKETS_COUNT].start;
    while(current_node)
    {
        if(current_node->value.first == hash)
            result_list.insert_at_end(current_node->value.second);
        current_node = current_node->next;
    }
    return result_list;
}


int main()
{
    std::srand(std::time(nullptr));

    //TASK 1

    DoubleLinkedList<Triangle> list1;

    list1.insert_at_end(Triangle::make_random());
    list1.insert_at_end(Triangle::make_random());

    list1.print();

    std::cin.get();


    //TASK 2

    std::cout << std::endl;

    CycleKeyList<Triangle> list2;
    for(int i = 0; i < 10000; i++)
    {
        list2.insert_at_end(Triangle::make_random());
    }

    task2(list2);

    std::cin.get();


    //TASK 3

    DoubleLinkedList<Triangle> list3;
    for(int i = 0; i < 100; i++)
    {
        list3.insert_at_end(Triangle::make_random());
    }
    list3.print();
    task3(list3);

    std::cin.get();



    //TASK 4
    HashTable<Triangle> list4;
    for(int i = 0; i < 100000; i++)
    {
        list4.add(Triangle::make_random());
    }
    std::cout << "Search result: " << std::endl;
    list4.find_values(2000).print();

    std::cin.get();

    return 0;
}
