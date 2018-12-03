#include <iostream>
#include <cstdio>
#include <cctype>

using namespace std;

/** inputFile: binary read stream
  * Return value: true on success, false on failure
  * Handles errors but does not close streams!
  */

bool readBinaryFile(FILE* file)
{
	if(!file)
	{
		perror("Failed to open binary file for reading");
		return false;
	}

	int num;
	while(fread(&num, sizeof(int), 1, file))
	{
//		cout << "pos: " << ftell(file) << endl;
        printf("%d\n", num);
	}

	if(ferror(file))
	{
		perror("Error occurred while reading output.bin");
		return false;
	}

	return true;
}

/** inputFile: read stream
  * outputFile: binary write stream
  * Return value: true on success, false on failure
  * Handles errors but does not close streams!
  */

bool processFile(FILE* inputFile, FILE* outputFile)
{
	if(!inputFile)
	{
		perror("Failed to open input file for reading");
		return false;
	}
	if(!outputFile)
	{
		perror("Failed to open output file for writing");
		return false;
	}

	int num;
	while(fscanf(inputFile, "%d", &num) != EOF)
	{
		//check for trailing chars and ignore if invalid int
		char c = fgetc(inputFile);

		if(c == EOF)
		{
			break;
		}
		else
		{
			ungetc(c, inputFile);
		}

        if(!isdigit(c) && !isspace(c))
		{
			fscanf(inputFile, "%*s"); //skip string
			puts("Encountered non-parseable string: ignoring...");
			continue;
		}

		//valid int: print
		printf("%d\n", num);
		int result = fwrite(&num, sizeof(int), 1, outputFile);
		if(!result)
		{
			break;
		}
	}

	if(ferror(inputFile))
	{
		perror("Error occurred while reading input.txt");
		return false;
	}

	if(ferror(outputFile))
	{
		perror("Error occurred while writing output.txt");
		return false;
	}

	return true;
}

int main()
{
	FILE* inputFile;
	inputFile = fopen("input.txt", "r");

	FILE* outputFile;
	outputFile = fopen("output.bin", "wb");

	puts("Reading text file");
	bool result = processFile(inputFile, outputFile);

	if(outputFile) fclose(outputFile);
	if(inputFile) fclose(inputFile);

	if(!result)
	{
		return EXIT_FAILURE;
	}
	else
	{
		puts("\nChecking binary file");

		outputFile = fopen("output.bin", "rb");

		result = readBinaryFile(outputFile);

		if(outputFile) fclose(outputFile);
		if(!result)
		{
			return EXIT_FAILURE;
		}
		else
		{
			return EXIT_SUCCESS;
		}
	}
}
