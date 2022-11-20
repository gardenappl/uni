#ifndef SERIAL
#define SERIAL

void ProcessInitialization(double *&pData, int &DataSize);
void ProcessTermination(double *pData);
void DummyDataInitialization(double *&pData, int &DataSize);
void RandomDataInitialization(double *&pData, int &DataSize);
void SerialBubble(double *pData, int DataSize);

#endif
