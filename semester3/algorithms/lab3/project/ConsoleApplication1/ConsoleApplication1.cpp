#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;


size_t get_children_index(size_t index, int n)
{
	return index * n + 1;
}

size_t get_parent_index(size_t index, int n)
{
	return (index - 1) / n;
}



//internal function, used for insert();
void increase_key(int* heap, int n, size_t index, int new_key)
{
	heap[index] = new_key;
	while (index != 0)
	{
		size_t parent_index = get_parent_index(index, n);
		//In a max-heap, the parent must be larger than its children.
		if (heap[parent_index] < heap[index])
			swap(heap[parent_index], heap[index]);
		index = parent_index;
	}
}

void insert(int* heap, size_t& next_index, size_t length, int n, int element)
{
	if (next_index >= length)
		throw invalid_argument("Tried to insert into full array.");

	//Insert new element at the bottom layer (so, last element in the array)
	//And re-arrange until it becomes a max-heap again.
	increase_key(heap, n, next_index, element);
	next_index++;
}

int extract_max(int* heap, size_t& next_index, int n)
{
	if (next_index == 0)
		throw invalid_argument("Tried to extract from empty array.");

	//The largest element is at the top
	int return_element = heap[0];
	//Replace the top element with the last element
	heap[0] = heap[next_index - 1];

	size_t current_index = 0;
	while (true)
	{
		//In a max-heap, the parent must be larger than all its children.
		size_t children_index = get_children_index(current_index, n);
		size_t largest_index = current_index;
		for (size_t i = children_index; i < children_index + n && i < next_index; i++)
		{
			if (heap[i] > heap[largest_index])
				largest_index = i;
		}
		if (largest_index == current_index)
			break;
		else
		{
			swap(heap[largest_index], heap[current_index]);
			current_index = largest_index;
		}
	}

	next_index--;
	return return_element;
}


int main()
{
	cout << "Enter array length:\n";
	int length;
	cin >> length;
	int* heap_array = new int[length];

	const int n = 3;
	size_t next_index = 0;

	cout << "Enter " << length << " elements.\n";
	for (int i = 0; i < length; i++)
	{
		int new_element;
		cin >> new_element;
		insert(heap_array, next_index, length, n, new_element);
	}
	cout << "Sorted elements:\n";

	while (next_index > 0)
	{
		cout << extract_max(heap_array, next_index, n) << endl;
	}
	return 0;

}
