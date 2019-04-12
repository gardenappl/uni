#ifndef MYDEQUE_H
#define MYDEQUE_H

#include <vector>
#include <iostream>
#include <exception>
#include <string>
#include <cstddef>
#include <cstdlib>

namespace ErrorMessages
{
    static const std::string pop_empty_deque_message = "Attempted to pop empty stack.";
    static const std::string array_deque_overflow = "Attempted to insert too many elements into array deque.";
}

template<typename T>
struct Deque
{
    virtual ~Deque() {}

    virtual void print() const = 0;
    virtual void append_left(const T& value) = 0;
    virtual void append_right(const T& value) = 0;
    virtual T pop_left() = 0;
    virtual T pop_right() = 0;
    virtual bool is_empty() const = 0;
    virtual int get_count() const = 0;
};




template<typename T>
struct VectorDeque : public Deque<T>
{
    VectorDeque() {}

    void print() const override;
    void append_left(const T& value) override;
    void append_right(const T& value) override;
    T pop_left() override;
    T pop_right() override;
    bool is_empty() const override;
    int get_count() const override;

private:
    std::vector<T> data_vector;
};

template<typename T>
bool VectorDeque<T>::is_empty() const
{
    return data_vector.empty();
}

template<typename T>
int VectorDeque<T>::get_count() const
{
    return data_vector.size();
}

template<typename T>
void VectorDeque<T>::print() const
{
    std::cout << "{ ";
    if(!this->data_vector.empty())
    {
        std::cout << this->data_vector[0];
        for(int i = 1; i < this->data_vector.size(); i++)
        {
            std::cout << ", " << this->data_vector[i];
        }
    }
    std::cout << " }" << std::endl;
}

template<typename T>
void VectorDeque<T>::append_left(const T& value)
{
    this->data_vector.insert(this->data_vector.begin(), value);
}

template<typename T>
void VectorDeque<T>::append_right(const T& value)
{
    this->data_vector.push_back(value);
}

template<typename T>
T VectorDeque<T>::pop_left()
{
    if(this->data_vector.empty())
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }

    T value = this->data_vector[0];
    this->data_vector.erase(this->data_vector.begin());
    return value;
}

template<typename T>
T VectorDeque<T>::pop_right()
{
    if(this->data_vector.empty())
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }
    T value = this->data_vector.back();
    this->data_vector.pop_back();
    return value;
}




template<typename T>
struct ArrayDeque : public Deque<T>
{
    ArrayDeque<T>(size_t array_size);
    ~ArrayDeque<T>();

    void print() const override;
    void append_left(const T& value) override;
    void append_right(const T& value) override;
    T pop_left() override;
    T pop_right() override;
    bool is_empty() const override;
    int get_count() const override;

private:
    T* data_array;
    size_t array_size;
    size_t data_start = 0;
    size_t data_size = 0;
};

template<typename T>
ArrayDeque<T>::ArrayDeque(size_t array_size)
: data_array((T*)malloc(sizeof(T) * array_size)), array_size(array_size)
{}

template<typename T>
bool ArrayDeque<T>::is_empty() const
{
    return data_size == 0;
}

template<typename T>
int ArrayDeque<T>::get_count() const
{
    return data_size;
}

template<typename T>
ArrayDeque<T>::~ArrayDeque()
{
    delete data_array;
}

template<typename T>
void ArrayDeque<T>::append_left(const T& value)
{
    if(data_size == array_size)
    {
        throw std::runtime_error(ErrorMessages::array_deque_overflow);
    }
    data_start = (data_start == 0) ? (array_size - 1) : (data_start - 1);
    data_size++;
    data_array[data_start] = value;
}

template<typename T>
void ArrayDeque<T>::append_right(const T& value)
{
    if(data_size == array_size)
    {
        throw std::runtime_error(ErrorMessages::array_deque_overflow);
    }
    data_size++;
    data_array[(data_start + data_size - 1) % array_size] = value;
}

template<typename T>
void ArrayDeque<T>::print() const
{
//    std::cout << data_start << ' ' << data_size << ' ' << std::endl;
    std::cout << "{ ";
    if(data_size > 0)
    {
        std::cout << data_array[data_start];
    }
    for(size_t i = 1; i < data_size; i++)
    {
        std::cout << ", " << data_array[(data_start + i) % array_size];
    }
    std::cout << " }" << std::endl;
}

