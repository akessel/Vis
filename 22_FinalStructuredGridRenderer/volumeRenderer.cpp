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
#include "vtkPlane.h"
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"
#include "vtkLookupTable.h"
#include "vtkOpenGLRenderer.h"
#include "vtkInteractorStyleJoystickActor.h"
#include "vtkObjectFactory.h"
#include "vtkRendererCollection.h"
#include "vtkFastSplatter.h"
#include "vtkDataSetMapper.h"
#include "vtkTextActor.h"

#include "MyStructuredGridReader.h"
#include "MyFPSCallback.h"

int main(int, char* [])
{
	// setting up the renderer, window and interactor
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);
	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);
	
//	printf(ren1->GetClassName());
//	printf("\n");
	
	// load the volume data
//	MyStructuredGridReader reader("../SampleData/RAW/ens.raw",258,256,512,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	MyStructuredGridReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	
	vtkDataSetMapper *mapper = vtkDataSetMapper::New();
	mapper->SetInput(reader.GetOutput());
	
	vtkLookupTable *table = vtkLookupTable::New();
	table->SetNumberOfColors(4);
	table->Build();
	table->SetTableValue(0, 1.0, 0.0, 0.0, 0.1);
	table->SetTableValue(1, 0.0, 1.0, 0.0, 0.1);
	table->SetTableValue(2, 0.0, 0.0, 1.0, 0.1);
	table->SetTableValue(3, 1.0, 0.0, 1.0, 0.1);
//	table->SetTableRange(-0.000000000903996, 0.0000000153714);
//	table->SetAlphaRange(0.1, 0.1);
//	table->SetHueRange(0.0, 1.0);
//	table->SetSaturationRange(0.0, 1.0);
//	table->SetValueRange(0.0, 1.0);
//	table->SetRampToLinear();
	mapper->SetLookupTable(table);
	mapper->SetScalarRange(-0.000000000903996, 0.0000000153714);
	
	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	ren1->AddActor(actor);
	
	vtkTextActor *text = vtkTextActor::New();	
	text->SetDisplayPosition(10, 10);
	ren1->AddActor2D(text);	
	MyFPSCallback *UpdateFPS = MyFPSCallback::New();
	UpdateFPS->SetTextActor(text);
	ren1->AddObserver(vtkCommand::EndEvent,UpdateFPS);

	// start the rendering
	iren->Initialize();
	renWin->Render();
	iren->Start();
	
	return EXIT_SUCCESS;
}
