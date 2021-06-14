#include <iostream>
#include <random>

using namespace std;





size_t get_index(int width, int x, int y)
{
	return width * y + x;
}

void print_matrix(int* matrix, int width, int height)
{
	for(int y = 0; y < height; y++)
	{
		for(int x = 0; x < width; x++)
		{
			cout << matrix[get_index(width, x, y)] << ' ';
		}
		cout << '\n';
	}
}

int* generate_random_matrix(int size, int min, int max)
{
	static default_random_engine rng_engine;
	uniform_int_distribution<int> element_rng(min, max);

	int* matrix = new int[size * size];
	for(size_t i = 0; i < size * size; i++)
		matrix[i] = element_rng(rng_engine);
	return matrix;
}

int* multiply_standard(int* m1, int* m2, int size)
{
	int* result = new int[size * size];
	for(int x = 0; x < size; x++)
	{
		for(int y = 0; y < size; y++)
		{
			int result_num = 0;
			for(int i = 0; i < size; i++)
			{
				result_num += m1[get_index(size, i, y)] * m2[get_index(size, x, i)];
			}
			result[get_index(size, x, y)] = result_num;
		}
	}
	return result;
}

//Strassen multiplication requires splitting a matrix into sub-matrices
//Instead of splitting, we make pointers using offset_x and offset_y
struct sq_matrix
{
	int* array;
	int size;
	int offset_x;
	int offset_y;
	int array_width;

	sq_matrix(int* array, int size, int offset_x = 0, int offset_y = 0,
			int array_width = -1)
	: array(array), size(size), offset_x(offset_x), offset_y(offset_y)
	{
		if(array_width == -1)
			this->array_width = size;
		else
			this->array_width = array_width;
	}
};

size_t get_index(const sq_matrix& m, int x, int y)
{
	return m.array_width * (m.offset_y + y) + (m.offset_x + x);
}
void print_matrix(const sq_matrix& m)
{
	for(int y = 0; y < m.size; y++)
	{
		for(int x = 0; x < m.size; x++)
		{
			cout << m.array[get_index(m, x, y)] << ' ';
		}
		cout << '\n';
	}
}

int* generate_random_array(size_t size, int min, int max)
{
	static default_random_engine rng_engine;
	uniform_int_distribution<int> element_rng(min, max);

	int* array = new int[size];
	for(size_t i = 0; i < size; i++)
		array[i] = element_rng(rng_engine);
	return array;
}

sq_matrix new_sq_matrix_zero(int size)
{
	int* array = new int[size * size];
	for(int i = 0; i < size * size; i++)
		array[i] = 0;
	return sq_matrix(array, size, 0, 0);
}

void add(const sq_matrix& m1, const sq_matrix& m2, sq_matrix& result)
{
	for(int x = 0; x < result.size; x++)
	{
		for(int y = 0; y < result.size; y++)
		{
			result.array[get_index(result, x, y)] =
					m1.array[get_index(m1, x, y)] + m2.array[get_index(m2, x, y)];
		}
	}
}

void subtract(const sq_matrix& m1, const sq_matrix& m2, sq_matrix& result)
{
	for(int x = 0; x < result.size; x++)
	{
		for(int y = 0; y < result.size; y++)
		{
			result.array[get_index(result, x, y)] =
					m1.array[get_index(m1, x, y)] - m2.array[get_index(m2, x, y)];
		}
	}
}

