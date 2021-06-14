#include <iostream>
#include <stack>
#include <sstream>
#include <cctype>
#include <cstdio>
#include <map>
#include <utility>
#include "Stack.h"

class Interpreter
{
public:
	bool debug = false;

	~Interpreter();

	void perform_program(std::istream& input_stream);
	bool perform_operation(std::istream& input_stream);
	void define_operator(std::istream& input_stream);

private:
	Stack<int> data_stack;
	std::map<std::string, std::stringstream*> custom_operators;
};

Interpreter::~Interpreter()
{
    for(auto& custom_operator : custom_operators)
    {
        delete custom_operator.second;
    }
}

//Returns true if there are more operations to go
bool Interpreter::perform_operation(std::istream& input_stream)
{
	int c = input_stream.peek();
	if(c == EOF)
		return false;


	//if(debug) std::cout << "(performing: " << (char)c << ")" << std::endl;
	if(std::isdigit(c))
	{
		int num;
		input_stream >> num;
		data_stack.push(num);
	}
	else
	{
		int num1;
		int num2;
		bool valid_char_operator = true;
		switch(c)
		{
		case '+':
			num1 = data_stack.top();
			data_stack.pop();

			num2 = data_stack.top();
			data_stack.pop();

			data_stack.push(num1 + num2);
			break;
		case '-':
			num1 = data_stack.top();
			data_stack.pop();

			num2 = data_stack.top();
			data_stack.pop();

			data_stack.push(num1 - num2);
			break;
		case '*':
			num1 = data_stack.top();
			data_stack.pop();

			num2 = data_stack.top();
			data_stack.pop();

			data_stack.push(num1 * num2);
			break;
		case '/':
			num1 = data_stack.top();
			data_stack.pop();

			num2 = data_stack.top();
			data_stack.pop();

			data_stack.push(num1 / num2);
			break;
		case '.':
			std::cout << data_stack.top() << ' ';
			data_stack.pop();
			break;
		case ':':
			input_stream.ignore();
			input_stream >> std::ws;
			define_operator(input_stream);
			break;
		default:
			valid_char_operator = false;
			break;
		}

		if(valid_char_operator)
		{
			input_stream.ignore();
		}
		else
		{
			std::string word;
			input_stream >> word;
			if(word == "MOD")
			{
				int num1 = data_stack.top();
				data_stack.pop();

				int num2 = data_stack.top();
				data_stack.pop();

				data_stack.push(num1 % num2);
			}
			else if(word == "PICK")
			{
				int num = data_stack.top();
				data_stack.pop();

				num = data_stack.get_value(num);

				data_stack.push(num);
			}
			else if(word == "ROLL")
			{
				int num = data_stack.top();
				data_stack.pop();

				StackPos<int> pos = data_stack.get(num);
				num = pos.get_value();
				data_stack.remove(pos);

				data_stack.push(num);
			}
			else
			{
				auto custom_operator = custom_operators.find(word);
				if(custom_operator != custom_operators.end())
				{
                    if(debug)
                        std::cout << "Custom operator start.";

                    perform_program(*custom_operator->second);

                    if(debug)
                        std::cout << "Custom operator end.";

                    //Reset stringstream
					custom_operator->second->clear();
					custom_operator->second->seekg(0);
				}
				else
					throw std::runtime_error("Unknown operator: " + word);
			}
		}
	}

	if(debug)
	{
		std::cout << std::endl << "Data stack: ";
		data_stack.print();
	}
	input_stream >> std::ws; //skip whitespace
	return true;
}

void Interpreter::perform_program(std::istream& input_stream)
{
	bool operator_in_progress = true;

	while(operator_in_progress)
	{
		operator_in_progress = perform_operation(input_stream);
	}
}

void Interpreter::define_operator(std::istream& input_stream)
{
	std::string operator_name;
	input_stream >> operator_name;

	std::stringstream* operator_ss = new std::stringstream;

	int c;
	while((c = input_stream.peek()) != ';' && c != EOF)
	{
		std::string temp_string;
		input_stream >> temp_string;
		*operator_ss << temp_string;
		*operator_ss << ' ';

		input_stream >> std::ws;
	}
	if(c == EOF)
		throw std::runtime_error("expected ;");

	operator_ss->seekp(0);
	custom_operators.insert(std::pair<std::string, std::stringstream*>(operator_name, operator_ss));
	if(debug)
	{
		std::cout << "Added new operator " << operator_name << std::endl;
	}
}


int main()
{
	Interpreter interpreter;
	interpreter.debug = true;

	std::stringstream test_stream(": CUBE 0 PICK 0 PICK * * ; 3 CUBE CUBE .");
    interpreter.perform_program(test_stream);

	interpreter.debug = false;

	std::cout << "\nEnter program: " << std::endl;

	std::string input;
	std::getline(std::cin, input);
	std::stringstream input_stream(input);

	try
	{
        interpreter.perform_program(input_stream);
	}
    catch(const std::exception& e)
    {
        std::cout << "Error: " << e.what() << std::endl;
    }


	return 0;
}
