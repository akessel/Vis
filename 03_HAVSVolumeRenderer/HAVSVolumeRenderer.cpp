#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkDataSetMapper.h"
#include "vtkImageReader.h"
#include "vtkCamera.h"
#include "vtkRectilinearGridReader.h"
#include "vtkDataSet.h"
#include "vtkHAVSVolumeMapper.h"
#include "vtkOpenGLHAVSVolumeMapper.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredGridReader.h"

#define FILEPATH "../SampleData/RectGrid2.vtk"

int main(int, char* [])
{
	// source
	vtkRectilinearGridReader *reader = vtkRectilinearGridReader::New();
//	vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
//	vtkStructuredGridReader *reader = vtkStructuredGridReader::New();
	reader->SetFileName(FILEPATH);
	
//	vtkVolumeRayCastCompositeFunction *function = vtkVolumeRayCastCompositeFunction::New();
//	function->SetCompositeMethodToInterpolateFirst();

	// set up the transfer and opacity functions
	vtkColorTransferFunction *colorTrans = vtkColorTransferFunction::New();
	colorTrans->AddRGBPoint(64, 1.0, 0.0, 0.0);
	colorTrans->AddRGBPoint(128, 0.0, 1.0, 0.0);
	colorTrans->AddRGBPoint(196, 0.0, 0.0, 1.0);

	vtkPiecewiseFunction *opacity = vtkPiecewiseFunction::New();
	opacity->AddPoint(0, 0.0);
	opacity->AddPoint(255, 0.2);
	
	// mapper
	vtkOpenGLHAVSVolumeMapper *mapper = vtkOpenGLHAVSVolumeMapper::New();
	mapper->SetInputConnection(reader->GetOutputPort());
//	mapper->SetVolumeRayCastFunction(function);
	
	// props
	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetColor(colorTrans);
	property->SetScalarOpacity(opacity);
	property->SetInterpolationTypeToLinear();
	property->ShadeOn();
	property->SetAmbient(0.4);
	property->SetDiffuse(0.6);
	property->SetSpecular(0.2);
	
	// actor
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	volume->SetProperty(property);	
	
	// renderers
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->AddViewProp(volume);
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
	// start the rendering
	iren->Initialize();
	iren->Start();
	
	return EXIT_SUCCESS;

/*

	// setting up the renderer and window
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);

	// set up mapper and actor
	vtkOpenGLHAVSVolumeMapper *mapper = vtkOpenGLHAVSVolumeMapper::New();
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	ren1->AddActor(volume);

	// load the volume data
	vtkRectilinearGridReader *reader = vtkRectilinearGridReader::New();
	reader->SetFileName(FILEPATH);
	mapper->SetInput((vtkDataSet *)reader->GetOutput());

	// render
	renWin->Render();

	while(true)
	{
		ren1->GetActiveCamera()->Azimuth(1);
		ren1->GetActiveCamera()->Roll(1);
		renWin->Render();
	}
*/
}