template<typename T>
T ArrayDeque<T>::pop_left()
{
    if(data_size == 0)
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }
    T value = data_array[data_start];
    data_start = (data_start + 1) % array_size;
    data_size--;
    return value;
}

template<typename T>
T ArrayDeque<T>::pop_right()
{
    if(data_size == 0)
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }
    T value = data_array[(data_start + data_size - 1) % array_size];
    data_size--;
    return value;
}




template<typename T>
struct DoubleNode
{
    T value;
    DoubleNode<T>* prev;
    DoubleNode<T>* next;
    DoubleNode<T>(T value, DoubleNode<T>* prev = nullptr, DoubleNode<T>* next = nullptr);
    static int total;
    ~DoubleNode();
};
template<typename T>
int DoubleNode<T>::total = 0;

template<typename T>
DoubleNode<T>::DoubleNode(T value, DoubleNode<T>* prev, DoubleNode<T>* next)
: value(value), prev(prev), next(next)
{
    DoubleNode<T>::total++;
}

template<typename T>
DoubleNode<T>::~DoubleNode()
{
    //std::cout << 'c';
    total--;
}


template<typename T>
struct LinkedDeque : public Deque<T>
{
    ~LinkedDeque();

    DoubleNode<T>* start = nullptr;
    DoubleNode<T>* end = nullptr;
    int count = 0;

    bool is_empty() const override;
    int get_count() const override;
    void append_left(const T& value) override;
    void append_right(const T& value) override;
    void print() const override;
    T pop_left() override;
    T pop_right() override;
};

template<typename T>
LinkedDeque<T>::~LinkedDeque()
{
//    std::cout << 'a';
    DoubleNode<T>* current_node = start;
    while(current_node)
    {
        DoubleNode<T>* old_node = current_node;
        current_node = current_node->next;
        delete old_node;
    }
}

template<typename T>
bool LinkedDeque<T>::is_empty() const
{
    return start == nullptr;
}

template<typename T>
int LinkedDeque<T>::get_count() const
{
    return count;
}

template<typename T>
void LinkedDeque<T>::append_left(const T& value)
{
//    std::cout << 'b';
    if(is_empty())
    {
        start = new DoubleNode<T>(value);
        end = start;
    }
    else
    {
        DoubleNode<T>* new_node = new DoubleNode<T>(value, nullptr, start);
        start->prev = new_node;
        start = new_node;
    }
    count++;
}

template<typename T>
void LinkedDeque<T>::append_right(const T& value)
{
//    std::cout << 'a';
    if(is_empty())
    {
        start = new DoubleNode<T>(value);
        end = start;
    }
    else
    {
        DoubleNode<T>* new_node = new DoubleNode<T>(value, end, nullptr);
        end->next = new_node;
        end = new_node;
    }
    count++;
}

template<typename T>
void LinkedDeque<T>::print() const
{
    std::cout << "{ ";
    DoubleNode<T>* current_node = start;
    while(current_node)
    {
        std::cout << current_node->value;// << '(' << current_node << ' ' << current_node->prev << ' ' << current_node->next << ')';
        if(current_node->next)
        {
            std::cout << ", ";
            current_node = current_node->next;
        }
        else
        {
            break;
        }
    }
    std::cout << " }" << std::endl;
}

template<typename T>
T LinkedDeque<T>::pop_left()
{
    if(is_empty())
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }

    T value = start->value;
    DoubleNode<T>* old_start = start;
    start = old_start->next;
    if(old_start == end)
    {
        end = nullptr;
    }
    else
    {
        start->prev = nullptr;
    }
    //std::cout << "Deleting " << old_start << std::endl;
    delete old_start;
    count--;
    return value;
}

template<typename T>
T LinkedDeque<T>::pop_right()
{
    if(is_empty())
    {
        throw std::runtime_error(ErrorMessages::pop_empty_deque_message);
    }

    T value = end->value;
    DoubleNode<T>* old_end = end;
    end = old_end->prev;
    if(old_end == start)
    {
        start = nullptr;
    }
    else
    {
        end->next = nullptr;
    }
    //std::cout << "Deleting " << old_end << std::endl;
    delete old_end;
    count--;
    return value;
}

#endif // MYDEQUE_H
