# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(QED)

# Find Geant4 package, activating all available UI and Vis drivers by default
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /opt/local/libexec/root5/etc/root/cmake)
find_package(ROOT REQUIRED)


# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS} /Users/eg415/CADMesh-1.1/install/include)

# Locate sources and headers for this project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, and link it to the Geant4 libraries
add_executable(QED QED.cc ${sources} ${headers})
target_link_libraries(QED ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(QED /Users/eg415/CADMesh-1.1/install/lib/libcadmesh.dylib)
