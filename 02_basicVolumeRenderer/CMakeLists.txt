cmake_minimum_required(VERSION 2.6)

# define the project
PROJECT(basicVolumeRenderer)

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
ADD_EXECUTABLE(basicVolumeRenderer basicVolumeRenderer.cpp)

TARGET_LINK_LIBRARIES(basicVolumeRenderer
	vtkCommon
	vtkGraphics
	vtkIO
	vtkRendering
	vtkVolumeRendering
)

