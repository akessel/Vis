#include <stdio.h>
#include "MyFileReader.h"

#define FILEPATH "../SampleData/RAW/ens.raw"

int main(int argc, char *argv[])
{
	MyFileReader<float> reader;
	
	reader.SetFilename(FILEPATH);
	
	printf("size of float = %i\n", sizeof(float));
	
	for(int i = 0; i < 10; i++)
	{
		float val = reader.GetNextValue();
		printf("val = %e\n", val);
	}
}
