#for debug : cmake -DCMAKE_BUILD_TYPE=Debug /path/to/source

cmake_minimum_required(VERSION 2.6)
project(sketch_node)

add_definitions(-std=c++11)


SET(CMAKE_CXX_FLAGS "-Wall")

# Version for all the file in debug have TIMED defined in debug mode
SET(CMAKE_CXX_FLAGS_DEBUG  "-DDEBUG -g")
#Version for the same thing but for one target
# target_compile_definitions(Probabilitieslib PUBLIC $<$<CONFIG:Debug>:TIMED>)

find_package( OpenCV REQUIRED)

find_package( Boost REQUIRED )

find_package( editdistance REQUIRED )
find_package( vodigrex REQUIRED )
find_package( bettergraph REQUIRED )
find_package( sketchAlgorithm REQUIRED )

if ( NOT vodigrex_FOUND )
  message(STATUS "This project requires the VODIGREX library, and will not be compiled.")
  return()  
endif()

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()  
endif()


# //I don't think those are needed because they are in usr/include
# include_directories(${editdistance_INCLUDE_DIRS})
# include_directories(${vodigrex_INCLUDE_DIRS})

# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
# install(TARGETS editdistance RUNTIME DESTINATION bin)


include_directories("${PROJECT_BINARY_DIR}" ${CMAKE_CURRENT_BINARY_DIR} ${vodigrex_INCLUDE_DIRS} ${sketchAlgorithm_INCLUDE_DIRS}
	includes includes/MapInterpretation/ 
	Test/Ddata/ 
)


add_library(Maplib 
			includes/MapInterpretation/SketchMap.cpp
)		
target_link_libraries(Maplib ${OpenCV_LIBS} ${vodigrex_LIBRARIES} )



add_executable(main main.cpp)
target_link_libraries(main ${OpenCV_LIBS} ${sketchAlgorithm_LIBRARIES} Maplib -lboost_filesystem -lboost_system)

# add_executable(main_windows main_windows.cpp)
# target_link_libraries(main_windows ${OpenCV_LIBS} ${sketchAlgorithm_LIBRARIES} Maplib -lboost_filesystem -lboost_system)

add_subdirectory(includes)