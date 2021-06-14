#include <iostream>
#include <algorithm>

using namespace std;

//In this program, negative integers are treated as "0"s,
//positive integers are "1"s

bool is_zero(int element)
{
	return element < 0;
}
bool is_one(int element)
{
	return element >= 0;
}

void print(int* array, size_t size)
{
	cout << "{ ";
	if(size > 0)
		cout << array[0];
	for(size_t i = 1; i < size; i++)
		cout << ", " << array[i];
	cout << " }\n";
}

//Code is self-explanatory.

void in_place_fast_sort(int* array, size_t size)
{
	size_t last_zero_index = -1;
	for(size_t i = 0; i < size; i++)
	{
		if(is_zero(array[i]))
		{
			swap(array[last_zero_index + 1], array[i]);
			last_zero_index++;
		}
	}
}

void fast_stable_sort(int* array, size_t size)
{
	int* ones_array = new int[size];
	size_t ones_count = 0;
	int* zeros_array = new int[size];
	size_t zeros_count = 0;

	for(size_t i = 0; i < size; i++)
	{
		if(is_zero(array[i]))
		{
			zeros_array[zeros_count] = array[i];
			zeros_count++;
		}
		else
		{
			ones_array[ones_count] = array[i];
			ones_count++;
		}
	}
	copy(zeros_array, zeros_array + zeros_count, array);
	copy(ones_array, ones_array + ones_count, array + zeros_count);

	delete[] ones_array;
	delete[] zeros_array;
}

void in_place_stable_sort(int* array, size_t size)
{
	size_t last_zero_index = -1;
	for(size_t i = 0; i < size; i++)
	{
		if(is_zero(array[i]))
		{
			int element = array[i];
			for(size_t j = i - 1; j > last_zero_index; j--)
				array[j + 1] = array[j];
			array[last_zero_index + 1] = element;
			last_zero_index++;
		}
	}
}


int main() {
	size_t size;
	cout << "Enter array size: ";
	cin >> size;
	int* array = new int[size];

	cout << "Enter " << size << " numbers: ";
	for(size_t i = 0; i < size; i++)
	{
		cin >> array[i];
		print(array, i + 1);
	}

	int* array_copy = new int[size];

	copy(array, array + size, array_copy);
	cout << "In-place fast sort: ";
	in_place_fast_sort(array_copy, size);
	print(array_copy, size);

	copy(array, array + size, array_copy);
	cout << "Stable fast sort: ";
	fast_stable_sort(array_copy, size);
	print(array_copy, size);

	copy(array, array + size, array_copy);
	cout << "In-place stable sort: ";
	in_place_stable_sort(array_copy, size);
	print(array_copy, size);

	return 0;
}
