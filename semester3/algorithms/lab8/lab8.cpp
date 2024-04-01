#include <iostream>
#include <algorithm>
#include <time.h>

using namespace std;


/**/
/* NAIVE */
/**/

bool naive_search(const char* pattern, size_t pattern_size, const char* text, size_t text_size)
{
	for (size_t i = 0; i < text_size - pattern_size + 1; i++)
	{
		bool success = true;
		for (size_t j = 0; j < pattern_size; j++)
		{
			if (pattern[j] != text[i + j])
			{
				success = false;
				break;
			}
		}
		if (success)
			return true;
	}
	return false;
}




/**/
/* RABIN-KARP */
/**/

//can be any number below 256, but ideally it should be prime
const int mod = 101;

//variant of the Rabin fingerprint
//the result is 256^n * c[0] + 256^(n-1) * c[1] + 256^(n-2) * c[2] + ... + 256c[n-1] + c[n] (mod 101)
//(where n = length - 1)
int rabin_hash(const char* c, int length)
{
	int hash = c[0];
	for (size_t i = 1; i < length; i++)
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

bool rabin_karp_search(const char* pattern, size_t pattern_length, const char* text, size_t text_length)
{
	int pow256 = 1;
	for (int i = 0; i < pattern_length - 1; i++)
		pow256 = pow256 * 256 % mod;

	//compute pattern hash
	int pattern_hash = rabin_hash(pattern, pattern_length);

	int current_hash;

	//int hash_collisions = 0;
	for (size_t i = 0; i < text_length - pattern_length + 1; i++)
	{
		if (i == 0)
		{
			//compute initial hashes
			current_hash = rabin_hash(text, text_length);
		}
		else
		{
			//get next hash
			current_hash = new_rabin_hash(text[i - 1], current_hash, text[i + pattern_length - 1], pow256);
		}


		if (current_hash == pattern_hash)
		{
			//hash_collisions++;
			//if hashes match, scan entire substring
			bool success = true;
			for (size_t j = 0; j < pattern_length; j++)
			{
				if (text[i + j] != pattern[j])
				{
					success = false;
					break;
				}
			}
			if (success)
				return true;
		}
	}
	//cout << hash_collisions << endl;

	return false;
}


/**/
/* Knuth–Morris–Pratt algorithm */
/**/


// Fills lps[] for given patttern pat[0..M-1] 
void compute_LPS_array(const char* pattern, int pattern_length, int* lps)
{
	// length of the previous longest prefix suffix 
	int sequence_length = 0;

	lps[0] = 0; // lps[0] is always 0 

	// the loop calculates lps[i] for i = 1 to M-1 
	int i = 1;
	while (i < pattern_length) {
		if (pattern[i] == pattern[sequence_length]) {
			sequence_length++;
			lps[i] = sequence_length;
			i++;
		}
		else
		{
			// This is tricky. Consider the example. 
			// AAACAAAB and i = 7.
			if (sequence_length != 0) {
				sequence_length = lps[sequence_length - 1];

				// Also, note that we do not increment 
				// i here 
			}
			else // if (len == 0) 
			{
				lps[i] = 0;
				i++;
			}
		}
	}
}

bool KMP_search(const char* pattern, size_t pattern_length, const char* text, size_t text_length)
{
	// create lps[] that will hold the longest prefix suffix 
	// values for pattern 
	int* lps = new int[pattern_length];

	// Preprocess the pattern (calculate lps[] array) 
	compute_LPS_array(pattern, pattern_length, lps);

	//int steps = 0;
	int i = 0; // index for text
	int j = 0; // index for pattern 
	while (i < text_length) {
		//steps++;
		//cout << "j = " << j << " i = " << i << "...\n";
		//cout << "Comparing " << pattern[j] << " and " << text[i] << "...\n";
		if (pattern[j] == text[i]) {
			j++;
			i++;
		}

		if (j == pattern_length) {
			//cout << "Found pattern at index " << (i - j) << endl;
			//j = lps[j - 1];
			delete[] lps;
			return true;
		}

		// mismatch after j matches 
		else if (i < text_length && pattern[j] != text[i]) {
			// Do not match lps[0..lps[j-1]] characters, 
			// they will match anyway 
			if (j != 0)
				j = lps[j - 1];
			else
				i = i + 1;
		}
	}
	delete[] lps;
	return false;
}


/**/
/* BOYER-MOORE */
/**/

/**
 * Is text[p:end] a prefix of text?
 */
bool is_prefix(const char* pattern, size_t pattern_length, int start)
{
	for (int i = start, j = 0; i < pattern_length; i++, j++)
	{
		if (pattern[i] != pattern[j])
		{
			return false;
		}
	}
	return true;
}

/**
 * Returns the maximum length of the substring which ends at p and is a suffix.
 */
int suffix_length(const char* pattern, size_t pattern_length, int p)
{
	int len = 0;
	for (int i = p, j = pattern_length - 1;
		i >= 0 && pattern[i] == pattern[j]; i--, j--)
	{
		len += 1;
	}
	return len;
}

/**
 * Makes the jump table based on the mismatched character information.
 */
int* make_char_table(const char* pattern, size_t pattern_length)
{
	const int ALPHABET_SIZE = 256; //max value of char
	int* table = new int[ALPHABET_SIZE];
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		table[i] = pattern_length;
	}
	for (int i = 0; i < pattern_length - 1; i++)
	{
		table[pattern[i]] = pattern_length - 1 - i;
	}
	return table;
}

