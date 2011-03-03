#ifndef _MYFILTER_H_
#define _MYFILTER_H_

#include "vtkImageData.h"
#include "vtkUnstructuredGrid.h"

class MyFilter
{
	private:
		vtkImageData *data;
		
		int nX, nY, nZ;
		
		int phi, theta, rho; // long, lat, radius

	public:
		MyFilter(int x, int y, int z, int phi, int theta, int rho);
		
		~MyFilter();
	
		void SetInput(vtkImageData *data);
		
		vtkUnstructuredGrid * GetOutput();
};

#endif
