#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkImageReader2.h"
#include "vtkHAVSVolumeMapper.h"
#include "vtkImageToStructuredPoints.h"
#include "vtkStructuredPointsReader.h"

#include "MyUnstructuredGridReader.h"

#define FILEPATH "../SampleData/RAW/ens0.raw"


int main(int, char* [])
{
	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	// load the volume data
//	MyUnstructuredGridReader reader(FILEPATH,258,256,512,0,0,0);
	MyUnstructuredGridReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyUnstructuredGridReader reader(FILEPATH,65,64,128,0,0,0);
	
	// Create transfer mapping scalar value to opacity.
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
	opacityTransferFunction->AddPoint(-0.000000000903996, 0.0); // quantized = 0
	opacityTransferFunction->AddPoint(0.00000000451572, 0.333);	// quantized = 85
	opacityTransferFunction->AddPoint(0.00000000995172, 0.667); // quantized = 170
	opacityTransferFunction->AddPoint(0.0000000153714, 1.0); // quantized = 255

	 
	// Create transfer mapping scalar value to color.	
	vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
	double range[] = {-0.000000000903996, 0.000000015698};
//	colorTransferFunction->AdjustRange(range);
	colorTransferFunction->SetColorSpaceToHSV();
	colorTransferFunction->AddRGBPoint(-0.000000000903996, 255.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000451572, 4.0, 255.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000995172, 0.0, 0.0, 255.0);
	colorTransferFunction->AddRGBPoint(0.0000000153714, 251.0, 0.0, 255.0);
	
	colorTransferFunction->AddHSVPoint(-0.000000000903996, 0.0, 255.0, 255.0);
	colorTransferFunction->AddHSVPoint(0.00000000451572, 119.0, 255.0, 255.0);
	colorTransferFunction->AddHSVPoint(0.00000000995172, 240.0, 255.0, 255.0);
	colorTransferFunction->AddHSVPoint(0.0000000153714, 299.0, 255.0, 255.0);
	
	// set up the volume properties
	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetColor(colorTransferFunction);
	property->SetScalarOpacity(opacityTransferFunction);
	property->ShadeOff();
	property->SetInterpolationTypeToLinear();

	// set up mapper and compositing function
	vtkHAVSVolumeMapper *mapper = vtkHAVSVolumeMapper::New();
	printf("Mapper Getting output\n");
	mapper->SetInput(reader.GetOutput());
	printf("Mapper got output\n");

	// set up the volume (actor)
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	volume->SetProperty(property);	
	ren1->AddVolume(volume);

	// start the rendering
	iren->Initialize();
	iren->Start();
	
	return EXIT_SUCCESS;
}
