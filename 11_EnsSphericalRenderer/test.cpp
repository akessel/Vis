#include <stdio.h>
#include "MyFileReader.h"

#define FILEPATH "../SampleData/RAW/ens.raw"


		
char ReverseBits(char in)
{
	int maskInt = 1;
	int *p = &maskInt;
	char *ptr = (char*)p;
	char mask = ptr[0];
	
	int msbMaskInt = 128;
	int *p2 = &msbMaskInt;
	char *ptr2 = (char*)p2;
	char MSB = ptr2[0];
	
//	printf("mask = %c\n", mask);
		
	for(int i = 0; i < 8; i++)
	{
		char LSB = in & mask;
		char temp = in;
		temp >> 1;

		if(LSB)
		{
			temp |= MSB;
		}

		in = temp;// | LSB;
	}
	return in;
}


int main(int argc, char *argv[])
{
	

/*
	unsigned int i = 2155905152;
	unsigned int r = ReverseBits(i);
	printf("i = %u, r = %u\n", i, r);
*/

	int i = 16843009;

	int *ip = &i;
	char *ptr = (char*)ip;
		
	printf("%c %c %c %c\n", ptr[0], ptr[1], ptr[2], ptr[3]);

	char c0 = ReverseBits(ptr[0]);
	char c1 = ReverseBits(ptr[1]);
	char c2 = ReverseBits(ptr[2]);	
	char c3 = ReverseBits(ptr[3]);
	printf("%c %c %c %c\n", c0, c1, c2, c3);

/*
	MyFileReader<float> reader;
	
	reader.SetFilename(FILEPATH);
	
	printf("%i\n", sizeof(float));
	
	for(int i = 0; i < 10; i++)
	{
		printf("%f\n", reader.GetNextValue());
	}
*/
}
