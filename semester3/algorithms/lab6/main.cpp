#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool check_substring(string text, string pattern)
{
	//'\0' - символ, €кий не може м≥ститис€ у введеному текст≥
	string str = pattern + '\0' + text;
	int* z = new int[str.length()];
	z[0] = 0;

	int left = 0, right = 0;
	for (int i = 1; i < str.length(); i++)
	{
		cout << i << endl;
		//«находимо початкове значенн€ z[i]
		if (i <= right)
			z[i] = min(right - i + 1, z[i - left]);
		else
			z[i] = 0;

		//ЌамагаЇмос€ його зб≥льшити
		while (i + z[i] < str.length() && str[z[i]] == str[i + z[i]])
			z[i]++;

		//якщо був в≥дсканований новий сегмент, записуЇмо його.
		if (i + z[i] - 1 > right)
		{
			left = i;
			right = i + z[i] - 1;
		}

		//якщо довжина сегменту б≥льша за довжину п≥д-строки, то вс€ п≥д-строка належить строц≥.
		if (z[i] >= pattern.length())
		{
			delete[] z;
			return true;
		}
	}
	delete[] z;
	return false;
}

bool check_cycled_string(string text1, string text2)
{
	return text1.length() == text2.length() && check_substring(text1 + text1, text2);
}

int main()
{
	string text1;
	cout << "Enter string:";
	cin >> text1;

	string text2;
	cout << "Enter another string:";
	cin >> text2;

	if (check_cycled_string(text1, text2))
		cout << text2 << " is a re-cycled " << text1 << "!\n";
	else
		cout << text2 << " is not a re-cycled " << text1 << "!\n";

	return 0;
}