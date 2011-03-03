#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkCamera.h"


int main(int, char* [])
{

	// setting up the renderer and window
	vtkRenderWindow *renWin = vtkRenderWindow::New();
	vtkRenderer *ren1 = vtkRenderer::New();
	renWin->AddRenderer(ren1);

	// set up mapper and actor
	vtkConeSource *cone = vtkConeSource::New();
	cone->SetResolution(8);
	vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
	mapper->SetInput(cone->GetOutput());
	vtkActor *actor = vtkActor::New();
	actor->SetMapper(mapper);
	ren1->AddActor(actor);

	// render
	renWin->Render();

	while(true)
	{
		ren1->GetActiveCamera()->Azimuth(1);
		renWin->Render();
	}
}
