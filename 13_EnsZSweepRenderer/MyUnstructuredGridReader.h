#ifndef _MYUNSTRUCTUREDGRIDREADER_H_
#define _MYUNSTRUCTUREDGRIDREADER_H_

#include <stdio.h>
#include <string>
#include <math.h>
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkCellType.h"
#include "vtkCellArray.h"
#include "vtkTetra.h"
#include "vtkVertex.h"
#include "MyFileReader.h"

#define PI 3.14159265

using namespace std;

class MyUnstructuredGridReader
{
	private:
		int nRad, nLat, nLong;
		
		double minRad, minLat, minLong;
		
		double maxRad, maxLat, maxLong;
		
		MyFileReader<float> reader;

	public:
		MyUnstructuredGridReader(string filename, int nRad, int nLat, int nLong,
								 double minRad, double minLat, double minLong, double maxRad, double maxLat, double maxLong)
		{
			this->reader.SetFilename(filename);
			this->nRad = nRad;
			this->nLat = nLat;
			this->nLong = nLong;
			this->minRad = minRad;
			this->minLat = minLat;
			this->minLong = minLong;
			this->maxRad = maxRad;
			this->maxLat = maxLat;
			this->maxLong = maxLong;
		}
		
		vtkUnstructuredGrid * GetOutput()
		{
			printf("Starting to get output...\n");
	
			vtkUnstructuredGrid *grid = vtkUnstructuredGrid::New();
	
			int numPoints = this->nRad * this->nLat * this->nLong;
	
			vtkPoints *points = vtkPoints::New();
			vtkFloatArray *scalars = vtkFloatArray::New();
			vtkCellArray *cells = vtkCellArray::New();
			
			points->SetNumberOfPoints(numPoints);
			scalars->SetNumberOfValues(numPoints);

			/*
			this->nLong = 2;
			this->nLat = 2;
			this->nRad = 2;
			*/
			// iterate through the points, transforming them to spherical
			int count = 0;
			for(int i = 0; i < this->nLong; i++)			
			{		
				for(int j = 0; j < this->nLat; j++)
				{
					for(int k = 0; k < this->nRad; k++)
					{
						
						float value = this->reader.GetNextValue();
						
						//if(value > max) max = value;
						//if(value < min) min = value;
						
						double rad = k * ( (this->maxRad - this->minRad) / this->nRad ) + this->minRad;
						double phi = j * ( (this->maxLat - this->minLat) / this->nLat ) + this->minLat;
						double theta = i * ( (this->maxLong - this->minLong) / this->nLong ) + this->minLong;
						
						double x = rad * sin(theta*PI/180.0) * cos(phi*PI/180.0);
						double y = rad * sin(phi*PI/180.0);
						double z = rad * cos(theta*PI/180.0) * cos(phi*PI/180.0);
						/*
						x = (double)k;
						y = (double)j;
						z = (double)i;
						*/
						points->InsertPoint(count, x, y, z);									
						scalars->InsertValue(count, value);
				
						count++;
						
						int FLT;	// index of Front	Left	Top
						int FLB;	// index of Front	Left	Bottom
						int FRT;	// index of Front	Right	Top
						int FRB;	// index of Front	Right	Bottom
						int BLT;	// index of Back	Left	Top
						int BLB;	// index of Back	Left	Bottom
						int BRT;	// index of Back	Right	Top
						int BRB;	// index of Back	Right	Bottom

						if(i > 0 && j > 0 && k > 0)
						{
							FLT = this->LinearizeCoordinate(i-1,j-1,k-1);
							FLB = this->LinearizeCoordinate(i-1,j,  k-1);
							FRT = this->LinearizeCoordinate(i-1,j-1,k);
							FRB = this->LinearizeCoordinate(i-1,j,  k);
							BLT = this->LinearizeCoordinate(i,	j-1,k-1);
							BLB = this->LinearizeCoordinate(i,	j,  k-1);
							BRT = this->LinearizeCoordinate(i,	j-1,k);
							BRB = this->LinearizeCoordinate(i,	j,  k);
							
							this->InsertTetrahedrons(FLT, FLB, FRT, FRB, BLT, BLB, BRT, BRB, cells);
						}/*						
						else if(i == 0 && j == 0)
						{
							// wrap around both longitude and latitude
							FLT = this->LinearizeCoordinate(this->nLong-1,this->nLat-1,k-1);
							FLB = this->LinearizeCoordinate(this->nLong-1,j,  k-1);
							FRT = this->LinearizeCoordinate(this->nLong-1,this->nLat-1,k);
							FRB = this->LinearizeCoordinate(this->nLong-1,j,  k);
							BLT = this->LinearizeCoordinate(i,	this->nLat-1,k-1);
							BLB = this->LinearizeCoordinate(i,	j,  k-1);
							BRT = this->LinearizeCoordinate(i,	this->nLat-1,k);
							BRB = this->LinearizeCoordinate(i,	j,  k);
						}*/
						else if(i == 0)
						{
							// wrap around longitude
							FLT = this->LinearizeCoordinate(this->nLong-1,j-1,k-1);
							FLB = this->LinearizeCoordinate(this->nLong-1,j,  k-1);
							FRT = this->LinearizeCoordinate(this->nLong-1,j-1,k);
							FRB = this->LinearizeCoordinate(this->nLong-1,j,  k);
							BLT = this->LinearizeCoordinate(i,	j-1,k-1);
							BLB = this->LinearizeCoordinate(i,	j,  k-1);
							BRT = this->LinearizeCoordinate(i,	j-1,k);
							BRB = this->LinearizeCoordinate(i,	j,  k);
							
							this->InsertTetrahedrons(FLT, FLB, FRT, FRB, BLT, BLB, BRT, BRB, cells);
						}						
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
			
			printf("Count = %i\n", count);
			//printf("Min = %e, Max = %e\n", min, max);
			
			return grid;		
		}
		
		~MyUnstructuredGridReader()
		{
		}
		
	private:		
		int LinearizeCoordinate(int i, int j, int k)
		{
			return i*this->nLat*this->nRad + j*this->nRad + k;
		}
		
		void InsertTetrahedrons(int FLT, int FLB, int FRT, int FRB, int BLT, int BLB, int BRT, int BRB, vtkCellArray *cells)
		{
			// each cube is composed of five tetrahedrons
			vtkTetra *frontLeftCell = vtkTetra::New();
			vtkTetra *frontRightCell = vtkTetra::New();
			vtkTetra *backLeftCell = vtkTetra::New();
			vtkTetra *backRightCell = vtkTetra::New();
			vtkTetra *centerCell = vtkTetra::New();
	
			frontLeftCell->GetPointIds()->SetNumberOfIds(4);
			frontRightCell->GetPointIds()->SetNumberOfIds(4);
			backLeftCell->GetPointIds()->SetNumberOfIds(4);
			backRightCell->GetPointIds()->SetNumberOfIds(4);
			centerCell->GetPointIds()->SetNumberOfIds(4);
			
			frontLeftCell->GetPointIds()->SetId(0, FLB);
			frontLeftCell->GetPointIds()->SetId(1, FRB);
			frontLeftCell->GetPointIds()->SetId(2, BLB);
			frontLeftCell->GetPointIds()->SetId(3, FLT);
			
			frontRightCell->GetPointIds()->SetId(0, FLT);
			frontRightCell->GetPointIds()->SetId(1, BRT);
			frontRightCell->GetPointIds()->SetId(2, FRT);
			frontRightCell->GetPointIds()->SetId(3, FRB);
			
			backRightCell->GetPointIds()->SetId(0, FRB);
			backRightCell->GetPointIds()->SetId(1, BRB);
			backRightCell->GetPointIds()->SetId(2, BLB);
			backRightCell->GetPointIds()->SetId(3, BRT);
			
			backLeftCell->GetPointIds()->SetId(0, FLT);
			backLeftCell->GetPointIds()->SetId(1, BLT);
			backLeftCell->GetPointIds()->SetId(2, BRT);
			backLeftCell->GetPointIds()->SetId(3, BLB);
			
			centerCell->GetPointIds()->SetId(0, FLT);
			centerCell->GetPointIds()->SetId(1, BRT);
			centerCell->GetPointIds()->SetId(2, FRB);
			centerCell->GetPointIds()->SetId(3, BLB);

			cells->InsertNextCell(frontLeftCell);
			cells->InsertNextCell(frontRightCell);
			cells->InsertNextCell(backLeftCell);
			cells->InsertNextCell(backRightCell);							
			cells->InsertNextCell(centerCell);
		}
};



#endif
