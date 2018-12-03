#include <iostream>
#include <string>
#include <iostream>

using namespace std;

int subStringSearch(const string &text, const string &searchQuery, int textStart = 0, int searchQueryStart = 0)
{
	for(; textStart < text.length() - searchQuery.length() + 1; textStart++)
	{
		bool success = true;

//		cout << "new start: " << textStart << endl;
		for(int i1 = textStart, i2 = searchQueryStart; i2 < searchQuery.length(); i1++, i2++)
		{
//			cout << text[i1] << " " << searchQuery[i2] << endl;
			if(i1 >= text.length())
			{
				success = false;
				break;
			}

            if(searchQuery[i2] == '?')
				continue;

			if(searchQuery[i2] == '*')
			{
				if(i2 == searchQuery.length() - 1)
					return textStart;

				for(int newStart = i1; newStart < text.length() - (searchQuery.length() - i2); newStart++)
				{
					int result = subStringSearch(text, searchQuery, newStart, i2 + 1);
					if(result != -1)
						return result;
				}
				break;
			}
			else if(searchQuery[i2] != text[i1])
			{
//				cout << "Symbols don't match" << endl;
				success = false;
				break;
			}
		}

		if(success)
		{
			return textStart;
		}
	}
	return -1;
}

int main()
{
	string text;
	cout << "Enter text: ";
	cin >> text;

	string searchText;
	cout << "Enter search query: ";
	cin >> searchText;

	int result = subStringSearch(text, searchText);
	if(result != -1)
		cout << "Match! (" << result << ")" << endl;
	else
		cout << "Doesn't match!" << endl;

    return 0;
}
