#include "vtkStructuredGridWriter.h"
#include "vtkStructuredGrid.h"
#include "MyStructuredGridReader.h"

#define FILEPATH "../SampleData/RAW/ens0.raw"

int main(int, char* [])
{
	MyStructuredGridReader reader("../SampleData/RAW/ens.raw",258,256,512,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);


	vtkStructuredGrid *grid = reader.GetOutput();
	
	vtkStructuredGridWriter *writer = vtkStructuredGridWriter::New();
	writer->SetInput(grid);
	writer->SetFileName("ens_Binary.vtk");
	writer->SetFileTypeToBinary();
	writer->Write();	
}

