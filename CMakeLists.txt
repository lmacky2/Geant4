#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(MuogenicTCNs)

#----------------------------------------------------------------------------

# Find Geant4 package, activating all available UI and Vis drivers by default

find_package(Geant4 REQUIRED COMPONENTS ui_all vis_all tools)


#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------

# Locate sources and headers for this project

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(TCNProduction ${sources} ${headers})
target_link_libraries(TCNProduction PRIVATE ${Geant4_LIBRARIES})
target_include_directories(TCNProduction PRIVATE ${Geant4_INCLUDE_DIRS})
target_include_directories(TCNProduction PRIVATE "${PROJECT_SOURCE_DIR}/src")


#----------------------------------------------------------------------------

add_custom_target(MuogenicTCNs DEPENDS TCNProduction)
