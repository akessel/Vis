#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkImageReader2.h"
#include "vtkFixedPointVolumeRayCastMapper.h"

#include "MyFilter.h"

#define FILEPATH "../SampleData/RAW/ens.raw"

int main(int, char* [])
{
	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// load the volume data
	vtkImageReader2 *reader = vtkImageReader2::New();
	reader->SetFileDimensionality(3);
	reader->SetDataScalarTypeToFloat();
	reader->SetDataExtent(0, 257, 0, 255, 0, 511);
	reader->SetFileName(FILEPATH);
	reader->Update();
	
	// transform to spherical
	MyFilter filter (258, 256, 512, 0, 0, 0);
	
	// set up mapper and compositing function
//	vtkVolumeRayCastCompositeFunction *function = vtkVolumeRayCastCompositeFunction::New();
//	function->SetCompositeMethodToInterpolateFirst();
	vtkFixedPointVolumeRayCastMapper *mapper = vtkFixedPointVolumeRayCastMapper::New();
	mapper->SetInputConnection(reader->GetOutputPort());
//	mapper->SetVolumeRayCastFunction(function);
 
	// Create transfer mapping scalar value to opacity.
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
	opacityTransferFunction->AddPoint(0.0000000006043, 0.18);
	opacityTransferFunction->AddPoint(0.0000000116628, 0.7);

	 
	// Create transfer mapping scalar value to color.
	vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
	colorTransferFunction->AddRGBPoint(0.000000000903996, 255.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000362272, 4.0, 255.0, 0.0);
	colorTransferFunction->AddRGBPoint(0.00000000995063, 0.0, 0.0, 255.0);
	colorTransferFunction->AddRGBPoint(0.0000000153698, 251.0, 0.0, 255.0);
	
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
	
	// start the rendering
	iren->Initialize();
	iren->Start();
	
	return EXIT_SUCCESS;
	
/*
	// render
	renWin->Render();

	while(true)
	{
		ren1->GetActiveCamera()->Azimuth(0.1);
		ren1->GetActiveCamera()->Roll(0.1);
		renWin->Render();
	}
*/
}
