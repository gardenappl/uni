#include <iostream>
#include <list>

using namespace std;


int main()
{
	const int numArray[] { 8, 4, 2, 5, 10, 6, 7, 9 };
	const int numArrayLength = 8;

	/* This algorithm iterates from end to start and finds the largest sub-sequence for each element
	 * Example: for an array { 8, 4, 3, 5, 10, 6, 7, 9 }
	 * 9 -> { 9 }
	 * 7 -> { 7, 9 } (we can append 9's sub-sequence because 9 is larger than 7)
	 * 6 -> { 6, 7, 9 } (we can append 7's or 9's subsequences but we choose 7's as it's the longest)
	 * 10 -> { 10 }
	 * 5 -> { 5, 6, 7, 9 }
	 * ...
	 * The longest sequence will be { 3, 5, 6, 7, 9 } */


	int* sequences[numArrayLength];
	int sequenceLengths[numArrayLength];
	for(int i = 0; i < numArrayLength; i++)
	{
		sequences[i] = new int { numArray[i] };
		sequenceLengths[i] = 1;
	}

	int longestSequenceIndex = -1;

	//Create sequences starting from each element
	for(int i = numArrayLength - 1; i >= 0; i--)
	{
		int* currentLongestSequence = sequences[i];
		int currentLongestSequenceLength = 1;

		//Try to append sequences from later numbers
		for(int i2 = i + 1; i2 < numArrayLength; i2++)
		{
			if(numArray[i] < numArray[i2] && currentLongestSequenceLength < 1 + sequenceLengths[i2])
			{
//                cout << "new longest sequence for " << numArray[i] << endl;
				delete currentLongestSequence;
				currentLongestSequenceLength = 1 + sequenceLengths[i2];
                currentLongestSequence = new int[currentLongestSequenceLength];
//                cout << "length: " << currentLongestSequenceLength << endl;

                //Append existing sequence to this
                currentLongestSequence[0] = numArray[i];
                copy(sequences[i2], sequences[i2] + sequenceLengths[i2], currentLongestSequence + 1);

//                for(int i = 0; i < currentLongestSequenceLength; i++)
//				{
//					cout << currentLongestSequence[i] << " ";
//				}
//				cout << endl;
			}
		}

		sequences[i] = currentLongestSequence;
		sequenceLengths[i] = currentLongestSequenceLength;
		if(longestSequenceIndex == -1 || sequenceLengths[longestSequenceIndex] < currentLongestSequenceLength)
		{
			longestSequenceIndex = i;
		}
	}

	for(int i = 0; i < sequenceLengths[longestSequenceIndex]; i++)
	{
		cout << sequences[longestSequenceIndex][i] << " ";
	}

    return 0;
}
