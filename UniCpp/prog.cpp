#include "prog.h"
#include "OtherClass.h"
#include <iostream>
#include <exception>
#include <memory>

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	shared_ptr<OtherClass> obj(new OtherClass());

	cout << "Какое задание? (5 или 12)" << endl;
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
	cout << "Всего результатов: " << total << endl;
}

//bool Prog::isPrime(const int& num) {
//	for(int i = 2; i <= num / 2; i++) {
//		if(num % i == 0) {
//			return false;
//		}
//	}
//	return true;
//}

void Prog::task1_12()
{
	int_fast16_t power;
	cout << "Какая степень? ";
	cin >> power;

	//125 chars == 1000 bits, so 2^1000 is OK
	//most significant digits go first
	const int_fast16_t arrLength = 125;
	unsigned char numData [125] = {};

	int_fast16_t bytePos = arrLength - 1 - power / 8;

//	cout << "(байт: " << bytePos << " позиция: " << (power % 8) << ")" << endl;
	numData[bytePos] |= (1 << (power % 8));

	cout << "hex: " << hex;
	for(int i = 0; i < arrLength; i++)
	{
		cout << (int)numData[i];
	}
	cout << dec << endl;
}

