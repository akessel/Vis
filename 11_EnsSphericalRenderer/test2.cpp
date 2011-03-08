#include <stdio.h>
#include <fstream>
#include <string>

#define FILEPATH "../SampleData/RAW/MyData.raw"

using namespace std;

int main(int argc, char *argv[])
{
	ofstream stream (FILEPATH, ios::out | ios::binary);
	
	float val = 1.175494e-38;
	
	char *ptr = (char*)(&val);
	
	int x = 16;
	int y = 16;
	int z = 16;
	
	int size = sizeof(float);
	
	for(int i = 0; i < x; i++)
	{
		for(int j = 0; j < y; j++)
		{
			for(int k = 0; k < z; k++)
			{
				stream.write(ptr, size);
			}
		}
	}
	
	stream.close();
}
