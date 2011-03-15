#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRectilinearGridReader.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"
#include "vtkStructuredPointsReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkPolyDataReader.h"
#include "vtkDataSetMapper.h"
#include "vtkRectilinearGridReader.h"
#include "vtkRectilinearGridToTetrahedra.h"
#include "vtkStructuredPointsReader.h"

#define FILEPATH "../SampleData/StructuredPoints/matrix.vtk"

int main(int, char* [])
{

	// source
//	vtkRectilinearGridReader *reader = vtkRectilinearGridReader::New();
	vtkStructuredPointsReader *reader = vtkStructuredPointsReader::New();
//	vtkStructuredGridReader *reader = vtkStructuredGridReader::New();
//	vtkPolyDataReader *reader = vtkPolyDataReader::New();
//	vtkImageDataReader *reader = vtkImageDataReader::New();
	reader->SetFileName(FILEPATH);
	
	//vtkRectilinearGridToTetrahedra *filter = vtkRectilinearGridToTetrahedra::New();
	//filter->SetInputConnection(reader->GetOutputPort());
	
	vtkVolumeRayCastCompositeFunction *function = vtkVolumeRayCastCompositeFunction::New();
	function->SetCompositeMethodToInterpolateFirst();

	// set up the transfer and opacity functions
	vtkColorTransferFunction *colorTrans = vtkColorTransferFunction::New();
	colorTrans->AddRGBPoint(64, 1.0, 0.0, 0.0);
	colorTrans->AddRGBPoint(128, 0.0, 1.0, 0.0);
	colorTrans->AddRGBPoint(196, 0.0, 0.0, 1.0);

	vtkPiecewiseFunction *opacity = vtkPiecewiseFunction::New();
	opacity->AddPoint(0, 0.0);
	opacity->AddPoint(255, 0.2);
	
	// mapper
	vtkVolumeRayCastMapper *mapper = vtkVolumeRayCastMapper::New();	
//	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->SetInputConnection(reader->GetOutputPort());
//	mapper->SetInputConnection(algorithm->GetOutputPort());
	mapper->SetVolumeRayCastFunction(function);
	
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
//	vtkActor *actor = vtkActor::New();
//	actor->SetMapper(mapper);

	
	// renderers
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	ren1->AddViewProp(volume);
//	ren1->AddActor(actor);
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
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
