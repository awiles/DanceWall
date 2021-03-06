project(DanceWall)

cmake_minimum_required(VERSION 2.8.11)

# if we don't define it, let's assume debug.
if(NOT DEFINED CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Output directories.
SET(LIBRARY_OUTPUT_PATH ${DanceWall_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all libraries.")
SET(EXECUTABLE_OUTPUT_PATH ${DanceWall_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

# initialize the prefix path.
set(CMAKE_PREFIX_PATH)
# add the directory above to help search for related projects (e.g. openExternals)
list(APPEND CMAKE_PREFIX_PATH ${DanceWall_SOURCE_DIR}/..)

# set up search paths for cmake modules.
set(CMAKE_MODULE_PATH ${DanceWall_SOURCE_DIR}/cmake)

# set the QT_DIR and set up some other variables.
if(WIN32)
	# TODO:This works for Windows, MSVC 2012, 64 bits.  Need logic here based on the generator.
	# NOTE: this may not be the best way to handle this -- need to figure out the best path forward here.
    set(QT_DIR "C:\\Qt\\5.2.1\\msvc2012_64_opengl" CACHE PATH "Need to set the Qt prefix for various things.")
    set(CMAKE_LIBRARY_PATH "C:\\Program Files (x86)\\Windows Kits\\8.0\\Lib\\win8\\um\\x64" CACHE PATH "Qt5 requires this.")
	# Need wzunzip.exe for Boost download on windows.
elseif(APPLE)
    # assume QT is installed to /opt/Qt
    # need to use 5.4 with VTK 6.3 on Mac OSX Sierra.
    set(QT_DIR "/opt/Qt/5.4/clang_64" CACHE PATH "Need to set the Qt prefix for various things.")
else()
    set(QT_DIR "/opt/Qt/5.2.1/gcc_64" CACHE PATH "Need to set the Qt path in order to find it more easily." )
endif()
# update the prefixes.
list(APPEND CMAKE_PREFIX_PATH ${QT_DIR})

# Looking for openExternals
find_package(openExternals)
if(openExternals_FOUND)
	message(STATUS "Found openExternals: ${openExternals_DIR}")
	list( APPEND CMAKE_PREFIX_PATH ${openExternals_DIR} )
			
	#eigen
	set(DanceWall_EIGEN_DIR ${openExternals_EIGEN_DIR})
	list( APPEND CMAKE_PREFIX_PATH ${DanceWall_EIGEN_DIR} )
	
	#vtk
	set(DanceWall_VTK_DIR ${openExternals_VTK_DIR})
	list( APPEND CMAKE_PREFIX_PATH ${DanceWall_VTK_DIR} )

	#opencv
	set(DanceWall_OPENCV_DIR ${openExternals_OPENCV_DIR})
	list( APPEND CMAKE_PREFIX_PATH ${DanceWall_OPENCV_DIR} )
		#need the Haar Cascade files.
		configure_file(${openExternals_OPENCV_DIR}/share/OpenCV/haarcascades/haarcascade_frontalface_alt2.xml ${CMAKE_BINARY_DIR}/resources/haarcascade_frontalface_alt2.xml COPYONLY)
		
		configure_file(${openExternals_OPENCV_DIR}/share/OpenCV/haarcascades/haarcascade_eye_tree_eyeglasses.xml ${CMAKE_BINARY_DIR}/resources/haarcascade_eye_tree_eyeglasses.xml COPYONLY)
		
		configure_file(${CMAKE_SOURCE_DIR}/resources/haarcascade_palm.xml ${CMAKE_BINARY_DIR}/resources/haarcascade_palm.xml COPYONLY)
		
		configure_file(${CMAKE_SOURCE_DIR}/resources/haarcascade_closed_frontal_palm.xml ${CMAKE_BINARY_DIR}/resources/haarcascade_closed_frontal_palm.xml COPYONLY)
		
		configure_file(${CMAKE_SOURCE_DIR}/resources/haarcascade_fist.xml ${CMAKE_BINARY_DIR}/resources/haarcascade_fist.xml COPYONLY)
	
	# #boost
	# set(DanceWall_BOOST_DIR ${openExternals_BOOST_DIR})
	# message(STATUS "Boost dir: ${DanceWall_BOOST_DIR}")
	# set(BOOST_ROOT ${DanceWall_BOOST_DIR} )
	# set(BOOST_INCLUDEDIR ${BOOST_ROOT}/include )
	# set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib )
	# set(Boost_NO_SYSTEM_PATHS ON )
	# set(Boost_USE_STATIC_LIBS ON )
	
	# #flann
	# set(DanceWall_FLANN_INCLUDE_DIR ${openExternals_FLANN_INCLUDE_DIR})
	# set(DanceWall_FLANN_LIBRARY_DIR ${openExternals_FLANN_LIBRARY_DIR})
	# set(DanceWall_FLANN_LIBRARY ${openExternals_FLANN_LIBRARY})
	
	# #pcl
	# set(DanceWall_PCL_DIR ${openExternals_PCL_DIR})
	# list(APPEND CMAKE_PREFIX_PATH ${DanceWall_PCL_DIR})
	
	# #ctk
	# set(DanceWall_CTK_DIR ${openExternals_CTK_DIR})
	# list( APPEND CMAKE_PREFIX_PATH ${DanceWall_CTK_DIR} )
else()
	message(FATAL_ERROR "Cannot not find openExternals project.")
endif()

find_package(OpenCV REQUIRED)

########  DANCE WALL  ########
include_directories(${OpenCV_INCLUDE_DIRS})

set(DanceWall_SRCS
	DanceWall.cxx
	DWObject.cxx
	AbstractEffect.cxx
	CartoonEffect.cxx
	ColorEffect.cxx
	ColorMapEffect.cxx
	EdgeEffect.cxx
	HandTracker.cxx
	LineEffect.cxx
	MotionEffect.cxx
	MotionFlowEffect.cxx
	SimpleMotionTracker.cxx
	ThresholdEffect.cxx
	TrackingEffect.cxx
	WarpEffect.cxx
	main.cxx)
	
set(DanceWall_HDRS
	DanceWall.h
	DWObject.h
	AbstractEffect.h
	CartoonEffect.h
	ColorEffect.h
	ColorMapEffect.h
	EdgeEffect.h 
	HandTracker.h
	LineEffect.h
	MotionEffect.h
	MotionFlowEffect.h
	SimpleMotionTracker.h
	ThresholdEffect.h
	TrackingEffect.h
	Warpeffect.h)

add_executable(DanceWall ${DanceWall_SRCS} ${DanceWall_HDRS})
target_link_libraries(DanceWall ${OpenCV_LIBS})

# set up the windows batch file for setting up the correct runtime paths.
# TODO: build a script for VS2010, VS2012 and VS2013.  Also flip between 32 and 64 bits when needed.
if(WIN32)
	if(MSVC11 AND CMAKE_CL_64 )
		set(BUILD_TYPE "Debug")
		configure_file(
			${CMAKE_SOURCE_DIR}/cmake/DanceWall-VS2012-64-dev.bat.in
			${CMAKE_BINARY_DIR}/../DanceWall-VS2012-64-devDebug.bat )
		set(BUILD_TYPE "Release")
		configure_file(
			${CMAKE_SOURCE_DIR}/cmake/DanceWall-VS2012-64-dev.bat.in
			${CMAKE_BINARY_DIR}/../DanceWall-VS2012-64-devRelease.bat )
	else()
		message(WARNING "No launch batch files created for this IDE environment.")
	endif()
elseif(APPLE)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/DanceWall-QtCreator-OSX-dev.sh.in
        ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DanceWall-QtCreator-OSX-dev.command )
    file(COPY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DanceWall-QtCreator-OSX-dev.command DESTINATION ${CMAKE_BINARY_DIR}/../ FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(WIN32)
