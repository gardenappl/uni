#ifndef PARALLEL_TEST
#define PARALLEL_TEST

void CopyMatrix(int *pMatrix, int Size, int *pMatrixCopy);
bool CompareMatrices(int *pMatrix1, int *pMatrix2, int Size);
void SerialFloyd(int *pMatrix, int Size);
void PrintMatrix(int *pMatrix, int RowCount, int ColCount);

#endif
