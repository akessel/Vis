#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkVolumeProperty.h"
#include "vtkVolume.h"

#include "vtkVolumeRayCastMapper.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"

#include "vtkStructuredPointsReader.h"
#include "vtkStructuredGridReader.h"
#include "vtkImageReader.h"
#include "vtkRectilinearGridReader.h"
#include "vtkPolyDataReader.h"

#include "vtkImageCast.h"
#include "vtkImageShiftScale.h"

#define FILEPATH "../SampleData/RAW/Particles.raw"

int main(int, char* [])
{

	// source
	vtkImageReader *reader = vtkImageReader::New();
	reader->SetFileName(FILEPATH);
	reader->SetFileDimensionality(3);
	reader->SetDataByteOrderToBigEndian();
	reader->SetDataExtent(0,31,0,31,0,31);
	reader->SetDataScalarTypeToUnsignedShort();
	reader->SetDataOrigin(0.0, 0.0, 0.0);
	reader->SetNumberOfScalarComponents(1);
	reader->SetHeaderSize(0);
	/*
	vtkImageShiftScale *shifter = vtkImageShiftScale::New();
	shifter->SetInputConnection(reader->GetOutputPort());
	shifter->SetOutputScalarTypeToUnsignedShort();
	shifter->SetShift(32767.0);
	*/
//	vtkImageCast *caster = vtkImageCast::New();
//	caster->SetInputConnection(reader->GetOutputPort());
//	caster->SetOutputScalarTypeToUnsignedShort();
	
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
	vtkVolumeRayCastMapper *mapper = vtkVolumeRayCastMapper::New();	
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
