#for debug : cmake -DCMAKE_BUILD_TYPE=Debug /path/to/source

cmake_minimum_required(VERSION 2.6)
project(sketch_ui)

find_package(Qt4 REQUIRED)
include_directories("${PROJECT_BINARY_DIR}" ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR}  includes includes/Qtfiles)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
	# Needed for the moc
	set(MapMakerv3_SRCS main.cpp includes/Qtfiles/MapMakerv3.cpp includes/Qtfiles/imageLabel.cpp includes/Qtfiles/MapMakerExpert.cpp includes/Qtfiles/MapMakerExpertWindows.cpp)
		
	set(CMAKE_AUTOMOC TRUE)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	add_library(UI_libs
			includes/Qtfiles/MapMakerv3.cpp 
			includes/Qtfiles/imageLabel.cpp 
			includes/Qtfiles/MapMakerExpert.cpp
			includes/Qtfiles/MapMakerExpertWindows.cpp)
else ()
	# Needed for the moc
	set(MapMakerv3_SRCS main.cpp includes/Qtfiles/MapMakerv3.cpp includes/Qtfiles/imageLabel.cpp includes/Qtfiles/MapMakerExpert.cpp)
		
	set(CMAKE_AUTOMOC TRUE)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	add_library(UI_libs
			includes/Qtfiles/MapMakerv3.cpp 
			includes/Qtfiles/imageLabel.cpp 
			includes/Qtfiles/MapMakerExpert.cpp)
	
endif ()

add_executable(mapmakerv3 main.cpp)
target_link_libraries(mapmakerv3 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} UI_libs)

add_executable(ui_expert ui_expert.cpp)
target_link_libraries(ui_expert ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} UI_libs)

if (CMAKE_SYSTEM_NAME MATCHES Windows)
	add_executable(ui_windows ui_windows.cpp)
	target_link_libraries(ui_windows ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} UI_libs)
endif ()

install(TARGETS mapmakerv3 RUNTIME DESTINATION bin)
add_subdirectory(includes)
