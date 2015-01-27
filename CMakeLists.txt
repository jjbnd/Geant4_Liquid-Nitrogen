#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(LiquidNitrogen)

set(targetname "LiquidNitrogen")

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

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

########################################################################################################
# include root reference => http://huiso.blogspot.kr/2013/05/how-to-embed-root-code-inside-geant4.html #
########################################################################################################

#############
# for geant4
set(libs)
list(APPEND libs ${Geant4_LIBRARIES})

#############
# for root
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
find_package(ROOT REQUIRED)
list(APPEND libs ${ROOT_LIBRARIES})
include_directories(${ROOT_INCLUDE_DIR})


#############

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${targetname} ${targetname}.cc ${sources} ${headers})
target_link_libraries(${targetname} ${libs})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
#set(LiquidNitrogen_SCRIPTS
#  LiquidNitrogen.in
#  LiquidNitrogen.out
#  init_vis.mac
#  run1.mac
#  run2.mac
#  vis.mac
#  )

set(LiquidNitrogen_SCRIPTS
  init_vis.mac
  vis.mac
  run1.mac
  show.c
  )

foreach(_script ${LiquidNitrogen_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
#add_custom_target(B1 DEPENDS exampleB1)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS LiquidNitrogen DESTINATION bin)