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
	
			// iterate through the points, transforming them to spherical
			int count = 0;
			for(int i = 0; i < this->nX; i++)
			{		
				printf("i = %i out of %i\n", i, this->nX-1);
				for(int j = 0; j < this->nY; j++)
				{
//					printf("i = %i, j = %i\n", i, j);
					for(int k = 0; k < this->nZ; k++)
					{
//						printf("i = %i, j = %i, k = %i\n", i, j, k);
						
						float value = this->reader.GetNextValue();
				
//						printf("value = %f\n", value);
				
						double di = ((double)i)/this->nX;
						double dj = ((double)j)/this->nY;
						double dk = ((double)k)/this->nZ;
						
//						printf("di = %g, dj = %g, dk = %g, count = %i\n", di, dj, dk, count);
		/*			
						double phi = arctan(dj/di);
						double rho = sqrt(di*di + dj*dj + dk*dk);
						double theta = arccos(dk / rho);
			
						points->InsertPoint(count, phi, theta, rho);
		*/
//						printf("InsertPoint...\n");
						points->InsertPoint(count, di, dj, dk);									
//						printf("InsertValue...\n");
						scalars->InsertValue(count, value);
								
//						printf("Start if statement...\n");
						if(i > 0 && j > 0 && k > 0)
						{
//							printf("Inside if statement...\n");
						//	if( ( (i % 10 == 0) && (j % 10 == 0) && (k % 10 == 0) )
						//		|| ( (i == ((this->nX)-1)) && (j == ((this->nY)-1)) && (k == ((this->nZ)-1)) ) )
						//	{
//								printf("Creating cell { %i, %i, %i }...\n", i, j, k);
						//	}
					
							vtkTetra *cell = vtkTetra::New();
					
							cell->GetPointIds()->SetNumberOfIds(4);					
					
							cell->GetPointIds()->SetId(0, i*this->nX + j*this->nY + k);
							cell->GetPointIds()->SetId(1, i*this->nX + j*this->nY + (k-1));
//							cell->GetPointIds()->SetId(2, i*this->nX + (j-1)*this->nY + k);
//							cell->GetPointIds()->SetId(3, i*this->nX + (j-1)*this->nY + (k-1));
					
							cell->GetPointIds()->SetId(4, (i-1)*this->nX + j*this->nY + k);
							cell->GetPointIds()->SetId(5, (i-1)*this->nX + j*this->nY + (k-1));
							//cell->GetPointIds()->SetId(6, (i-1)*this->nX + (j-1)*this->nY + k);
							//cell->GetPointIds()->SetId(7, (i-1)*this->nX + (j-1)*this->nY + (k-1));
					
							cells->InsertNextCell(cell);
						}
				
						count++;
					}
				}
			}
	
			printf("Inserting points...\n");
			grid->SetPoints(points);
			printf("Inserting scalars...\n");
			grid->GetPointData()->SetScalars(scalars);
			printf("Inserting cells...\n");
			grid->SetCells(10, cells); // 10 = VTK_TETRA

			printf("Ending get output...\n");
			return grid;		
		}
		
		~MyUnstructuredGridReader()
		{
		}
};



#endif
