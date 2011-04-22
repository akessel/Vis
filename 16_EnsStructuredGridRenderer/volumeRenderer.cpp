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
#include "vtkTextActor.h"

#include "MyFPSCallback.h"
#include "MyStructuredGridReader.h"
#include "MyStructuredGridReader3.h"

void Cut(int n, vtkCamera *cam1, vtkPlane *plane, vtkCutter *cutter, vtkRenderWindow *renWin)
{
	printf("Cutting...\n");
	vtkRenderer *ren1 = renWin->GetRenderers()->GetFirstRenderer();
	vtkCamera *cam = ren1->GetActiveCamera();
	cam->ComputeViewPlaneNormal();
	plane->SetNormal(cam->GetViewPlaneNormal());
	plane->SetOrigin(cam->GetFocalPoint());
	cutter->GenerateValues(n, -1.0, 1.0);
	renWin->Render();
}

// Define interaction style
class MyInteractorStyle : public vtkInteractorStyleJoystickActor
{
  public:
    static MyInteractorStyle* New();
    vtkTypeRevisionMacro(MyInteractorStyle, vtkInteractorStyleJoystickActor);
 
    virtual void OnLeftButtonUp() 
    { 
    	printf("OnLeftButtonUp\n");
    	Cut(this->N, this->Cam, this->Plane, this->Cutter, this->RenWin);
      // Forward events
      vtkInteractorStyleJoystickActor::OnLeftButtonUp();
    }
    
    virtual void OnMouseWheelForward() 
    { 
      // Forward events
      vtkInteractorStyleJoystickActor::OnMouseWheelForward();
    }
    
    virtual void OnMouseWheelBackward() 
    { 
      // Forward events
      vtkInteractorStyleJoystickActor::OnMouseWheelBackward();
    }
 
    void SetActor(vtkActor *actor) {this->Actor = actor;}
    void Set(int n, vtkActor *actor, vtkPlane *plane, vtkCamera *cam, vtkCutter *cutter, vtkRenderWindow *renWin)
    {
    	this->N = n;
    	this->Actor = actor;
    	this->Plane = plane;
    	this->Cam = cam;
    	this->Cutter = cutter;
    	this->RenWin = renWin;
    }
 
  private:
  	int N;
    vtkActor *Actor;
    vtkPlane *Plane;
    vtkCamera *Cam;
    vtkCutter *Cutter;
    vtkRenderWindow *RenWin;
 
 
};
vtkCxxRevisionMacro(MyInteractorStyle, "$Revision: 1.1 $");
vtkStandardNewMacro(MyInteractorStyle);


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
//	MyStructuredGridReader reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);

//	MyStructuredGridReader3 reader("../SampleData/RAW/ens.raw",258,256,512,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	MyStructuredGridReader3 reader("../SampleData/RAW/ens0.raw",33,32,64,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader3 reader("../SampleData/RAW/ens1.raw",65,64,128,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
//	MyStructuredGridReader3 reader("../SampleData/RAW/ens2.raw",129,128,256,0.571,-89.6484,-179.648,1.0,89.6484,179.648);
	
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
	
	vtkPlane *plane = vtkPlane::New();
	plane->SetOrigin(0,0,0);
	plane->SetNormal(0,1,0);
	
	vtkCutter *cutter = vtkCutter::New();
	cutter->SetInput(reader.GetOutput());
	cutter->SetCutFunction(plane);
	cutter->GenerateCutScalarsOff();
	cutter->SetSortByToSortByCell();
	
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInputConnection(cutter->GetOutputPort());
	
	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	ren1->AddActor(actor);
	
	vtkCamera *cam = ren1->GetActiveCamera();
	cam->SetFocalPoint(0,0,0);
	cam->ComputeViewPlaneNormal();

	int n = 20;	
	plane->SetNormal(cam->GetViewPlaneNormal());
	plane->SetOrigin(cam->GetFocalPoint());
	cutter->GenerateValues(n, -1.0, 1.0);
	
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
	
	MyInteractorStyle *style = MyInteractorStyle::New();
	style->Set(n, actor, plane, cam, cutter, renWin);
	iren->SetInteractorStyle(style);
	
	
	
	/*
	// set up the volume properties
	vtkVolumeProperty *property = vtkVolumeProperty::New();
	property->SetColor(colorTransferFunction);
	property->SetScalarOpacity(opacityTransferFunction);
	property->ShadeOn();
	property->SetInterpolationTypeToLinear();
	*/
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
/*
	vtkVolumeRayCastMapper *mapper = vtkVolumeRayCastMapper::New();
	mapper->SetInput(reader.GetOutput());
*/
/*
	// set up mapper and compositing function
	vtkHAVSVolumeMapper *mapper = vtkHAVSVolumeMapper::New();
	printf("Mapper Getting output\n");
	mapper->SetInput(reader.GetOutput());
	printf("Mapper got output\n");
*/
/*
	// set up the volume (actor)
	vtkVolume *volume = vtkVolume::New();
	volume->SetMapper(mapper);
	volume->SetProperty(property);	
	ren1->AddVolume(volume);
*/
	// display the fps
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
