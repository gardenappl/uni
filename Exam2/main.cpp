#include <iostream>
#include <fstream>
#include <cmath>
#include <limits.h>
#include <iterator>


using namespace std;


//Task 2



struct AlgebraVector
{
	float x, y;

	AlgebraVector(float x = 0, float y = 0);

	AlgebraVector operator+(const AlgebraVector& v2) const;
	 //not overriding * operator to avoid ambiguity with vector product
	static float scalarProduct(const AlgebraVector& v1, const AlgebraVector& v2);


	float getLength() const;
	//public because it's used in physics engines for performance reasons
	float getLengthSquared() const;
	//required for rotation
	//returns radians
	float getAngle() const;

	//mutates the vector instead of creating a new one
	//maybe a bad design???
	AlgebraVector& rotate(float angle, float xOrigin = 0, float yOrigin = 0);


	friend ostream& operator<<(ostream& os, const AlgebraVector& v);

	friend istream& operator>>(istream& is, AlgebraVector& v);
};

AlgebraVector::AlgebraVector(float x, float y)
: x(x), y(y)
{}

AlgebraVector AlgebraVector::operator+(const AlgebraVector& v2) const
{
	return AlgebraVector(this->x + v2.x, this->y + v2.y);
}

float AlgebraVector::scalarProduct(const AlgebraVector& v1, const AlgebraVector& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

ostream& operator<<(ostream& os, const AlgebraVector& v)
{
	os << '(' << v.x << ", " << v.y << ')';
	return os;
}

void eat(istream& is, char c)
{
	char readChar;
	is >> readChar;
	if(readChar != c)
	{
		throw runtime_error("Expected " + c);
	}
}

istream& operator>>(istream& is, AlgebraVector& v)
{
	eat(is, '(');

	float num;
	is >> num;
	v.x = num;

	eat(is, ',');

	is >> num;
	v.y = num;

	eat(is, ')');

	return is;
}

float AlgebraVector::getLengthSquared() const
{
	return this->x * this->x + this->y * this->y;
}

float AlgebraVector::getLength() const
{
	return sqrt(getLengthSquared());
}

float AlgebraVector::getAngle() const
{
	return atan2(y, x);
}

//copy-paste from Stack Overflow, heyo
AlgebraVector& AlgebraVector::rotate(float angle, float xOrigin, float yOrigin)
{
	this->x = ((this->x - xOrigin) * cos(angle)) - ((yOrigin - this->y) * sin(angle)) + xOrigin;
	this->y = ((yOrigin - this->y) * cos(angle)) - ((this->x - xOrigin) * sin(angle)) + yOrigin;

	return *this;
}





//Task 1





void processFile(const string& inputFile, const string& outputFile)
{
	ifstream inputStream;
	inputStream >> noskipws; //handle whitespace manually
	inputStream.open(inputFile);

	ofstream outputStream;
	outputStream.open(outputFile);

	if(inputStream.is_open())
	{
		string word;
		while(inputStream >> word)
		{
			outputStream << word;
			cout << word;

			bool placeNewLine = false;

			char nextChar;
			while(nextChar = inputStream.peek())
			{
				if(nextChar == '\n')
				{
					if(!placeNewLine)
					{
						placeNewLine = true;
					}
					inputStream.ignore(1);
				}
				else if(iswspace(nextChar))
				{
					inputStream.ignore(1);
				}
				else
				{
					break;
				}
			}

			if(placeNewLine)
			{
				outputStream << endl;
				cout << endl;
			}
			else //place whitespace
			{
				outputStream << ' ';
				cout << ' ';
			}
		}
	}


	inputStream.close();
	outputStream.close();
}





//Task 4


//Efficient algorithm using a continuous one-dimensional array instead of array of arrays
//(idea stolen from S0)


struct Matrix
{
	int width;
	int height;

	Matrix(int* values, int width, int height)
	: width(width), height(height), values(values)
	{}

	Matrix(const Matrix& matrix);

	~Matrix();

	int get(int x, int y) const;
	void set(int x, int y, int value);

	Matrix rotateClockwise() const;

	void prettyPrint() const;

private:
	int* values;
	inline int index(int x, int y) const;
};

Matrix::Matrix(const Matrix& matrix)
{
	values = new int[matrix.width * matrix.height];
	width = matrix.width;
	height = matrix.height;
	copy(matrix.values, matrix.values + matrix.width * matrix.height, values);
}

Matrix::~Matrix()
{
	delete [] values;
}

inline int Matrix::index(int x, int y) const
{
	return x + width * y;
}

int Matrix::get(int x, int y) const
{
	return this->values[this->index(x, y)];
}

void Matrix::set(int x, int y, int value)
{
	this->values[this->index(x, y)] = value;
}

Matrix Matrix::rotateClockwise() const
{
    Matrix rotatedMatrix(new int[this->width * this->height], this->height, this->width);

	for(int x = 0; x < this->width; x++)
	{
		for(int y = 0; y < this->height; y++)
		{
			rotatedMatrix.set(this->height - 1 - y, x, this->get(x, y));
		}
	}
	return rotatedMatrix;
}

//not a << overload because it messes with newlines and stuff
void Matrix::prettyPrint() const
{
	cout << endl;
	for(int y = 0; y < this->height; y++)
	{
		for(int x = 0; x < this->width; x++)
		{
			cout << this->get(x, y) << ' ';
		}
		cout << endl;
	}
}


//Task 3

//Naive depth-first search using Matrix class from previous task (yay)

//Matrix argument: 0 = empty tile, 1 = occupied, 2 = already visited, 3 = end point

void findShortestPath(Matrix& matrix, int x, int y, int* currentMinimum, int currentLength = 0)
{
	if(currentLength >= *currentMinimum)
		return;
	if(x < 0 || x >= matrix.width || y < 0 || y >= matrix.height)
		return;
	if(matrix.get(x, y) == 3)
	{
		*currentMinimum = min(*currentMinimum, currentLength);
		return;
	}
	else if(matrix.get(x, y) != 0)
		return;

	Matrix matrixCopy = Matrix(matrix); //copy
	matrixCopy.set(x, y, 2);
//	matrixCopy.prettyPrint();
//	cout << endl;

	currentLength++;

    findShortestPath(matrixCopy, x,     y - 1, currentMinimum, currentLength);
    findShortestPath(matrixCopy, x + 1, y - 1, currentMinimum, currentLength);
    findShortestPath(matrixCopy, x + 1, y,     currentMinimum, currentLength);
    findShortestPath(matrixCopy, x + 1, y + 1, currentMinimum, currentLength);
    findShortestPath(matrixCopy, x,     y + 1, currentMinimum, currentLength);
    findShortestPath(matrixCopy, x - 1, y + 1, currentMinimum, currentLength);
    findShortestPath(matrixCopy, x - 1, y,     currentMinimum, currentLength);
    findShortestPath(matrixCopy, x - 1, y - 1, currentMinimum, currentLength);
}




int main()
{
	processFile("input.txt", "output.txt");

	AlgebraVector sum = AlgebraVector(5, 10) + AlgebraVector(15, -20);
	cout << sum << endl;
	cout << AlgebraVector::scalarProduct(AlgebraVector(5, 10), AlgebraVector(15, -20)) << endl;

	cout << AlgebraVector(2, 0).rotate(M_PI) << endl;

	AlgebraVector userInputVector;
	cout << "Input vector (notation: (a, b), whitespaces ignored): ";
	cin >> userInputVector;
	cout << userInputVector;

	int* a = new int[12]
	{
		0, 1, 2, 3,
		4, 5, 6, 7,
		8, 9, 0, 0
	};

	Matrix m = Matrix(a, 4, 3);
	m.prettyPrint();
	m.rotateClockwise().prettyPrint();

	int* a2 = new int[15]
	{
		0, 1, 0, 0, 0,
		3, 1, 0, 1, 0,
		0, 0, 0, 1, 0
	};
	Matrix m2 = Matrix(a2, 5, 3);
	int minimum = INT_MAX;
	findShortestPath(m2, 4, 2, &minimum);
	cout << minimum;

	return EXIT_SUCCESS;
}
