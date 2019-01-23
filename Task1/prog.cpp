#include "prog.h"
#include "OtherClass.h"
#include <iostream>
#include <exception>
#include <memory>
#include <clocale>
#include <cstdint>

using namespace std;


int main()
{
	setlocale(LC_ALL, "ru-RU");

	cout << "Какое задание ? (5  12)" << endl;
	int num;
	cin >> num;
	switch(num)
	{
	case 5:
		Prog::task1_5();
		break;
	case 12:
		Prog::task1_12();
		break;
	default:
		throw invalid_argument("Задание не имплементировано");
	}
}


void Prog::task1_5()
{
	int n, total = 0;
	cout << "Привет" << endl << "Введите N: ";
	cin >> n;

	for(int z = 0; z <= n; z++)
	{
		for(int y = 0; y < z; y++)
		{
			for(int x = 0; x < y; x++)
			{
				if(x * x + y * y == z * z)
				{
					total++;
					cout << x << "^2 + " << y << "^2 = " << z << "^2"
					     << "   (" << x * x << " + " << y * y << " = " << z * z << ")" << endl;
				}
			}
		}
	}
	cout << " : " << total << endl;
}

//(число виводиться, але в hex формі :Р)

void Prog::task1_12()
{
	uint_fast8_t power;
	cout << " ? ";
	cin >> power;

	//125 * 8 == 1000 бітів, як раз для 2^1000
	//порядок зі старшого члена до молодшого
	const uint_fast8_t arrLength = 125;
	unsigned char numData [125] = {};

	uint_fast8_t bytePos = arrLength - 1 - power / 8;

//	cout << "(байт: " << bytePos << " позиция: " << (power % 8) << ")" << endl;
	numData[bytePos] |= (1 << (power % 8));

	cout << "hex: " << hex;
	for(int i = 0; i < arrLength; i++)
	{
		cout << (int)numData[i];
	}
	cout << dec << endl;
}

