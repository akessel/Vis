# define the project
PROJECT(boilerplate)

# find VTK
FIND_PACKAGE(VTK)
IF(VTK_FOUND)
	INCLUDE(${VTK_USE_FILE})
ELSE(VTK_FOUND)
	MESSAGE(FATAL_ERROR
		"Cannot build the executable without VTK. Please set the
		VTK_DIR")
ENDIF(VTK_FOUND)

# define the executable
ADD_EXECUTABLE(boilerplate boilerplate.cpp)

TARGET_LINK_LIBRARIES(boilerplate
	vtkCommon
	vtkGraphics
	vtkRendering
)

