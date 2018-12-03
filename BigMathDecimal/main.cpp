#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

const uint32_t MAX_VALUE = 999999999;
const int MAX_DIGITS = 9;

int toInt(char const* s, int length)
{
     if ( s == nullptr || *s == '\0' )
        throw std::invalid_argument("null or empty string argument");

     int result = 0;
     for(int i = 0; i < length; i++)
     {
          if (s[i] >= '0' && s[i] <= '9')
              result = result * 10 + (s[i] - '0');
          else
              throw std::invalid_argument("invalid input string");
     }
     return result;
}

class BigInteger
{
private:
	uint32_t* data;
	int dataLength;

	BigInteger(uint32_t* data, int dataLength);

	static void addIntAtPos(uint32_t num, int pos, uint32_t* data);
	static void addIntArrayAtPos(uint32_t* nums, int count, int pos, uint32_t* data);
	static BigInteger add(uint32_t* data1, int dataLength1, uint32_t* data2, int dataLength2);
	static BigInteger multiply(uint32_t* data1, int dataLength1, uint32_t* data2, int dataLength2);
public:
	~BigInteger();

	static BigInteger parseString(string s);

	friend BigInteger operator+(const BigInteger& bigNum1, const BigInteger& bigNum2);
	friend BigInteger operator*(const BigInteger& bigNum1, const BigInteger& bigNum2);

	friend ostream& operator<<(ostream& os, const BigInteger& bigNum);
};

BigInteger::BigInteger(uint32_t* data, int dataLength)
{
	this->data = data;
	this->dataLength = dataLength;
}

BigInteger::~BigInteger()
{
//	cout << "deleted boi" << endl;
	delete[] data;
}

BigInteger BigInteger::parseString(string s)
{
	int dataLength = (s.length() - 1) / MAX_DIGITS + 1;
//	cout << "length: " << s.length() << " dataLength: " << dataLength << endl;
	uint32_t* data = new uint32_t[dataLength];
	for(int i = 0, lastIndex = s.length(); i < dataLength; i++)
	{
		int index = max(0, (int)s.length() - (i + 1) * MAX_DIGITS);
		string subString = s.substr(index, lastIndex - index);
//		cout << "substring: " << subString << endl;
		int num = toInt(&s[index], lastIndex - index);
		data[i] = num;

		lastIndex = index;
	}
	return BigInteger(data, dataLength);
}

void BigInteger::addIntAtPos(uint32_t num, int pos, uint32_t* data)
{
	for(int i = pos; ; i++)
	{
		if(MAX_VALUE - data[i] >= num) //no overflow
		{
			data[i] += num;
			return;
		}
		else //overflow
		{
			data[i] = (num + data[i]) % (MAX_VALUE + 1);
			num = 1;
		}
	}
}

void BigInteger::addIntArrayAtPos(uint32_t* nums, int count, int pos, uint32_t* data)
{
	for(int i = 0; i < count; i++)
	{
		BigInteger::addIntAtPos(nums[i], pos + i, data);
	}
}

BigInteger BigInteger::add(uint32_t* data1, int dataLength1, uint32_t* data2, int dataLength2)
{
	//data1 should be larger than data2
	if(dataLength2 > dataLength1)
	{
		swap(data1, data2);
		swap(dataLength1, dataLength2);
	}

	uint32_t* resultData = new uint32_t[dataLength1 + 1];
	copy(data1, data1 + dataLength1, resultData);
	resultData[dataLength1] = 0;

	BigInteger::addIntArrayAtPos(data2, dataLength2, 0, resultData);

	int resultLength = dataLength1;
	if(resultData[dataLength1] != 0)
	{
		resultLength++;
	}
	return BigInteger(resultData, resultLength);
}

BigInteger BigInteger::multiply(uint32_t* data1, int dataLength1, uint32_t* data2, int dataLength2)
{
	uint32_t* resultData = new uint32_t[dataLength1 + dataLength2];
	for(int i = 0; i < dataLength1 + dataLength2; i++)
	{
		resultData[i] = 0;
	}

	for(int i = 0; i < dataLength1; i++)
	{
		for(int j = 0; j < dataLength2; j++)
		{
			uint64_t product = (uint64_t)data1[i] * (uint64_t)data2[j];
			if(product > MAX_VALUE)
			{
				BigInteger::addIntAtPos((uint32_t)(product % (MAX_VALUE + 1)), i + j, resultData);
				BigInteger::addIntAtPos((uint32_t)(product / (MAX_VALUE + 1)), i + j + 1, resultData);
			}
			else
			{
				BigInteger::addIntAtPos((uint32_t)product, i + j, resultData);
			}
		}
	}

	int resultLength = dataLength1 + dataLength2;
	for(int i = resultLength - 1; i >= 0; i++)
	{
		if(resultData[i] == 0)
		{
			resultLength--;
		}
		else
		{
			break;
		}
	}

	return BigInteger(resultData, resultLength);
}

BigInteger operator+(const BigInteger& bigNum1, const BigInteger& bigNum2)
{
	return BigInteger::add(bigNum1.data, bigNum1.dataLength, bigNum2.data, bigNum2.dataLength);
}

BigInteger operator*(const BigInteger& bigNum1, const BigInteger& bigNum2)
{
	return BigInteger::multiply(bigNum1.data, bigNum1.dataLength, bigNum2.data, bigNum2.dataLength);
}

ostream& operator<<(ostream& os, const BigInteger& bigNum)
{
	for (int i = bigNum.dataLength - 1; i >= 0; i--)
	{
		if(i != bigNum.dataLength - 1)
		{
			os << setfill('0') << setw(MAX_DIGITS);
		}
		os << bigNum.data[i];
//		if (i != 0)
//			os << '_';
	}
//	os << " (pointer: " << bigNum.data << ", dataLen: " << bigNum.dataLength << ")";
	return os;
}

int main()
{
	string s;
	cout << "Enter number 1: ";
	cin >> s;
	BigInteger myInt = BigInteger::parseString(s);

	cout << "Enter number 2: ";
	cin >> s;
	BigInteger myInt2 = BigInteger::parseString(s);

	cout << endl << myInt << endl << '+' << endl << myInt2 << endl << '=' << endl << myInt + myInt2 << endl;
	cout << endl << myInt << endl << '*' << endl << myInt2 << endl << '=' << endl << myInt * myInt2 << endl;
	return 0;
}
