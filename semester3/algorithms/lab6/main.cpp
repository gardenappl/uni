#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

bool check_substring(string text, string pattern)
{
	//'\0' - ������, ���� �� ���� �������� � ��������� �����
	string str = pattern + '\0' + text;
	int* z = new int[str.length()];
	z[0] = 0;

	int left = 0, right = 0;
	for (int i = 1; i < str.length(); i++)
	{
		cout << i << endl;
		//��������� ��������� �������� z[i]
		if (i <= right)
			z[i] = min(right - i + 1, z[i - left]);
		else
			z[i] = 0;

		//���������� ���� ��������
		while (i + z[i] < str.length() && str[z[i]] == str[i + z[i]])
			z[i]++;

		//���� ��� ������������ ����� �������, �������� ����.
		if (i + z[i] - 1 > right)
		{
			left = i;
			right = i + z[i] - 1;
		}

		//���� ������� �������� ����� �� ������� ��-������, �� ��� ��-������ �������� ������.
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