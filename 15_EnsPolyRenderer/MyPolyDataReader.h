#ifndef _MYUNSTRUCTUREDGRIDREADER_H_
#define _MYUNSTRUCTUREDGRIDREADER_H_

#include <stdio.h>
#include <string>
#include <math.h>
#include "vtkPolyData.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkCellType.h"
#include "vtkCellArray.h"
#include "vtkTetra.h"
#include "vtkVertex.h"
#include "MyFileReader.h"
#include "vtkLine.h"
#include "vtkVertex.h"

#define PI 3.14159265

using namespace std;

class MyPolyDataReader
{
	private:
		int nRad, nLat, nLong;
		
		double minRad, minLat, minLong;
		
		double maxRad, maxLat, maxLong;
		
		MyFileReader<float> reader;

	public:
		MyPolyDataReader(string filename, int nRad, int nLat, int nLong,
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
		
		vtkPolyData * GetOutput()
		{
			printf("Starting to get output...\n");
	
			vtkPolyData *data = vtkPolyData::New();
	
			int numPoints = this->nRad * this->nLat * this->nLong;			
	
			vtkPoints *points = vtkPoints::New();
//			vtkFloatArray *scalars = vtkFloatArray::New();
			vtkCellArray *cells = vtkCellArray::New();
			vtkCellArray *vertices = vtkCellArray::New();
			
			points->SetNumberOfPoints(numPoints);
//			scalars->SetNumberOfValues(numPoints);

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
						
						double x = rad * sin(theta*PI/180.0) * cos(phi*PI/180.0);
						double y = rad * sin(phi*PI/180.0);
						double z = rad * cos(theta*PI/180.0) * cos(phi*PI/180.0);
						/*
						x = (double)k;
						y = (double)j;
						z = (double)i;
						*/
						points->InsertPoint(count, x, y, z);									
//						scalars->InsertValue(count, value);
						
						vtkVertex *vertex = vtkVertex::New();
						vertex->GetPointIds()->SetNumberOfIds(1);
						vertex->GetPointIds()->SetId(0, count);
						vertices->InsertNextCell(vertex);
						
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
							
							vtkLine *line01 = vtkLine::New();
							vtkLine *line02 = vtkLine::New();
							vtkLine *line03 = vtkLine::New();
							vtkLine *line04 = vtkLine::New();
							vtkLine *line05 = vtkLine::New();
							vtkLine *line06 = vtkLine::New();
							vtkLine *line07 = vtkLine::New();
							vtkLine *line08 = vtkLine::New();
							vtkLine *line09 = vtkLine::New();
							vtkLine *line10 = vtkLine::New();
							vtkLine *line11 = vtkLine::New();
							vtkLine *line12 = vtkLine::New();
							vtkLine *line13 = vtkLine::New();
							vtkLine *line14 = vtkLine::New();
							vtkLine *line15 = vtkLine::New();
							vtkLine *line16 = vtkLine::New();
							vtkLine *line17 = vtkLine::New();
							vtkLine *line18 = vtkLine::New();
							
							line01->GetPointIds()->SetNumberOfIds(2);
							line02->GetPointIds()->SetNumberOfIds(2);
							line03->GetPointIds()->SetNumberOfIds(2);
							line04->GetPointIds()->SetNumberOfIds(2);
							line05->GetPointIds()->SetNumberOfIds(2);
							line06->GetPointIds()->SetNumberOfIds(2);
							line07->GetPointIds()->SetNumberOfIds(2);
							line08->GetPointIds()->SetNumberOfIds(2);
							line09->GetPointIds()->SetNumberOfIds(2);
							line10->GetPointIds()->SetNumberOfIds(2);
							line11->GetPointIds()->SetNumberOfIds(2);
							line12->GetPointIds()->SetNumberOfIds(2);
							line13->GetPointIds()->SetNumberOfIds(2);
							line14->GetPointIds()->SetNumberOfIds(2);
							line15->GetPointIds()->SetNumberOfIds(2);
							line16->GetPointIds()->SetNumberOfIds(2);
							line17->GetPointIds()->SetNumberOfIds(2);
							line18->GetPointIds()->SetNumberOfIds(2);
							
							line01->GetPointIds()->SetId(0, FLB);
							line01->GetPointIds()->SetId(1, FLT);
							
							line02->GetPointIds()->SetId(0, FLT);
							line02->GetPointIds()->SetId(1, FRT);
							
							line03->GetPointIds()->SetId(0, FRT);
							line03->GetPointIds()->SetId(1, FRB);
							
							line04->GetPointIds()->SetId(0, FRB);
							line04->GetPointIds()->SetId(1, FLB);
							
							line05->GetPointIds()->SetId(0, BLT);
							line05->GetPointIds()->SetId(1, BLB);
							
							line06->GetPointIds()->SetId(0, BLT);
							line06->GetPointIds()->SetId(1, BRT);
							
							line07->GetPointIds()->SetId(0, BRT);
							line07->GetPointIds()->SetId(1, BRB);
							
							line08->GetPointIds()->SetId(0, BRB);
							line08->GetPointIds()->SetId(1, BLB);
							
							line09->GetPointIds()->SetId(0, FLT);
							line09->GetPointIds()->SetId(1, BLT);
							
							line10->GetPointIds()->SetId(0, FLB);
							line10->GetPointIds()->SetId(1, BLB);
							
							line11->GetPointIds()->SetId(0, FRT);
							line11->GetPointIds()->SetId(1, BRT);
							
							line12->GetPointIds()->SetId(0, FRB);
							line12->GetPointIds()->SetId(1, BRB);
							
							line13->GetPointIds()->SetId(0, FLT);
							line13->GetPointIds()->SetId(1, BRT);
							
							line14->GetPointIds()->SetId(0, FLT);
							line14->GetPointIds()->SetId(1, FRB);
							
							line15->GetPointIds()->SetId(0, FLT);
							line15->GetPointIds()->SetId(1, BLB);
							
							line16->GetPointIds()->SetId(0, BRT);
							line16->GetPointIds()->SetId(1, BLB);
							
							line17->GetPointIds()->SetId(0, BRT);
							line17->GetPointIds()->SetId(1, FRB);
							
							line18->GetPointIds()->SetId(0, BLB);
							line18->GetPointIds()->SetId(1, FRB);
							
							if(count%10000 == 0 || (i==1 && j==1 && k==1)){
							cells->InsertNextCell(line01);
							cells->InsertNextCell(line02);
							cells->InsertNextCell(line03);
							cells->InsertNextCell(line04);
							cells->InsertNextCell(line05);
							cells->InsertNextCell(line06);
							cells->InsertNextCell(line07);
							cells->InsertNextCell(line08);
							cells->InsertNextCell(line09);
							cells->InsertNextCell(line10);
							cells->InsertNextCell(line11);
							cells->InsertNextCell(line12);
							cells->InsertNextCell(line13);
							cells->InsertNextCell(line14);
							cells->InsertNextCell(line15);
							cells->InsertNextCell(line16);
							cells->InsertNextCell(line17);
							cells->InsertNextCell(line18);
							}
						}
					}
				}
			}
			
			printf("Inserting points...\n");
			data->SetPoints(points);
//			printf("Inserting scalars...\n");
//			data->GetPointData()->SetScalars(scalars);
			printf("Inserting cells...\n");
			data->SetLines(cells); // 3 = VTK_LINE
			data->SetVerts(vertices); // 3 = VTK_LINE

			printf("Ending get output...\n");
			
			printf("Count = %i\n", count);
			printf("Min = %e, Max = %e\n", min, max);
			printf("MinIdx = %i, MaxIdx = %i\n", minIdx, maxIdx);
			
			return data;		
		}
		
		~MyPolyDataReader()
		{
		}
		
	private:		
		int LinearizeCoordinate(int i, int j, int k)
		{
			return i*this->nLat*this->nRad + j*this->nRad + k;
		}
};



#endif
