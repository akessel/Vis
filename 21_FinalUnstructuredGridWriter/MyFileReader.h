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
			this->EnsureFileIsOpen();
			
			int size = sizeof(T);
			char buffer [size];
			
			this->stream.read(buffer, size);
			
			T result = *((T*)buffer);
			
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
};

#endif
