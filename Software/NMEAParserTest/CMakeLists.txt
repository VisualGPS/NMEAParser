cmake_minimum_required(VERSION 2.8)

#
# About the project
#
project(NMEAParserTest)

if(MSVC)
	# Disable MSVC warnings that suggest making code non-portable.  
	set(CMAKE_CXX_FLAGS "/wd4996 ${CMAKE_CXX_FLAGS}" CACHE STRING "" FORCE) 
else()
	set(CMAKE_CXX_FLAGS " -Wall")
endif ()

#
# Add subdirectories
#
#add_subdirectory(none)

#
# Include directories
#
include_directories(
    ../NMEAParserLib
)

#
# Add executable name and project files
#
add_executable(NMEAParserTest
	main.cpp
	
)

#
# Add additional libraries
#
target_link_libraries(NMEAParserTest NMEAParserLib)


