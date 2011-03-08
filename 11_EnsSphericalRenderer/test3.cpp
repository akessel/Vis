
#include <fstream>
#include <string>
#include <stdio.h>
#include "MyFileReader.h"

int main(int argc, char *argv[])
{
	MyFileReader<float> reader;
	
	reader.SetFilename("../ens0.raw");	
	int num = 33*32*64;
	
//	reader.SetFilename("../../ens.raw");	
//	int num = 258*256*512;
	/*
	
	for(int i = 0; i < num-10; i++)
	{
		if(i % 1000000 == 0)
		{
			printf("throwaway loop %i...\n", i);
		}
		reader.GetNextValue();
	}
	*/
	
	float min = 1000000.0;
	float max = -1000000.0;
	
	for(int i = 0; i < num; i++)
	{
//		printf("---------");
//		printf("Loop %i\n", i);
	
		float val = reader.GetNextValue();
//		printf("Val = %e\n", val);		
		
		if(val < min)
		{
//			printf("Min: Replacing %e with %e\n", min, val);
			min = val;
		}
		if(val > max)
		{
//			printf("Max: Replacing %e with %e\n", max, val);
			max = val;
		}
		
		if(i % 1000000 == 0)
		{
			printf("loop %i...\n", i);
		}
		
	}

	printf("values from ens.raw\n");
	printf("min = %e\n", min);	
	printf("max = %e\n", max);
}
