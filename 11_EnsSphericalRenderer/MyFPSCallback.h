#ifndef _MYFPSCALLBACK_H_
#define _MYFPSCALLBACK_H_

#include "vtkCommand.h"

class MyFPSCallback : public vtkCommand
{
	public:
	   static MyFPSCallback *New()
		 { return new MyFPSCallback; }

	   void SetTextActor(vtkTextActor *txt)
		 {
		 this->TextActor = txt;
		 }

	   virtual void Execute(vtkObject *caller, unsigned long, void*)
		 {
		 vtkRenderer *ren = reinterpret_cast<vtkRenderer *>(caller);
		 float fps = 1.0/ren->GetLastRenderTimeInSeconds();
//		 sprintf(this->TextBuff,"%.1f", fps);
//		 this->TextActor->SetInput(this->TextBuff);
		 printf("FPS: %g\n", fps);
		 }
	protected:
	   vtkTextActor *TextActor;
	   char TextBuff[128];
};

#endif
