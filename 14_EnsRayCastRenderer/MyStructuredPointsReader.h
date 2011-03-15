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

class MyStructuredPointsReader
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

			float max = -1000000.0;
			float min = 1000000.0;
			
			int minIdx = 100000000;
			int maxIdx = -100000000;
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
						
						if(value > max) max = value;
						if(value < min) min = value;
						
						double rad = k * ( (this->maxRad - this->minRad) / this->nRad ) + this->minRad;
						double phi = j * ( (this->maxLat - this->minLat) / this->nLat ) + this->minLat;
						double theta = i * ( (this->maxLong - this->minLong) / this->nLong ) + this->minLong;
						
						double x = rad * cos(phi) * cos(theta);
						double y = rad * cos(phi) * sin(theta);
						double z = rad * sin(phi);
						/*
						double x = (double)k;
						double y = (double)j;
						double z = (double)i;
						*/
						points->InsertPoint(count, x, y, z);									
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
							
							if(FLT < minIdx) minIdx = FLT;
							if(FLB < minIdx) minIdx = FLB;
							if(FRT < minIdx) minIdx = FRT;
							if(FRB < minIdx) minIdx = FRB;
							if(BLT < minIdx) minIdx = BLT;
							if(BLB < minIdx) minIdx = BLB;
							if(BRT < minIdx) minIdx = BRT;
							if(BRB < minIdx) minIdx = BRB;
							
							if(FLT > maxIdx) maxIdx = FLT;
							if(FLB > maxIdx) maxIdx = FLB;
							if(FRT > maxIdx) maxIdx = FRT;
							if(FRB > maxIdx) maxIdx = FRB;
							if(BLT > maxIdx) maxIdx = BLT;
							if(BLB > maxIdx) maxIdx = BLB;
							if(BRT > maxIdx) maxIdx = BRT;
							if(BRB > maxIdx) maxIdx = BRB;
							/*
							printf("---------------\n");
							printf("Cell [%i %i %i]\n", i, j, k);
							printf("FLT = %i\n", FLT);
							printf("FLB = %i\n", FLB);
							printf("FRT = %i\n", FRT);
							printf("FRB = %i\n", FRB);
							printf("BLT = %i\n", BLT);
							printf("BLB = %i\n", BLB);
							printf("BRT = %i\n", BRT);
							printf("BRB = %i\n", BRB);
							*/
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
			printf("Min = %e, Max = %e\n", min, max);
			printf("MinIdx = %i, MaxIdx = %i\n", minIdx, maxIdx);
			
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
};



#endif
