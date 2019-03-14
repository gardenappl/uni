#include <math.h>
#include <iostream>
#include <utility>

using namespace std;

//Task 1

const double EPSILON = 0.00001;

int factorial(int n)
{
	int result = 1;
	for(int i = 2; i <= n; i++)
	{
		result *= i;
	}
	return result;
}

void task1()
{
	double result = 0;
	double diff = 0;

	for(int i = 0; ; i++)
	{
		//Progress bar
//		if(i % 10000 == 0)
//			cout << '.';

		diff = pow(-2, i) / factorial(i);
		result += diff;
		if(abs(diff) < EPSILON)
			break;
	}

	cout << endl << "Result: " << result << " (+- " << abs(diff) << ")" << endl;
}

//Task 2

void task2()
{
	int n;
	cout << "Enter N: ";
	cin >> n;

	int x;
	int y;
	int maxX = ceil(sqrt(n / 2));
	bool success = false;

	for(x = 1; x <= maxX; x++)
	{
		int ySquared = n - (x * x);

		y = (int)sqrt(ySquared);

		//check if y is an integer
        if(y != sqrt(ySquared))
			continue;

		success = true;
		break;
	}

	if(success)
	{
		cout << x << "^2 + " << y << "^2 = " << n << endl;
	}
	else
	{
		cout << "Result not found" << endl;
	}
}

//Task 3


int getRandomIntInRange(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

void task3()
{
	int triangleCount, coordMax;
	cout << "Enter triangle count (N): ";
	cin >> triangleCount;
	cout << "Enter coordinate bounds (M): ";
	cin >> coordMax;

	//Normally I'd use a struct/class for this but I don't have much time aaaaaaaaa
	//Each pair is point (x, y)
	pair<int, int> triangleData[triangleCount * 3];
    for(int i = 0; i < triangleCount * 3; i++)
	{
		triangleData[i] = make_pair(getRandomIntInRange(-coordMax, coordMax), getRandomIntInRange(-coordMax, coordMax));
	}

	//Very, very ugly
	for(int i = 0; i < triangleCount; i++)
	{
        int aSquared = triangleData[i * 3].first * triangleData[i * 3].first +
			triangleData[i * 3].second * triangleData[i * 3].second;

        int bSquared = triangleData[i * 3 + 1].first * triangleData[i * 3 + 1].first +
			triangleData[i * 3 + 1].second * triangleData[i * 3 + 1].second;

        int cSquared = triangleData[i * 3 + 2].first * triangleData[i * 3 + 2].first +
			triangleData[i * 3 + 2].second * triangleData[i * 3 + 2].second;

		cout << "Triangle N" << i + 1 << ": (" << triangleData[i * 3].first << "; " << triangleData[i * 3].second << ") "
			<< " (" << triangleData[i * 3 + 1].first << "; " << triangleData[i * 3 + 1].second << ") "
			<< " (" << triangleData[i * 3 + 2].first << "; " << triangleData[i * 3 + 2].second << ") ";
		if((aSquared + bSquared == cSquared) ||
			(aSquared + cSquared == bSquared) ||
			(bSquared + cSquared == aSquared))
		{
			cout << " is correct!" << endl;
			cout << "a^2 = " << aSquared << " b^2 = " << bSquared << " c^2 = " << cSquared << endl;
		}
		else
		{
			cout << endl;
		}
	}
}

//Task 4

void task4()
{
	string str;
	cout << "Enter string: ";
    cin >> str;

	string subStr;
    cout << "Enter substring: ";
    cin >> subStr;

    if(subStr.length() > str.length())
	{
		cout << "Not found (Substring is larger than string!)" << endl;
		return;
	}

    //Naive algorithm
    for(int i = 0; i < str.length(); i++)
	{
		//Try to match pattern at index
		bool success = true;
		for(int i1 = i, i2 = 0; i2 < subStr.length(); i1++, i2++)
		{
			if(str[i1] != subStr[i2])
			{
				success = false;
				break;
			}
		}

		if(success)
		{
			cout << "Found at: " << i << endl;
			return;
		}
	}
	cout << "Not found" << endl;
}

//Quick wrapper (I had 15 minutes left and didn't want to do task 5 so I did this instead)

int main()
{
	void (*tasks[])() = { &task1, &task2, &task3, &task4 };

	cout << "Choose task (1 to 4): ";
	int taskNum;
	cin >> taskNum;

	if(taskNum <= 0 || taskNum > 4)
	{
		cout << "Invalid task number!" << endl;
		return EXIT_SUCCESS;
	}
	else
	{
		(tasks[taskNum - 1])();
		return EXIT_SUCCESS;
	}
}
