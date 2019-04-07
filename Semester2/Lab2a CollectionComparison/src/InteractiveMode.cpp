#include "InteractiveMode.h"
#include "IpAddress.h"
#include "iostream"
#include "exception"

Deque<IpAddress>* InteractiveMode::current_deque = nullptr;

void InteractiveMode::run()
{
	std::cout << "Welcome to the Interactive Deque Experience" << std::endl;
	std::cout << "Available commands: create-deque, append-left, append-right, pop-left, pop-right, print, quit" << std::endl;
	std::cout << "A VectorDeque has been created." << std::endl;
	InteractiveMode::current_deque = new VectorDeque<IpAddress>;

	std::string input;
	while(true)
	{
		std::cout << "Enter command: ";
		std::cin >> input;
		try
		{
			if(input == "create-deque")
			{
				Deque<IpAddress>* new_deque;

				while(true)
				{
					std::cout << "Select deque type (vector, array, linked): ";
					std::cin >> input;
					if(input == "vector")
					{
						std::cout << "Created new VectorDeque." << std::endl;
						new_deque = new VectorDeque<IpAddress>;
						break;
					}
					else if(input == "array")
					{
						std::cout << "Select size: ";
						int size;
						std::cin >> size;
						new_deque = new ArrayDeque<IpAddress>((size_t)size);
						std::cout << "Created new ArrayDeque with size " << (size_t)size << "." << std::endl;
						break;
					}
					else if(input == "linked")
					{
						std::cout << "Created new VectorDeque." << std::endl;
						new_deque = new LinkedDeque<IpAddress>;
						break;
					}
					else
					{
						std::cout << "Unknown type. Try again." << std::endl;
					}
				}

				delete InteractiveMode::current_deque;
				std::cout << "Deleted old deque." << std::endl;
				InteractiveMode::current_deque = new_deque;
			}
			else if(input == "append-left")
			{
				IpAddress address = read_ip_from_user_input();
				InteractiveMode::current_deque->append_left(address);
				std::cout << "Appended " << address << " to the left." << std::endl;
			}
			else if(input == "append-right")
			{
				IpAddress address = read_ip_from_user_input();
				InteractiveMode::current_deque->append_right(address);
				std::cout << "Appended " << address << " to the left." << std::endl;
			}
			else if(input == "pop-left")
			{
				IpAddress address = InteractiveMode::current_deque->pop_left();
				std::cout << "Popped " << address << " from the left." << std::endl;
			}
			else if(input == "pop-right")
			{
				IpAddress address = InteractiveMode::current_deque->pop_right();
				std::cout << "Popped " << address << " from the left." << std::endl;
			}
			else if(input == "print")
			{
				InteractiveMode::current_deque->print();
			}
			else if(input == "quit")
			{
				exit(0);
			}
			else
			{
				std::cout << "Unknown command" << std::endl;
			}
		}
		catch(const std::runtime_error& error)
		{
			std::cout << "An error occured while performing command: " << error.what() << std::endl;
		}
	}
}

IpAddress InteractiveMode::read_ip_from_user_input()
{
	std::cout << "Enter 4 bytes of the IPv4 address:" << std::endl;
	int bytes[4];
	for(int i = 0; i < 4; i++)
	{
		while(true)
		{
			std::cout << "Byte #" << i << ": ";
			std::cin >> bytes[i];
			if(bytes[i] < 0 || bytes[i] > 255)
			{
				std::cout << "A byte needs to be in the range [0, 255]. Try again." << std::endl;
			}
			else
			{
				break;
			}
		}

	}
	return IpAddress(bytes[0], bytes[1], bytes[2], bytes[3]);
}
