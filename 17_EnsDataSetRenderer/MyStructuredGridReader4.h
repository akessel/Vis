#ifndef _MYUNSTRUCTUREDGRIDREADER4_H_
#define _MYUNSTRUCTUREDGRIDREADER4_H_

#include <stdio.h>
#include <string>
#include <math.h>
#include "vtkStructuredGrid.h"
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

class MyStructuredGridReader4
{
	private:
		int nRad, nLat, nLong;
		
		double minRad, minLat, minLong;
		
		double maxRad, maxLat, maxLong;
		
		MyFileReader<float> reader;

	public:
		MyStructuredGridReader4(string filename, int nRad, int nLat, int nLong,
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
		
		vtkStructuredGrid * GetOutput()
		{
			printf("Starting to get output...\n");
	
			vtkStructuredGrid *grid = vtkStructuredGrid::New();
			grid->SetDimensions(this->nRad, this->nLat, this->nLong);
	
			int numPoints = this->nRad * this->nLat * (this->nLong + 1); // +1 for nLong for the ghost row/stitching up the discontinuity	
	
			vtkPoints *points = vtkPoints::New();
			vtkFloatArray *scalars = vtkFloatArray::New();
			
			points->SetNumberOfPoints(numPoints);
			scalars->SetNumberOfValues(numPoints);

			double xMin = 10000000.0;
			double xMax = -10000000.0;
			double yMin = 10000000.0;
			double yMax = -10000000.0;
			double zMin = 10000000.0;
			double zMax = -10000000.0;

			/*
			this->nLong = 2;
			this->nLat = 2;
			this->nRad = 2;
			*/
			// iterate through the points, transforming them to spherical
			int count = 0;
			for(int i = 0; i <= this->nLong; i++) // extra step along longitude will sew up the discontinuity
			{		
				for(int j = 0; j < this->nLat; j++)
				{
					for(int k = 0; k < this->nRad; k++)
					{
						if(i != this->nLong)
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
						
							if(x > xMax) xMax = x;
							if(x < xMin) xMin = x;
							if(y > yMax) yMax = y;
							if(y < yMin) yMin = y;
							if(z > zMax) zMax = z;
							if(z < zMin) zMin = z;
						
							/*
							x = (double)k;
							y = (double)j;
							z = (double)i;
							*/
							points->InsertPoint(count, x, y, z);									
							scalars->InsertValue(count, value);
						}
						else // i == this->nLong // ghost rows; stitching up the discontinuity
						{
							int pseudoCount = j * this->nRad + k;
							float value = scalars->GetValue(pseudoCount);
						
							double rad = k * ( (this->maxRad - this->minRad) / this->nRad ) + this->minRad;
							double phi = j * ( (this->maxLat - this->minLat) / this->nLat ) + this->minLat;
							double theta = /*i * ( (this->maxLong - this->minLong) / this->nLong ) + */this->minLong - 0.000000000000000000001;
						
							double x = rad * sin(theta*PI/180.0) * cos(phi*PI/180.0);
							double y = rad * sin(phi*PI/180.0);
							double z = rad * cos(theta*PI/180.0) * cos(phi*PI/180.0);
						
							points->InsertPoint(count, x, y, z);									
							scalars->InsertValue(count, value);
						}
				
						count++;					
					}
				}
			}
			
			printf("Inserting points...\n");
			grid->SetPoints(points);
			printf("Inserting scalars...\n");
			grid->GetPointData()->SetScalars(scalars);

			printf("Ending get output...\n");
			
			printf("Count = %i\n", count);
			printf("x: Min = %e, Max = %e\n", xMin, xMax);
			printf("y: Min = %e, Max = %e\n", yMin, yMax);
			printf("z: Min = %e, Max = %e\n", zMin, zMax);			
			//printf("Min = %e, Max = %e\n", min, max);
			
			return grid;		
		}
		
		~MyStructuredGridReader4()
		{
		}
		
	private:		
		int LinearizeCoordinate(int i, int j, int k)
		{
			return i*this->nLat*this->nRad + j*this->nRad + k;
		}
};



#endif
