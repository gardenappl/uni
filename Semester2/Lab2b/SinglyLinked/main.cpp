#include <iostream>
#include <algorithm>
#include <stdexcept>

template<typename T>
struct Node
{
    T value;
    Node<T>* next = nullptr;
    Node<T>(T value, Node<T>* next = nullptr);
};

template<typename T>
Node<T>::Node(T value, Node<T>* next)
: value(value), next(next)
{}

template<typename T>
struct LinkedList
{
    Node<T>* start = nullptr;
    Node<T>* end = nullptr;

    ~LinkedList<T>();

    LinkedList<T>* reverse(int start_index, int end_index);
    void append_start(const T& value);
    void append_end(const T& value);
    void print() const;
};

template<typename T>
LinkedList<T>::~LinkedList()
{
    Node<T>* current_node = this->start;
    while(current_node)
    {
        Node<T>* current_node_old = current_node;
        current_node = current_node->next;
        delete current_node_old;
    }
}

template<typename T>
void LinkedList<T>::append_start(const T& value)
{
    if(!start)
    {
        start = new Node<T>(value);
        end = start;
    }
    else
    {
        start = new Node<T>(value, start);
    }
}

template<typename T>
void LinkedList<T>::append_end(const T& value)
{
    if(!start)
    {
        start = new Node<T>(value);
        end = start;
    }
    else
    {
        end->next = new Node<T>(value);
        end = end->next;
    }
}

template<typename T>
void LinkedList<T>::print() const
{
	std::cout << "{ ";
	if(start)
	{
		std::cout << start->value;
		Node<T>* current_element = start->next;
		while(current_element)
		{
			std::cout << ", " << current_element->value;
			current_element = current_element->next;
		}
	}
	std::cout << " }" << std::endl;
}

template<typename T>
LinkedList<T>* LinkedList<T>::reverse(int start_index, int end_index)
{
    if(start_index < 0)
        throw std::out_of_range("min value out of range!");
    int min_index = std::min(start_index, end_index);
    int max_index = std::max(start_index, end_index);

    int i = 0;
    Node<T>* current_node = start;
    LinkedList<T>* result_list = new LinkedList<T>;
    while(current_node)
    {
        if(i >= min_index)
        {
            if(i > max_index)
                return result_list;

            if(start_index < end_index)
                result_list->append_start(current_node->value);
            else
                result_list->append_end(current_node->value);
        }
        current_node = current_node->next;
        i++;
    }
    if(max_index > i + 1)
        throw std::out_of_range("max value out of range!");
    return result_list;
}



void demo_reverse()
{
    std::cout << "\n=== DEMO START===\n";
    LinkedList<int> list;
    for(int i = 0; i <= 10; i++)
        list.append_end(i);
    list.print();

    std::cout << std::endl;
    std::cout << "Start index: 0, end index: 10" << std::endl;
    LinkedList<int>* reverse_list = list.reverse(0, 10);
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
    LinkedList<int> list;
    std::cout << "Available commands: demo-reverse, reset-list, append, print, print-reverse, quit" << std::endl;

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
				list = LinkedList<int>();
			}
			else if(input == "append")
			{
				std::cout << "Enter number: ";
				int num;
				std::cin >> num;
				list.append_end(num);
				std::cout << "Added " << num << '.' << std::endl;
			}
			else if(input == "print")
			{
				list.print();
			}
			else if(input == "print-reverse")
			{
                std::cout << "Enter start index: ";
                int start_index;
                std::cin >> start_index;

                std::cout << "Enter end index: ";
                int end_index;
                std::cin >> end_index;

                LinkedList<int>* reverse_list = list.reverse(start_index, end_index);
                reverse_list->print();
                delete reverse_list;
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
