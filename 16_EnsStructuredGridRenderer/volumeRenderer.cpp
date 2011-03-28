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
#include "vtkVolumeRayCastMapper.h"
#include "vtkCutter.h"

#include "MyStructuredGridReader.h"

int main(int, char* [])
{
	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	// load the volume data
//	MyStructuredGridReader reader("../SampleData/RAW/ens.raw",258,256,512,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	MyStructuredGridReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	
	// Create transfer mapping scalar value to opacity.
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
//	opacityTransferFunction->AddPoint(-0.000000000903996, 0.0); // quantized = 0
//	opacityTransferFunction->AddPoint(0.00000000451572, 0.333);	// quantized = 85
//	opacityTransferFunction->AddPoint(0.00000000995172, 0.667); // quantized = 170
//	opacityTransferFunction->AddPoint(0.0000000153714, 1.0); // quantized = 255
	
	opacityTransferFunction->AddPoint(-0.000000000903996, 1000.0); // quantized = 0
	opacityTransferFunction->AddPoint(0.00000000451572, 1000.0);	// quantized = 85
	opacityTransferFunction->AddPoint(0.00000000995172, 1000.0); // quantized = 170
	opacityTransferFunction->AddPoint(0.0000000153714, 1000.0); // quantized = 255
	 
	// Create transfer mapping scalar value to color.	
	vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
	colorTransferFunction->SetColorSpaceToHSV();
	colorTransferFunction->AddRGBPoint(-0.000000000903996, 1.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000451572, 0.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000995172, 0.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(0.0000000153714, 1.0, 0.0, 1.0);
	
	colorTransferFunction->AddHSVPoint(-0.000000000903996, 0.0, 1.0, 1.0);
	colorTransferFunction->AddHSVPoint(0.00000000451572, 0.467, 1.0, 1.0);
	colorTransferFunction->AddHSVPoint(0.00000000995172, 0.94, 1.0, 1.0);
	colorTransferFunction->AddHSVPoint(0.0000000153714, 1.17, 1.0, 1.0);
	
	// set up the volume properties
	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetColor(colorTransferFunction);
	property->SetScalarOpacity(opacityTransferFunction);
	property->ShadeOn();
	property->SetInterpolationTypeToLinear();

	/*
	// filter the structured grid
	vtkStructuredGridGeometryFilter *filter = vtkStructuredGridGeometryFilter::New();
	filter->SetInputConnection(reader.GetOutput()->GetProducerPort());
	filter->Update();
	*/
	/*
	// Create a mapper and actor
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(filter->GetOutputPort());
	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
//	actor->GetProperty()->SetPointSize(3);
	ren1->AddActor(actor);
	*/

	vtkVolumeRayCastMapper *mapper = vtkVolumeRayCastMapper::New();
	mapper->SetInput(reader.GetOutput());

/*
	// set up mapper and compositing function
	vtkHAVSVolumeMapper *mapper = vtkHAVSVolumeMapper::New();
	printf("Mapper Getting output\n");
	mapper->SetInput(reader.GetOutput());
	printf("Mapper got output\n");
*/

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
