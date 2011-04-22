#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGrid.h"
#include "MyUnstructuredGridReader.h"

#define FILEPATH "../SampleData/RAW/ens0.raw"

int main(int, char* [])
{
	MyUnstructuredGridReader reader(FILEPATH,33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	vtkUnstructuredGrid *grid = reader.GetOutput();
	
	vtkUnstructuredGridWriter *writer = vtkUnstructuredGridWriter::New();
	writer->SetInput(grid);
	writer->SetFileName("ens0_Binary.vtk");
	writer->SetFileTypeToBinary();
	writer->Write();	
}

