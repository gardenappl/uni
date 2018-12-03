#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//без динамической памяти

//int main()
//{
//	FILE* fp;
//	FILE* fout;
//	char buf[200];
//
//    printf("Enter file name: ");
//	scanf("%s",buf);
//
//	fp=fopen(buf,"r"); //открыть файл для чтения
//	fout=fopen("out.txt","w"); //открыть файл для записи
//	int i=0;
//	while (!feof(fp))
//	{
//        //buf=fgets(buf,200,fp); //читать по 200 символов
//        fscanf(fp,"%s",&buf); //читать слово (до пробела)
//        fprintf(fout,"sring number %d: %s !!! \n",i,&buf);
//        i++;
//	}
//	fclose(fp);
//	fclose(fout);
//
//    return 0;
//}


//с динамической памятью
int main()
{
	FILE* fp;
	FILE* fout;
	char* buf = (char*)malloc(200 * sizeof(char));

    printf("Enter file name: ");
	scanf("%s",buf);

	fp=fopen(buf,"r");
	fout=fopen("out.txt","w");
	int i=0;
	while (!feof(fp))
	{
	//buf=fgets(buf,20,fp);
	fscanf(fp,"%s",buf);
	fprintf(fout,"sring number %d: %s !!! \n",i,buf);
	i++;
	}
	fclose(fp);
	fclose(fout);
	free(buf);

    return 0;
}
