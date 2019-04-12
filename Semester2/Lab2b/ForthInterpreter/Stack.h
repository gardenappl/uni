#ifndef STACK_H
#define STACK_H

#include <stdexcept>

template<typename T>
struct Node
{
    T value;
    Node<T>* next;
    Node(const T& value, Node<T>* next = nullptr);
};

template<typename T>
Node<T>::Node(const T& value, Node<T>* next)
: value(value), next(next)
{}




//Encapsulates Node<T>.
//Can be used in some stack methods instead of the index, this allows for optimization.
template<typename T> struct Stack;

template<typename T>
struct StackPos
{
public:
    const T& get_value();

private:
    Node<T>* node;
    Node<T>* prev_node;
    StackPos(Node<T>* node, Node<T>* prev_node = nullptr);

    friend struct Stack<T>;
};

template<typename T>
StackPos<T>::StackPos(Node<T>* node, Node<T>* prev_node)
: node(node), prev_node(prev_node)
{}

template<typename T>
const T& StackPos<T>::get_value()
{
    return node->value;
}




template<typename T>
struct Stack
{
    Node<T>* start = nullptr;

    ~Stack();

    void push(const T& value);
    const T& top() const;
    void pop();
    void print() const;

    const T& get_value(int index);
    StackPos<T> get(int index);

    //void remove(int index);
    void remove(const StackPos<T>& pos);
};

template<typename T>
Stack<T>::~Stack()
{
    Node<T>* current_node = start;
    while(current_node)
    {
        delete current_node;
        current_node = current_node->next;
    }
}

template<typename T>
void Stack<T>::print() const
{
    Node<T>* current_node = start;
    while(current_node)
    {
        std::cout << current_node->value << ' ';
        current_node = current_node->next;
    }
    std::cout << std::endl;
}

template<typename T>
const T& Stack<T>::top() const
{
    if(!start)
        throw std::runtime_error("Attempted to get top element of empty stack.");
    return start->value;
}

template<typename T>
void Stack<T>::pop()
{
    if(!start)
        throw std::runtime_error("Attempted to pop empty stack.");
    Node<T>* old_start = start;
    start = start->next;
    delete old_start;
}

template<typename T>
void Stack<T>::push(const T& value)
{
    start = new Node<T>(value, start);
}

template<typename T>
StackPos<T> Stack<T>::get(int index)
{
    if(index == 0)
        return StackPos<T>(start);

    int i = 0;
    Node<T>* current_node = start;
    while(current_node->next)
    {
        if(i + 1 == index)
            return StackPos<T>(current_node->next, current_node);

        i++;
        current_node = current_node->next;
    }
    throw std::out_of_range("index out of range");
}

template<typename T>
const T& Stack<T>::get_value(int index)
{
    return get(index).get_value();
}

template<typename T>
void Stack<T>::remove(const StackPos<T>& pos)
{
    Node<T>* old_node = pos.node;
    pos.prev_node->next = pos.node->next;
    delete old_node;
}



#endif // STACK_H