/**
 * Makes the jump table based on the scan offset which mismatch occurs.
 */
int* make_offset_table(const char* pattern, size_t pattern_length)
{
	int* table = new int[pattern_length];
	int lastPrefixPosition = pattern_length;
	for (int i = pattern_length; i > 0; i--)
	{
		if (is_prefix(pattern, pattern_length, i))
		{
			lastPrefixPosition = i;
		}
		table[pattern_length - i] = lastPrefixPosition - i + pattern_length;
	}
	for (int i = 0; i < pattern_length - 1; ++i)
	{
		int slen = suffix_length(pattern, pattern_length, i);
		table[slen] = pattern_length - 1 - i + slen;
	}
	return table;
}

int boyer_moore_search(const char* pattern, size_t pattern_length, const char* text, size_t text_length) {
	if (pattern_length == 0) {
		return 0;
	}
	int* charTable = make_char_table(pattern, pattern_length);
	int* offsetTable = make_offset_table(pattern, pattern_length);
	for (int i = pattern_length - 1, j = 0; i < text_length;)
	{
		for (j = pattern_length - 1; pattern[j] == text[i]; i--, j--)
		{
			if (j == 0)
			{
				delete[] charTable;
				delete[] offsetTable;
				return i;
			}
		}
		// i += needle.length - j; // For naive method
		i += max(offsetTable[pattern_length - 1 - j], charTable[text[i]]);
	}
	delete[] charTable;
	delete[] offsetTable;
	return -1;
}



/**/
/* BOYER-MOORE-HORSPOOL */
/**/


int horspool_search(const char* pattern, size_t pattern_length, const char* text, size_t text_length)
{
	int* skip_table = make_char_table(pattern, pattern_length);
	int scan_start = 0;
	while (text_length - scan_start >= pattern_length)
	{
		int i = pattern_length - 1;
		while (text[scan_start + i] == pattern[i])
		{
			if (i == 0)
			{
				delete[] skip_table;
				return scan_start;
			}
			i++;
		}
		scan_start = scan_start + skip_table[text[scan_start + pattern_length - 1]];

	}
	delete[] skip_table;
	return -1;
}



int main()
{
	string example_text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum. Curabitur pretium tincidunt lacus.Nulla gravida orci a odio.Nullam varius, turpis et commodo pharetra, est eros bibendum elit, nec luctus magna felis sollicitudin mauris.Integer in mauris eu nibh euismod gravida.Duis ac tellus et risus vulputate vehicula.Donec lobortis risus a elit.Etiam tempor.Ut ullamcorper, ligula eu tempor congue, eros est euismod turpis, id tincidunt sapien risus a quam.Maecenas fermentum consequat mi.Donec fermentum.Pellentesque malesuada nulla a mi.Duis sapien sem, aliquet nec, commodo eget, consequat quis, neque.Aliquam faucibus, elit ut dictum aliquet, felis nisl adipiscing sapien, sed malesuada diam lacus eget erat.Cras mollis scelerisque nunc.Nullam arcu.Aliquam consequat.Curabitur augue lorem, dapibus quis, laoreet et, pretium ac, nisi.Aenean magna nisl, mollis quis, molestie eu, feugiat in, orci.In hac habitasse platea dictumst.";
	string random_text;
	for (int i = 0; i < 1000; i++)
		random_text.push_back(rand() % 2 ? 'a' : 'b');

	cout << "======== Human text search: ========\n";

	clock_t start = clock();
	for(int i = 0; i < 100000; i++)
		naive_search("Donec fermentum", 15, example_text.c_str(), example_text.length());
	clock_t end = clock();
	cout << "Naive: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		boyer_moore_search("Donec fermentum", 15, example_text.c_str(), example_text.length());
	end = clock();
	cout << "Boyer-Moore: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		KMP_search("Donec fermentum", 15, example_text.c_str(), example_text.length());
	end = clock();
	cout << "KMP: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		horspool_search("Donec fermentum", 15, example_text.c_str(), example_text.length());
	end = clock();
	cout << "Horspool: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		rabin_karp_search("Donec fermentum", 15, example_text.c_str(), example_text.length());
	end = clock();
	cout << "Rabin-Karp: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";


	cout << "======== Binary search: ========\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		naive_search("abaaababbbaabab", 15, random_text.c_str(), random_text.length());
	end = clock();
	cout << "Naive: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		boyer_moore_search("abaaababbbaabab", 15, random_text.c_str(), random_text.length());
	end = clock();
	cout << "Boyer-Moore: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		KMP_search("abaaababbbaabab", 15, random_text.c_str(), random_text.length());
	end = clock();
	cout << "KMP: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		horspool_search("abaaababbbaabab", 15, random_text.c_str(), random_text.length());
	end = clock();
	cout << "Horspool: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";

	start = clock();
	for (int i = 0; i < 100000; i++)
		rabin_karp_search("abaaababbbaabab", 15, random_text.c_str(), random_text.length());
	end = clock();
	cout << "Rabin-Karp: " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << " ms\n";
}