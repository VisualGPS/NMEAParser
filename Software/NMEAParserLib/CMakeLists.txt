cmake_minimum_required(VERSION 2.8)

#
# About the project
#
project(NMEAParserLib)


if(MSVC)
	# Do nothing for now...
else()
        set(CMAKE_CXX_FLAGS " -Wall")
endif ()

FIND_PACKAGE(Git)

execute_process(COMMAND git describe --abbrev=4 HEAD
               COMMAND sed -e "s/-/./g"
               OUTPUT_VARIABLE GIT_VERSION
               OUTPUT_STRIP_TRAILING_WHITESPACE)

#
# Add subdirectories
#
#add_subdirectory(none)

#
# Add executable name and project files
#
add_library(NMEAParserLib
	DoxygenMainPage.md
    NMEAParserPacket.cpp
    NMEAParserPacket.h
    NMEAParser.cpp
    NMEAParser.h
    NMEAParserData.h
	NMEASentenceBase.cpp
	NMEASentenceBase.h
	NMEASentenceGGA.cpp
	NMEASentenceGGA.h
	NMEASentenceGSV.cpp
	NMEASentenceGSV.h
	NMEASentenceGSA.cpp
	NMEASentenceGSA.h
	NMEASentenceRMC.cpp
	NMEASentenceRMC.h
)

#
# Add additional libraries
#
#target_link_libraries(rt pthread)

# add a target to generate API documentation with Doxygen
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(WARNING "Doxygen is needed to build the documentation. Skipping this step. Install Doxygen if you want html source documentation.")
    else()

		set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/DoxyfileSetup.txt)
		set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

		configure_file(${doxyfile_in} ${doxyfile} @ONLY)

		add_custom_target(doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating API documentation with Doxygen"
			VERBATIM)

		install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
    endif()
endif()
