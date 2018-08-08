# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(BetakammerSetup)

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


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# creates some folders for macros at the "At build system generation"
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros/unfolding)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros/calibration)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/macros/systematics)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Calibration)
file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Scripte)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(BetaChamberSetup BetaChamberSetup.cc ${sources} ${headers})
target_link_libraries(BetaChamberSetup ${Geant4_LIBRARIES})

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build BetaChamberSetup. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(BetaChamberSetup_SCRIPTS
  BetaChamberSetup.in
  BetaChamberSetup.out

  macros/AllCommands.mac
  macros/Kalibration.mac
  macros/LoadCs137.mac
  macros/LoadMikkoSi31Spektrum.mac
  macros/vis.mac
  macros/Decay0.mac
  macros/Decay0Si31.mac
  macros/Decay0AT.mac
  macros/DrawChamber.mac
  macros/init_vis.mac
  Calibration/Ba133.dat
  Calibration/Cs137.dat
  Calibration/Bi207.dat

  Scripte/RunUnfolding.c
  Scripte/RunUnfolding_Bismut.c
  Scripte/RunUnfolding_Si31.c
  Scripte/RunSystematicsSiliconThickness.c
  Scripte/RunSystematicsBismuthThickness.c
  Scripte/RunCalibration.c
  Scripte/Run50muSiMessung.c

  Scripte/ExtractUnfolding.c
  Scripte/ExtractUnfolding_Bismut.c
  Scripte/ExtractSystematicsSiliconThickness.c
  Scripte/ExtractSystematicsBismuthThickness.c
  Scripte/ExtractCalibration.c

)

foreach(_script ${BetaChamberSetup_SCRIPTS})
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
add_custom_target(BetakammerSetup DEPENDS BetaChamberSetup)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS BetaChamberSetup DESTINATION bin)
