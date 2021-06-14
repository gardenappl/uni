#include <iostream>
#include <stdexcept>
#include <cmath>



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
struct DoublyLinkedList
{
    int count = 0;
    DoubleNode<T>* start = nullptr;
    DoubleNode<T>* end = nullptr;
    ~DoublyLinkedList();

    void append(const T& value);
    void remove(DoubleNode<T>* node, bool delete_node);
    void print();
    DoublyLinkedList<T>* slice(int start_index, int count, int step = 1);
    DoublyLinkedList<T>* reverse(int start_index, int end_index);
};

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
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
void DoublyLinkedList<T>::append(const T& value)
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
DoublyLinkedList<T>* DoublyLinkedList<T>::reverse(int start_index, int end_index)
{
    if(end_index > start_index)
    {
        return slice(end_index, end_index - start_index + 1, -1);
    }
    else
    {
        return slice(end_index, start_index - end_index + 1, 1);
    }
}

template<typename T>
void DoublyLinkedList<T>::remove(DoubleNode<T>* node, bool delete_node)
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
void DoublyLinkedList<T>::print()
{
	std::cout << "{ ";
	if(start)
	{
		std::cout << start->value;
		DoubleNode<T>* current_element = start->next;
		while(current_element)
		{
			std::cout << ", " << current_element->value;
			current_element = current_element->next;
		}
	}
	std::cout << " }" << std::endl;
}

template<typename T>
DoublyLinkedList<T>* DoublyLinkedList<T>::slice(int start_index, int count, int step)
{
    DoublyLinkedList<T>* slice_list = new DoublyLinkedList<T>;
    if(count == 0)
        return slice_list;
    if(step == 0)
        throw std::invalid_argument("Step cannot be equal to 0!");

    DoubleNode<T>* current_node = start;
    //find start node
    int current_index = 0;
    while(current_node)
    {
        if(current_index == start_index)
            break;

        current_index++;
        current_node = current_node->next;
    }
    if(!current_node || current_index != start_index)
        throw std::out_of_range("Start index is out of bounds!");

    slice_list->append(current_node->value);

    int skip = std::abs(step) - 1;
    for(int i = 1; i < count;)
    {
        current_node = step > 0 ? current_node->next : current_node->prev;
        if(!current_node)
            throw std::out_of_range("End index is out of bounds!");

        if(skip > 0)
        {
            skip--;
        }
        else
        {
            slice_list->append(current_node->value);
            i++;
            skip = std::abs(step) - 1;
        }
    }
    return slice_list;
}


void demo_slice()
{
    std::cout << "\n=== DEMO START===\n";
    DoublyLinkedList<int> list;
    for(int i = 0; i <= 10; i++)
        list.append(i);
    list.print();

    std::cout << std::endl;
    std::cout << "Start index: 0, count: 2" << std::endl;
    DoublyLinkedList<int>* slice_list = list.slice(0, 2);
    slice_list->print();
    delete slice_list;

    std::cout << std::endl;
    std::cout << "Start index: 1, count: 5" << std::endl;
    slice_list = list.slice(1, 5);
    slice_list->print();
    delete slice_list;

    std::cout << std::endl;
    std::cout << "Start index: 2, count: 4, step: 2" << std::endl;
    slice_list = list.slice(2, 4, 2);
    slice_list->print();
    delete slice_list;

    std::cout << std::endl;
    std::cout << "Start index: 9, count: 5, step: -1" << std::endl;
    slice_list = list.slice(9, 5, -1);
    slice_list->print();
    delete slice_list;

    std::cout << std::endl;
    std::cout << "Start index: 8, count: 3, step: -2" << std::endl;
    slice_list = list.slice(8, 3, -2);
    slice_list->print();
    delete slice_list;
    std::cout << "\n=== DEMO END===\n";
}



void demo_reverse()
{
    std::cout << "\n=== DEMO START===\n";
    DoublyLinkedList<int> list;
    for(int i = 0; i <= 10; i++)
        list.append(i);
    list.print();

    std::cout << std::endl;
    std::cout << "Start index: 0, end index: 10" << std::endl;
    DoublyLinkedList<int>* reverse_list = list.reverse(0, 10);
    reverse_list->print();
    delete reverse_list;

    std::cout << std::endl;
    std::cout << "Start index: 3, end index: 5" << std::endl;
    reverse_list = list.reverse(3, 5);
    reverse_list->print();
    delete reverse_list;

    std::cout << std::endl;
    std::cout << "Start index: 8, end index: 4" << std::endl;
    reverse_list = list.reverse(8, 4);
    reverse_list->print();
    delete reverse_list;
    std::cout << "\n=== DEMO END===\n";
}





int main()
{
    DoublyLinkedList<int> list;
    std::cout << "Available commands: demo-slice, demo-reverse, reset-list, append, print, print-slice, print-reverse, quit" << std::endl;

    std::string input;
    while(true)
	{
		std::cout << "Enter command: ";
		std::cin >> input;
		try
		{
			if(input == "quit")
			{
				exit(0);
			}
			else if(input == "reset-list")
			{
				list = DoublyLinkedList<int>();
			}
			else if(input == "append")
			{
				std::cout << "Enter number: ";
				int num;
				std::cin >> num;
				list.append(num);
				std::cout << "Added " << num << '.' << std::endl;
			}
			else if(input == "print")
			{
				list.print();
			}
			else if(input == "print-slice")
			{
				std::cout << "Enter start index: ";
				int start_index;
				std::cin >> start_index;

				std::cout << "Enter slice element count: ";
				int count;
				std::cin >> count;

				std::cout << "Enter step (can be negative): ";
				int step;
				std::cin >> step;

				DoublyLinkedList<int>* slice_list = list.slice(start_index, count, step);
				slice_list->print();
				delete slice_list;
			}
			else if(input == "print-reverse")
			{
                std::cout << "Enter start index: ";
                int start_index;
                std::cin >> start_index;

                std::cout << "Enter end index: ";
                int end_index;
                std::cin >> end_index;

                DoublyLinkedList<int>* reverse_list = list.reverse(start_index, end_index);
                reverse_list->print();
                delete reverse_list;
			}
			else if(input == "demo-slice")
			{
                demo_slice();
			}
			else if(input == "demo-reverse")
			{
                demo_reverse();
			}
			else
			{
                std::cout << "Unknown command. Try again." << std::endl;
			}
		}
        catch(std::exception& e)
        {
            std::cout << "Error occured while performing command: " << e.what() << std::endl;
        }
    }

    return 0;
}