//Assume that matrix size is 2^n
void multiply_strassen(const sq_matrix& a, const sq_matrix& b, sq_matrix& result)
{
	if(a.size == 1)
	{
		result.array[get_index(result, 0, 0)] =
				a.array[get_index(a, 0, 0)] * b.array[get_index(b, 0, 0)];
		return;
	}

	size_t split_size = a.size / 2;

	sq_matrix a11(a.array, split_size, a.offset_x, 			   a.offset_y, a.array_width);
	sq_matrix a12(a.array, split_size, a.offset_x + a.size / 2, a.offset_y, a.array_width);
	sq_matrix a21(a.array, split_size, a.offset_x, 			   a.offset_y + a.size / 2, a.array_width);
	sq_matrix a22(a.array, split_size, a.offset_x + a.size / 2, a.offset_y + a.size / 2, a.array_width);

	sq_matrix b11(b.array, split_size, b.offset_x, 			   b.offset_y, b.array_width);
	sq_matrix b12(b.array, split_size, b.offset_x + b.size / 2, b.offset_y, b.array_width);
	sq_matrix b21(b.array, split_size, b.offset_x, 			   b.offset_y + b.size / 2, b.array_width);
	sq_matrix b22(b.array, split_size, b.offset_x + b.size / 2, b.offset_y + b.size / 2, b.array_width);

	sq_matrix c11(result.array, split_size, result.offset_x, 				  result.offset_y, result.array_width);
	sq_matrix c12(result.array, split_size, result.offset_x + result.size / 2, result.offset_y, result.array_width);
	sq_matrix c21(result.array, split_size, result.offset_x, 				  result.offset_y + result.size / 2, result.array_width);
	sq_matrix c22(result.array, split_size, result.offset_x + result.size / 2, result.offset_y + result.size / 2, result.array_width);


	sq_matrix temp1(new int[split_size * split_size], split_size);
	sq_matrix temp2(new int[split_size * split_size], split_size);

	sq_matrix p1(new int[split_size * split_size], split_size);
	sq_matrix p2(new int[split_size * split_size], split_size);
	sq_matrix p3(new int[split_size * split_size], split_size);
	sq_matrix p4(new int[split_size * split_size], split_size);
	sq_matrix p5(new int[split_size * split_size], split_size);
	sq_matrix p6(new int[split_size * split_size], split_size);
	sq_matrix p7(new int[split_size * split_size], split_size);

	//p1 = (a11 + a22) * (b11 + b22)
	add(a11, a22, temp1);
	add(b11, b22, temp2);
	multiply_strassen(temp1, temp2, p1);

	//p2 = (a21 + a22) * b11
	add(a21, a22, temp1);
	multiply_strassen(temp1, b11, p2);

	//p3 = a11 * (b12 - b22)
	subtract(b12, b22, temp1);
	multiply_strassen(a11, temp1, p3);

	//p4 = a22 * (b21 - b11)
	subtract(b21, b11, temp1);
	multiply_strassen(a22, temp1, p4);

	//p5 = (a11 + a12) * b22
	add(a11, a12, temp1);
	multiply_strassen(temp1, b22, p5);

	//p6 = (a21 - a11) * (b11 + b12)
	subtract(a21, a11, temp1);
	add(b11, b12, temp2);
	multiply_strassen(temp1, temp2, p6);

	//p7 = (a12 - a22) * (b21 + b22)
	subtract(a12, a22, temp1);
	add(b21, b22, temp2);
	multiply_strassen(temp1, temp2, p7);


	//c11 = p1 + p4 - p5 + p7
	add(p1, p4, c11);
	subtract(c11, p5, c11);
	add(c11, p7, c11);

	//c12 = p3 + p5
	add(p3, p5, c12);

	//c21 = p2 + p4
	add(p2, p4, c21);

	//c22 = p1 - p2 + p3 + p6
	subtract(p1, p2, c22);
	add(c22, p3, c22);
	add(c22, p6, c22);

//	print_matrix(c11);
//	print_matrix(c12);
//	print_matrix(c21);
//	print_matrix(c22);

	delete[] temp1.array;
	delete[] temp2.array;
	delete[] p1.array;
	delete[] p2.array;
	delete[] p3.array;
	delete[] p4.array;
	delete[] p5.array;
	delete[] p6.array;
	delete[] p7.array;
}

int main()
{
	const int size = 4;

	int* m1 = generate_random_matrix(size, -5, 5);
	sq_matrix a(m1, size);
	cout << "Matrix 1:\n";
	print_matrix(m1, size, size);

	int* m2 = generate_random_matrix(size, -5, 5);
	sq_matrix b(m2, size);
	cout << "Matrix 2:\n";
	print_matrix(m2, size, size);

	int* result = multiply_standard(m1, m2, size);
	cout << "Standard multiplication:\n";
	print_matrix(result, size, size);

	cout << "Strassen multiplication:\n";
	sq_matrix strassen_result = new_sq_matrix_zero(size);
	multiply_strassen(a, b, strassen_result);
	print_matrix(strassen_result);

	delete[] m1;
	delete[] m2;
	delete[] result;
	delete[] strassen_result.array;

	cin.get();

	return 0;
}
