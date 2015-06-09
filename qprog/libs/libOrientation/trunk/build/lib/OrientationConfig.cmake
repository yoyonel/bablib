# OrientationConfig.cmake: Configuration file to use Orientation
# Authors: Nicolas David and Bruno Vallet
# Usage: in your CMake file:
#include(MatisMacros)
#__default_install_configuration()
#find_package(Orientation) # if this does not work, try FIND_PACKAGE(Orientation NO_MODULE)
#if(Orientation_FOUND)
#	include_directories(${Orientation_INCLUDE_DIRS})
#	add_definitions(${Orientation_DEFINITIONS})
#else(Orientation_FOUND)
#	message(FATAL_ERROR "Orientation NOT found ! This library can be found at ")
#endif(Orientation_FOUND)
# and after you added your_target depending on Orientation:
#target_link_libraries(your_target ${Orientation_LIBRARIES})

get_filename_component(Orientation_SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${Orientation_SELF_DIR}/Orientation-targets.cmake OPTIONAL)

set(Orientation_DEFINITIONS )
set(Orientation_INCLUDE_DIR "C:/Program Files/MATIS/include/Orientation;C:/Program Files/MATIS/include/Orientation/config")
set(Orientation_INCLUDE_DIRS "C:/Program Files/MATIS/include/Orientation;C:/Program Files/MATIS/include/Orientation/config")
set(Orientation_LIBRARIES "Orientation;tinyxml")
set(Orientation_LIBRARY "")

include_directories("C:/Program Files/MATIS/include")
link_directories(${Orientation_SELF_DIR}/../lib)

message(STATUS "Found Orientation in MATIS/Extern using ${Orientation_SELF_DIR}/OrientationConfig.cmake")
