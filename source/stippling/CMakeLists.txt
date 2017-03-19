PROJECT( stippling )
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

SET( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules )

FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE( OpenGL REQUIRED )
FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE( GLEW REQUIRED )
FIND_PACKAGE( GLM REQUIRED )
FIND_PACKAGE( CGAL QUIET COMPONENTS Core )

# Show library finding errors
if(NOT QT4_FOUND)
  message(ERROR " Qt4 not found!")
else(NOT QT4_FOUND)
  message(STATUS " Qt4 OK.")
endif(NOT QT4_FOUND)
if(NOT OPENGL_FOUND)
  message(ERROR " OpenGL not found!")
else(NOT OPENGL_FOUND)
  message(STATUS " OpenGL OK.")
endif(NOT OPENGL_FOUND)
if(NOT OpenCV_FOUND)
  message(ERROR " OpenCV not found!")
else(NOT OpenCV_FOUND)
  message(STATUS " OpenCV OK.")
endif(NOT OpenCV_FOUND)
if(NOT GLEW_FOUND)
  message(ERROR " Glew not found!")
else(NOT GLEW_FOUND)
  message(STATUS " Glew OK.")
endif(NOT GLEW_FOUND)
if(NOT GLM_FOUND)
  message(ERROR " GLM not found!")
else(NOT GLM_FOUND)
  message(STATUS " GLM OK.")
endif(NOT GLM_FOUND)
if(NOT CGAL_FOUND)
  message(ERROR " CGAL not found!")
else(NOT CGAL_FOUND)
  message(STATUS " CGAL OK.")
endif(NOT CGAL_FOUND)

#SET( CMAKE_CXX_FLAGS "-ansi -pedantic" )
#SET( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11" )

#SET( GLEW_LIBRARY "-lGLEW" )
SET( QT_USE_QTOPENGL TRUE )

SET( stippling_SOURCES 
	${CMAKE_CURRENT_BINARY_DIR}/src/main.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/mainwindow.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidget3dengine.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/glentity.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/framebuffer.cc
	${CMAKE_CURRENT_BINARY_DIR}/src/entity3d.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/controlpoint.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/camera.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/prism.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/util.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/idmanager.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/focallengthnotfounddialog.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/floorplane.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/cylinder.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplane.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplaney0.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplanez0.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/triangle.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/configurationdialog.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/configuration.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreewidget.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreewidgetitem.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreenode.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/glcamera.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/boolmesh.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreecontroller.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/rootentity.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/operation.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/heightindicator.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/quadtreenode.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/quadtree.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/stippledot.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidgetstippling.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/dotgenerationworker.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/specificentityconfigurationdialog.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/specificentityconfiguration.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidget3denginesuperiorvisualization.cpp

)

SET( stippling_HEADERS 
	${CMAKE_CURRENT_BINARY_DIR}/src/mainwindow.h
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidget3dengine.h
	${CMAKE_CURRENT_BINARY_DIR}/src/glentity.h
	${CMAKE_CURRENT_BINARY_DIR}/src/framebuffer.hh
	${CMAKE_CURRENT_BINARY_DIR}/src/entity3d.h
	${CMAKE_CURRENT_BINARY_DIR}/src/controlpoint.h
	${CMAKE_CURRENT_BINARY_DIR}/src/camera.h
	${CMAKE_CURRENT_BINARY_DIR}/src/prism.h
	${CMAKE_CURRENT_BINARY_DIR}/src/util.h
	${CMAKE_CURRENT_BINARY_DIR}/src/enums.h
	${CMAKE_CURRENT_BINARY_DIR}/src/idmanager.h
	${CMAKE_CURRENT_BINARY_DIR}/src/focallengthnotfounddialog.h
	${CMAKE_CURRENT_BINARY_DIR}/src/floorplane.h
	${CMAKE_CURRENT_BINARY_DIR}/src/cylinder.h
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplane.h
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplaney0.h
	${CMAKE_CURRENT_BINARY_DIR}/src/infiniteplanez0.h
	${CMAKE_CURRENT_BINARY_DIR}/src/triangle.cpp
	${CMAKE_CURRENT_BINARY_DIR}/src/configurationdialog.h
	${CMAKE_CURRENT_BINARY_DIR}/src/configuration.h
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreewidget.h
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreewidgetitem.h
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreenode.h
	${CMAKE_CURRENT_BINARY_DIR}/src/glcamera.h
	${CMAKE_CURRENT_BINARY_DIR}/src/boolmesh.h
	${CMAKE_CURRENT_BINARY_DIR}/src/entitytreecontroller.h
	${CMAKE_CURRENT_BINARY_DIR}/src/rootentity.h
	${CMAKE_CURRENT_BINARY_DIR}/src/operation.h
	${CMAKE_CURRENT_BINARY_DIR}/src/heightindicator.h
	${CMAKE_CURRENT_BINARY_DIR}/src/quadtreenode.h
	${CMAKE_CURRENT_BINARY_DIR}/src/quadtree.h
	${CMAKE_CURRENT_BINARY_DIR}/src/stippledot.h
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidgetstippling.h
	${CMAKE_CURRENT_BINARY_DIR}/src/dotgenerationworker.h
	${CMAKE_CURRENT_BINARY_DIR}/src/specificentityconfigurationdialog.h
	${CMAKE_CURRENT_BINARY_DIR}/src/specificentityconfiguration.h
	${CMAKE_CURRENT_BINARY_DIR}/src/glwidget3denginesuperiorvisualization.h

)

