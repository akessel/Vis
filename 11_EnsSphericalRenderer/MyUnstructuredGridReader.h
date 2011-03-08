#ifndef _MYUNSTRUCTUREDGRIDREADER_H_
#define _MYUNSTRUCTUREDGRIDREADER_H_

#include <stdio.h>
#include <string>
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkCellType.h"
#include "vtkCellArray.h"
#include "vtkTetra.h"
#include "MyFileReader.h"

using namespace std;

class MyUnstructuredGridReader
{
	private:
		int nX, nY, nZ;
		
		int phi, theta, rho; // long, lat, radius
		
		MyFileReader<float> reader;

	public:
		MyUnstructuredGridReader(string filename, int nX, int nY, int nZ, int phi, int theta, int rho)
		{
			this->reader.SetFilename(filename);
			this->nX = nX;
			this->nY = nY;
			this->nZ = nZ;
			this->phi = phi;
			this->theta = theta;
			this->rho = rho;
		}
		
		vtkUnstructuredGrid * GetOutput()
		{
			printf("Starting to get output...\n");
	
			vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
	
			int numPoints = this->nX * this->nY * this->nZ;
	
			vtkPoints *points = vtkPoints::New();
			vtkFloatArray *scalars = vtkFloatArray::New();
			vtkCellArray *cells = vtkCellArray::New();

			float max = -1000000.0;
			float min = 1000000.0;
	
			// iterate through the points, transforming them to spherical
			int count = 0;
			for(int i = 0; i < this->nX; i++)
			{		
				for(int j = 0; j < this->nY; j++)
				{
					for(int k = 0; k < this->nZ; k++)
					{
						
						float value = this->reader.GetNextValue();
						
						if(value > max) max = value;
						else if(value < min) min = value;						
				
						double di = ((double)i)/this->nX;
						double dj = ((double)j)/this->nY;
						double dk = ((double)k)/this->nZ;
						
						points->InsertPoint(count, di, dj, dk);									
						scalars->InsertValue(count, value);
				
						count++;
								
						if(i > 0 && j > 0 && k > 0)
						{
							// find the indices of the points defining the vertices of the cube
							int FLT = this->LinearizeCoordinate(i-1,j-1,k-1);	// index of Front	Left	Top
							int FLB = this->LinearizeCoordinate(i-1,j,  k-1);	// index of Front	Left	Bottom
							int FRT = this->LinearizeCoordinate(i-1,j-1,k);	// index of Front	Right	Top
							int FRB = this->LinearizeCoordinate(i-1,j,  k);		// index of Front	Right	Bottom
							int BLT = this->LinearizeCoordinate(i,	j-1,k-1);	// index of Back	Left	Top
							int BLB = this->LinearizeCoordinate(i,	j,  k-1);		// index of Back	Left	Bottom
							int BRT = this->LinearizeCoordinate(i,	j-1,k);		// index of Back	Right	Top
							int BRB = this->LinearizeCoordinate(i,	j,  k);		// index of Back	Right	Bottom
					
							// each cube is composed of five tetrahedrons
							vtkTetra *cell1 = vtkTetra::New();
							vtkTetra *cell2 = vtkTetra::New();
							vtkTetra *cell3 = vtkTetra::New();
							vtkTetra *cell4 = vtkTetra::New();
							vtkTetra *cell5 = vtkTetra::New();
					
							cell1->GetPointIds()->SetNumberOfIds(4);
							cell2->GetPointIds()->SetNumberOfIds(4);
							cell3->GetPointIds()->SetNumberOfIds(4);
							cell4->GetPointIds()->SetNumberOfIds(4);
							cell5->GetPointIds()->SetNumberOfIds(4);
							
							cell1->GetPointIds()->SetId(0, FLT);
							cell1->GetPointIds()->SetId(1, FLB);
							cell1->GetPointIds()->SetId(2, FRB);
							cell1->GetPointIds()->SetId(3, BLB);
							
							cell2->GetPointIds()->SetId(0, FLT);
							cell2->GetPointIds()->SetId(1, FRT);
							cell2->GetPointIds()->SetId(2, FRB);
							cell2->GetPointIds()->SetId(3, BRT);
							
							cell3->GetPointIds()->SetId(0, FRB);
							cell3->GetPointIds()->SetId(1, BRB);
							cell3->GetPointIds()->SetId(2, BRT);
							cell3->GetPointIds()->SetId(3, BLB);
							
							cell4->GetPointIds()->SetId(0, FLT);
							cell4->GetPointIds()->SetId(1, BLT);
							cell4->GetPointIds()->SetId(2, BRT);
							cell4->GetPointIds()->SetId(3, BLB);
							
							cell5->GetPointIds()->SetId(0, FLT);
							cell5->GetPointIds()->SetId(1, BRT);
							cell5->GetPointIds()->SetId(2, BLB);
							cell5->GetPointIds()->SetId(3, FRB);

							cells->InsertNextCell(cell1);
							cells->InsertNextCell(cell2);
							cells->InsertNextCell(cell3);
							cells->InsertNextCell(cell4);							
							cells->InsertNextCell(cell5);
						}
					}
				}
			}
			
			double coord[3];
			for(int i = 0; i < count; i += 10)
			{
				points->GetPoint(i, coord);
				printf("Point %i: (%f, %f, %f)\n", i, coord[0], coord[1], coord[2]);
			}
	
			printf("Inserting points...\n");
			grid->SetPoints(points);
			printf("Inserting scalars...\n");
			grid->GetPointData()->SetScalars(scalars);
			printf("Inserting cells...\n");
			grid->SetCells(10, cells); // 10 = VTK_TETRA

			printf("Ending get output...\n");
			
			printf("Min = %e, Max = %e\n", min, max);
			
			return grid;		
		}
		
		~MyUnstructuredGridReader()
		{
		}
		
	private:		
		int LinearizeCoordinate(int i, int j, int k)
		{
			return i*(this->nX) + j*(this->nY) + k;
		}
};



#endif
