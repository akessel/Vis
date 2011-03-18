#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"
#include "MyPolyDataReader.h"

int main(int, char* [])
{
	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	// load the volume data
//	MyPolyDataReader reader("../SampleData/RAW/ens.raw",258,256,512,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	MyPolyDataReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyPolyDataReader reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyPolyDataReader reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	function->SetCompositeMethodToInterpolateFirst();
	
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInput(reader.GetOutput());
 /*
	// Create transfer mapping scalar value to opacity.
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
	opacityTransferFunction->AddPoint(-0.000000000903996, 0.0); // quantized = 0
	opacityTransferFunction->AddPoint(0.00000000451572, 0.333);	// quantized = 85
	opacityTransferFunction->AddPoint(0.00000000995172, 0.667); // quantized = 170
	opacityTransferFunction->AddPoint(0.0000000153714, 1.0); // quantized = 255
	 
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
	property->ShadeOff();
	property->SetInterpolationTypeToLinear();
	
	// set up the volume (actor)
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	volume->SetProperty(property);	
	ren1->AddViewProp(volume);
*/
	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	ren1->AddActor(actor);
	
	// start the rendering
	iren->Initialize();
	iren->Start();
	
	return EXIT_SUCCESS;
}
