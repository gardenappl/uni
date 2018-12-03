#ifndef UTILS_H
#define UTILS_H

#include <stdexcept>
#include <stack>
#include <queue>

using namespace std;

template<typename T>
T safePop(stack<T>& argStack)
{
	if(argStack.empty())
		throw invalid_argument("Attempted to pop empty stack");
	T value = argStack.top();
	argStack.pop();

	return value;
}

template<typename T>
void print(queue<T> argQueue)
{
	while(!argQueue.empty())
	{
		cout << *(argQueue.front()) << ' ';
		argQueue.pop();
	}
}

#endif // UTILS_H
