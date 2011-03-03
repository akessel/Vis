#ifndef _MYFILEREADER_H_
#define _MYFILEREADER_H_

#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

template<class T>
class MyFileReader
{
	private:
		string filename;
		
		ifstream stream;
	
	public:
		MyFileReader()
		{			
		}
		
		void SetFilename(string filename)
		{
			this->filename = filename;
		}
		
		T GetNextValue()
		{
			printf("Getting next value\n");
			this->EnsureFileIsOpen();
			
			int size = sizeof(T);
			char buffer [size];
			
			printf("Reading value\n");
			this->stream.read(buffer, size);
//			long newPointer = this->stream.tellg() + (long)size;
//			this->stream.seekg(newPointer);

			printf("Temping value\n");
			char temp0 = buffer[0];
			char temp1 = buffer[1];
			char temp2 = buffer[2];
			char temp3 = buffer[3];
			printf("Reversing value\n");
			buffer[0] = ReverseBits(temp3);
			printf("Reversing done (1), next\n");
			buffer[1] = ReverseBits(temp2);
			printf("Reversing done (2), next\n");
			buffer[2] = ReverseBits(temp1);
			printf("Reversing done (3), next\n");
			buffer[3] = ReverseBits(temp0);
			printf("Reversing done (4), next\n");
			
			T result = (T)(*buffer);
			
			printf("result = %f\n", result);
			
			return result;
		}
		
		~MyFileReader()
		{
			if(this->stream.is_open())
			{
				this->stream.close();
			}
		}
		
	private:
		void EnsureFileIsOpen()
		{
			if(!this->stream.is_open())
			{
				this->stream.open(this->filename.c_str(), ios::in | ios::binary);
			}
		}
		
		char ReverseBits(char v)
		{
		//	unsigned int v;     // input bits to be reversed
			char r = v; // r will be reversed bits of v; first get LSB of v
			int s = sizeof(v) * 8 - 1; // extra shift needed at end

			for (v >>= 1; v; v >>= 1)
			{   
			  r <<= 1;
			  r |= v & 1;
			  s--;
			}
			r <<= s; // shift when v's highest bits are zero
	
			return r;
		}
};

#endif
