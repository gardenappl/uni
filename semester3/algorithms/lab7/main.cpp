#include <iostream>
#include <cmath>
#include <string>

using namespace std;

//can be any number below 256, but ideally it should be prime
const int mod = 101;

void print_array(int* arr, size_t length)
{
    cout << "{ ";
    if(length > 0)
    {
        cout << arr[0];
        for(size_t i = 1; i < length; i++)
            cout << ", " << arr[i];
    }
    cout << " }" << endl;
}

//variant of the Rabin fingerprint
//the result is 256^n * c[0] + 256^(n-1) * c[1] + 256^(n-2) * c[2] + ... + 256c[n-1] + c[n] (mod 101)
//(where n = length - 1)
int rabin_hash(const char* c, int length)
{
    int hash = c[0];
    for(size_t i = 1; i < length; i++)
    {
        hash = (hash * 256) % mod;
        hash = (hash + c[i]) % mod;
    }
    return hash;
}

//to get the new hash:
// subtract old_c * 256^n
// multiply by 256
// add new c
int new_rabin_hash(char old_c, int old_hash, char new_c, int pow256)
{
    old_hash = (old_hash + mod - old_c * pow256 % mod) * 256 % mod + new_c;
    return old_hash % mod;
}

bool rabin_karp_2d_search(char** matrix, size_t matrix_size, char** pattern, size_t pattern_size)
{
    int pow256 = 1;
    for(int i = 0; i < pattern_size - 1; i++)
        pow256 = pow256 * 256 % mod;

    //compute pattern hashes
    int* pattern_hashes = new int[pattern_size];
    for(size_t i = 0; i < pattern_size; i++)
        pattern_hashes[i] = rabin_hash(pattern[i], pattern_size);

    cout << "Pattern hashes: ";
    print_array(pattern_hashes, pattern_size);

    int* hashes = new int[pattern_size];

    for(size_t y = 0; y < matrix_size - pattern_size + 1; y++)
    {
        //compute initial hashes
        for(size_t i = 0; i < pattern_size; i++)
            hashes[i] = rabin_hash(matrix[y + i], pattern_size);

        for(size_t x = 0; x < matrix_size - pattern_size + 1; x++)
        {
            if(x != 0)
            {
                //shift hashes right
                for(size_t i = 0; i < pattern_size; i++)
                    hashes[i] = new_rabin_hash(matrix[y + i][x - 1],
                            hashes[i], matrix[y + i][x + pattern_size - 1], pow256);
            }
            cout << "x = " << x << "; y = " << y << ";\n";
            cout << "Hashes: ";
            print_array(hashes, pattern_size);

            //Check if hashes match
            bool success = true;
            for(size_t i = 0; i < pattern_size; i++)
            {
                if(hashes[i] != pattern_hashes[i])
                {
                    success = false;
                    break;
                }
            }

            check_pattern:
            if(success)
            {
                //Hashes matched: scan the entire pattern
                for(size_t y1 = 0; y1 < pattern_size; y1++)
                {
                    for(size_t x1 = 0; x1 < pattern_size; x1++)
                    {
                        if(matrix[y + y1][x + x1] != pattern[y1][x1])
                        {
                            success = false;
                            goto check_pattern;
                        }
                    }
                }
                delete[] pattern_hashes;
                delete[] hashes;
                return true;
            }
        }
    }

    delete[] pattern_hashes;
    delete[] hashes;
    return false;
}




char** enter_char_matrix(size_t matrix_size)
{
    char** matrix = new char*[matrix_size];
    cout << "Creating " << matrix_size << " x " << matrix_size << " char matrix...\n";

    for(size_t i = 0; i < matrix_size; i++)
    {
        string input_str;
        while(true)
        {
            cout << "Enter row " << i << ": ";
            cin >> input_str;
            if (input_str.length() == matrix_size)
                break;
            else
                cout << "Row must be " << matrix_size << " characters long.\n";
        }
        matrix[i] = new char[matrix_size + 1];
        copy(input_str.begin(), input_str.end(), matrix[i]);
		matrix[i][matrix_size] = '\0';
    }

    cout << '\n';
    for(size_t i = 0; i < matrix_size; i++)
        cout << matrix[i] << '\n';

    return matrix;
}

int main()
{
    cout << "Enter matrix size: ";
    size_t matrix_size;
    cin >> matrix_size;
    char** matrix = enter_char_matrix(matrix_size);

    size_t pattern_size;
    while(true)
    {
        cout << "Enter pattern size: ";
        cin >> pattern_size;
        if(pattern_size >= matrix_size)
            cout << "Pattern size should be smaller than matrix size";
        else
            break;
    }
    char** pattern = enter_char_matrix(pattern_size);

    cout << "Matrix contains pattern: " <<
            boolalpha << rabin_karp_2d_search(matrix, matrix_size, pattern, pattern_size);

    return 0;
}