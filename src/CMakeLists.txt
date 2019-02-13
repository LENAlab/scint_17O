# $Id: CMakeLists.txt 68058 2013-03-13 14:47:43Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(SiDetector)

# General flags
SET( CMAKE_CXX_FLAGS "-std=c++11")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# Check for the Geant4 datasets
foreach(dsname ${Geant4_DATASETS})            # Iterate over dataset names
  if(NOT EXISTS ${Geant4_DATASET_${dsname}_PATH})  # Check existence
    message(WARNING "${dsname} not located at ${Geant4_DATASET_${dsname}_PATH}")
  endif()
endforeach()

# Look for whether Geant4 has multithreading enabled
find_package(Geant4 COMPONENTS multithreaded)
if(Geant4_multithreaded_FOUND)
  message("Geant4 was built with multithreading enabled!")
else()
  message("Multithreading not enabled. BOO!")
endif()

#----------------------------------------------------------------------
# Find the ROOT stuff
# Load some basic macros which are needed later on
include(FindROOT.cmake)

set(INCLUDE_DIRECTORIES
${ROOT_INCLUDE_DIR} 
)
include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 
link_directories( ${LINK_DIRECTORIES})

#----------------------------------------------------------------------
# Make the root dictionary for the data class.
set(DICTFILE Dict/RootOutputDataDICT.cxx)
ADD_CUSTOM_COMMAND(OUTPUT ${DICTFILE}
  COMMAND mkdir -p Dict
  COMMAND rootcint -f ${DICTFILE} -c  ${PROJECT_SOURCE_DIR}/include/RootOutputData.hh
  DEPENDS ${PROJECT_SOURCE_DIR}/include/RootOutputData.hh)
#EXECUTE_PROCESS(COMMAND mkdir Dict) 
#EXECUTE_PROCESS(COMMAND rootcint -f Dict/RootOutputDataDICT.cxx -c ${PROJECT_SOURCE_DIR}/include/RootOutputData.hh)
#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(SiDetector SiDetector.cc ${DICTFILE} ${sources} ${headers})
target_link_libraries(SiDetector ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(SCRIPTS
  macs
  analysis
  )
foreach(_script ${SCRIPTS})
  file(COPY
    ${_script}
    DESTINATION ${PROJECT_BINARY_DIR}
    )
endforeach()
#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(SiDetector DEPENDS SiDetector)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS SiDetector DESTINATION bin)