SET( stippling_FORMS 
	${CMAKE_CURRENT_BINARY_DIR}/ui/mainwindow.ui
	${CMAKE_CURRENT_BINARY_DIR}/ui/focallengthnotfounddialog.ui
	${CMAKE_CURRENT_BINARY_DIR}/ui/configurationdialog.ui
	${CMAKE_CURRENT_BINARY_DIR}/ui/specificentityconfigurationdialog.ui

)

SET( stippling_RESOURCES 
	${CMAKE_CURRENT_BINARY_DIR}/resources/shaders.qrc
	${CMAKE_CURRENT_BINARY_DIR}/resources/camera_specifications.qrc
)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/src )
INCLUDE_DIRECTORIES( ${QT_INCLUDES} )
INCLUDE_DIRECTORIES( ${GLM_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_PATH} )

QT4_WRAP_CPP( stippling_HEADERS_MOC ${stippling_HEADERS} )
QT4_WRAP_UI( stippling_FORMS_HEADERS ${stippling_FORMS} )
QT4_ADD_RESOURCES( stippling_RESOURCES_RCC ${stippling_RESOURCES} )

INCLUDE( ${CGAL_USE_FILE} )

INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS})


MESSAGE(STATUS "")
MESSAGE(STATUS "")
MESSAGE(STATUS "=== LIBRARY STATUS BEFORE LINKING ===")
MESSAGE(STATUS "")
MESSAGE(STATUS "QT_LIBRARIES = ${QT_LIBRARIES}")
MESSAGE(STATUS "")
MESSAGE(STATUS "QT_QTGUI_LIBRARY = ${QT_QTGUI_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "QT_CORE_LIBRARY = ${QT_CORE_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "QT_QTOPENGL_LIBRARY = ${QT_QTOPENGL_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "QT_QTXML_LIBRARY = ${QT_QTXML_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "OPENGL_LIBRARIES = ${OPENGL_LIBRARIES}")
MESSAGE(STATUS "")
MESSAGE(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
MESSAGE(STATUS "")
MESSAGE(STATUS "GLEW_LIBRARY = ${GLEW_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "GLM_LIBRARY = ${GLM_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "CGAL_LIBRARY = ${CGAL_LIBRARY}")
MESSAGE(STATUS "")
MESSAGE(STATUS "=====================================")
MESSAGE(STATUS "")
MESSAGE(STATUS "")

ADD_EXECUTABLE( stippling 
    ${stippling_SOURCES}
	${stippling_HEADERS}
    ${stippling_HEADERS_MOC}
    ${stippling_FORMS_HEADERS}
    ${stippling_RESOURCES_RCC}
)
TARGET_LINK_LIBRARIES( stippling 
    ${QT_LIBRARIES}
    ${QT_QTGUI_LIBRARY}
    ${QT_CORE_LIBRARY}
    ${QT_QTOPENGL_LIBRARY}
    ${QT_QTXML_LIBRARY}
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBS}
    ${GLEW_LIBRARY}
    ${GLM_LIBRARY}
    ${CGAL_LIBRARY}
)



