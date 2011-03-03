#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkStructuredPointsReader.h"
#include "vtkVolumeTextureMapper3D.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"

#define FILEPATH "../../VTKData/Data/ironProt.vtk"

int main(int, char* [])
{

	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// load the volume data
	vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
	reader->SetFileName(FILEPATH);

	// set up mapper and compositing function
	vtkVolumeRayCastCompositeFunction *function = vtkVolumeRayCastCompositeFunction::New();
	function->SetCompositeMethodToInterpolateFirst();
	
	vtkVolumeTextureMapper3D *mapper = vtkVolumeTextureMapper3D::New();
	mapper->SetInputConnection(reader->GetOutputPort());
	mapper->SetVolumeRayCastFunction(function);
 
	// Create transfer mapping scalar value to opacity.
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New();
	opacityTransferFunction->AddPoint(80.0,  0.0);
	opacityTransferFunction->AddPoint(120.0, 0.2);
	opacityTransferFunction->AddPoint(255.0, 0.2);
	 
	// Create transfer mapping scalar value to color.
	vtkColorTransferFunction *colorTransferFunction = vtkColorTransferFunction::New();
	colorTransferFunction->AddRGBPoint(80.0,  0.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(120.0, 0.0, 0.0, 1.0);
	colorTransferFunction->AddRGBPoint(160.0, 1.0, 0.0, 0.0);
	colorTransferFunction->AddRGBPoint(200.0, 0.0, 1.0, 0.0);
	colorTransferFunction->AddRGBPoint(255.0, 0.0, 1.0, 1.0);
	
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
