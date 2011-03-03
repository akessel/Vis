#include <stdio.h>
#include "vtkImageData.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkCellType.h"
#include "vtkCellArray.h"
#include "vtkHexahedron.h"
#include "MyFilter.h"

MyFilter::MyFilter(int nX, int nY, int nZ, int phi, int theta, int rho)
{
	this->nX = nX;
	this->nY = nY;
	this->nZ = nZ;
	this->phi = phi;
	this->theta = theta;
	this->rho = rho;
}

MyFilter::~MyFilter()
{
//	delete this->data;
}

void MyFilter::SetInput(vtkImageData *data)
{
	this->data = data;
}
		
vtkUnstructuredGrid * MyFilter::GetOutput()
{
	cout << "Starting to get output..." << endl;
	
	vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
	
	int numPoints = this->nX * this->nY * this->nZ;
	
	vtkPoints *points = vtkPoints::New();
	vtkFloatArray *scalars = vtkFloatArray::New();
	vtkCellArray *cells = vtkCellArray::New();
	
	// iterate through the points, transforming them to spherical
	int count = 0;
	for(int i = 0; i < this->nX; i++)
	{		
		printf("i = %i\n", i);
		for(int j = 0; j < this->nY; j++)
		{
//			cout << " j=" << j;
			for(int k = 0; k < this->nZ; k++)
			{
//				cout << " k=" << k << endl;
				float *value = (float *)(this->data->GetScalarPointer(i,j,k));
				
				double coord[3];
				this->data->GetPoint(count, coord);
			
				double di = (double)i;
				double dj = (double)j;
				double dk = (double)k;
/*			
				double phi = arctan(dj/di);
				double rho = sqrt(di*di + dj*dj + dk*dk);
				double theta = arccos(dk / rho);
			
				points->InsertPoint(count, phi, theta, rho);
*/
				points->InsertPoint(count, coord[0], coord[1], coord[2]);									
				scalars->InsertValue(*value, count);
								
				if(i > 0 && j > 0 && k > 0)
				{
					if( ( (i % 10 == 0) && (j % 10 == 0) && (k % 10 == 0) )
						|| ( (i == ((this->nX)-1)) && (j == ((this->nY)-1)) && (k == ((this->nZ)-1)) ) )
					{
						printf("Creating cell { %i, %i, %i }...\n", i, j, k);
					}
					
					vtkHexahedron *cell = vtkHexahedron::New();
					
					cell->GetPointIds()->SetNumberOfIds(8);					
					
					cell->GetPointIds()->SetId(0, i*this->nX + j*this->nY + k);
					cell->GetPointIds()->SetId(1, i*this->nX + j*this->nY + (k-1));
					cell->GetPointIds()->SetId(2, i*this->nX + (j-1)*this->nY + k);
					cell->GetPointIds()->SetId(3, i*this->nX + (j-1)*this->nY + (k-1));
					
					cell->GetPointIds()->SetId(4, (i-1)*this->nX + j*this->nY + k);
					cell->GetPointIds()->SetId(5, (i-1)*this->nX + j*this->nY + (k-1));
					cell->GetPointIds()->SetId(6, (i-1)*this->nX + (j-1)*this->nY + k);
					cell->GetPointIds()->SetId(7, (i-1)*this->nX + (j-1)*this->nY + (k-1));
					
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
	grid->SetCells(12, cells); // 12 = VTK_HEXAHEDRON

	cout << "Ending get output..." << endl;
	return grid;
}
