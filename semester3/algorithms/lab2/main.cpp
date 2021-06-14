#include <iostream>
#include <algorithm>

using namespace std;


void print_array(int* array, size_t size)
{
	cout << "{ ";
	if(size == 0)
	{
		cout << "}" << endl;
		return;
	}

	cout << array[0];
	for(size_t i = 1; i < size; i++)
		cout << ", " << array[i];
	cout << " }" << endl;
}

size_t sort(int compare_bolt, int* nuts, size_t start, size_t end)
{
	bool found_match = false;
	size_t i = start;
	while(i <= end)
	{
		//If the nut is larger than our bolt, move the nut to the end of the array.
		if(compare_bolt < nuts[i])
		{
			std::swap(nuts[i], nuts[end]);
			end--;
		}
		//If the nut is smaller than our bolt and we haven't found a pair,
		//move on to the next nut
		else if(!found_match && compare_bolt > nuts[i])
		{
			i++;
		}
		//If the nut is smaller than our bolt and we have already found a pair,
		//swap nuts so that nuts[i] points to the paired nut
		//(and the smaller nut remains behind it)
		else if(found_match && compare_bolt > nuts[i])
		{
			std::swap(nuts[i - 1], nuts[i]);
			i++;
		}
		else if(!found_match && compare_bolt == nuts[i])
		{
			found_match = true;
			i++;
		}
	}
	return i - 1;
}

void sort_all(int* bolts, int* nuts, size_t start, size_t end)
{
	//Select the first bolt, split nuts into sub-arrays
	//(smaller than target bolt & larger than target bolt)
	size_t index = sort(bolts[start], nuts, start, end);
	//Select the nut that fits the first bolt, split nuts into sub-arrays
	//(smaller than target nut, larger than target nut)
	sort(nuts[index], bolts, start, end);

	cout << "Bolt size: " << bolts[index] << ", nut size: " <<
			nuts[index] << ", index: " << index << endl;

	//Recursion into the new sub-arrays
	//Don't need recursion if length of sub-array is 1
	if(index == start + 1)
		cout << "Bolt size: " << bolts[start] << ", nut size: " <<
				nuts[start] << ", index: " << start << endl;
	else if(index > start + 1)
		sort_all(bolts, nuts, start, index - 1);

	if(index == end - 1)
		cout << "Bolt size: " << bolts[end] << ", nut size: " <<
				nuts[end] << ", index: " << end << endl;
	else if(index < end - 1)
		sort_all(bolts, nuts, index + 1, end);
}


int main()
{
	cout << "Enter array length: " << endl;
	size_t length;
	cin >> length;

	cout << "Enter bolt sizes." << endl;
	int* bolts = new int[length];
	for(size_t i = 0; i < length; i++)
	{
		int size;
		cin >> size;
		bolts[i] = size;
		print_array(bolts, i + 1);
	}

	cout << "Enter nut sizes." << endl;
	int* nuts = new int[length];
	for(size_t i = 0; i < length; i++)
	{
		int size;
		cin >> size;
		nuts[i] = size;
		print_array(nuts, i + 1);
	}

	sort_all(bolts, nuts, 0, length - 1);
	cout << "Nuts array: ";
	print_array(nuts, length);
	cout << "Bolts array: ";
	print_array(bolts, length);
}
