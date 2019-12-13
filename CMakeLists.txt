cmake_minimum_required( VERSION 3.5 )

# Options can be turned on and off by providing cmake with a -DOPTIONNAME=ON (or OFF) argument.
# For example, to turn off benchmarks, and use sanitize flags in a DEBUG build:
#    cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DBENCH=OFF -DSANITIZE=ON

# Add test files with filenames <NAME>_test.cpp or <NAME>_test.c into test dir
# Add bench files with filenames <NAME>_bench.cpp or <NAME>_bench.c into test dir

# Override install prefix with CMAKE_INSTALL_PREFIX
# Override bin dir with BINDIR
# Override lib dir with LIBDIR

# Put tests and benchmarks into DIR_TESTS dirs
# Tests by glob *_test.cpp *_test.c
# Benchmarks by glob *_bench.cpp *_bench.c

option(TEST      "Build tests" ON)
option(BENCH     "Build benchmarks" OFF)
option(SANITIZE  "Adds sanitize flags" OFF)
option(TSANITIZE  "Adds thread sanitize flags" OFF)

if(SANITIZE)
  set(TSANITIZE OFF)
endif()

if(BENCH)
  set(TEST ON)
endif()

set( DIR_SOURCES src )
set( DIR_INCLUDES include )
set( DIR_TESTS test )
#set( DIR_TESTS_INTEGRATION test_integration )
set( DIR_TESTS_TOOLS tools )
set( DIR_DEP dep )
set( DIR_SCRIPT cmake )

if (NOT DEFINED BINDIR)
set( BINDIR bin )
endif()
if (NOT DEFINED LIBDIR)
set( LIBDIR lib )
endif()

set ( PROJECT cqueue )
set ( LIBRARY ${PROJECT} )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project( ${PROJECT} )

include( version.cmake )

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#set LIBRARIES (
#)


# Initialize conan
#INCLUDE("conan.cmake")

# Include cmake script from cmake dir
file(GLOB cmakeFiles ${DIR_SCRIPT}/*.cmake)
foreach(cmakeFile ${cmakeFiles})
    message("INCLUDE ${cmakeFile}")
    INCLUDE(${cmakeFile})
endforeach(cmakeFile)

if ( DEFINED DIR_INCLUDES )
    # Includes in separate directory
    include_directories( ${DIR_INCLUDES} )
endif()

#Scan dir for standart source files
aux_source_directory( ${DIR_SOURCES} SOURCES )

set (PUBLIC_HEADERS
	${DIR_INCLUDES}/example.h
)


#Add sources from dir
#set( SOURCES
#    ${DIR_SOURCES}/main.cpp
#    ${DIR_SOURCES}/palindrom.cpp
#)

# Add library target
# this is the "object library" target: compiles the sources only once
add_library( ${LIBRARY}_objlib OBJECT ${SOURCES} )
target_include_directories( ${LIBRARY}_objlib PRIVATE ${DIR_INCLUDES} )
# shared libraries need PIC
set_property( TARGET ${LIBRARY}_objlib PROPERTY POSITION_INDEPENDENT_CODE 1 )
# build shared library
add_library( ${LIBRARY} SHARED $<TARGET_OBJECTS:${LIBRARY}_objlib> )
set ("${LIBRARY}_VERSION" "${VERSION_MAJOR}.${VERSION_MINOR}" )
set_target_properties(${LIBRARY} PROPERTIES VERSION "${${LIBRARY}_VERSION}" SOVERSION "${VERSION_MAJOR}")
# build static library
add_library( ${LIBRARY}_s STATIC $<TARGET_OBJECTS:${LIBRARY}_objlib> )
if(LIBRARIES)
	target_link_libraries ( ${LIBRARY} ${LIBRARIES} )
	target_link_libraries ( ${LIBRARY}_s ${LIBRARIES} )
endif()

if(DEFINED ENV{DEVENV})
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

INSTALL( TARGETS ${LIBRARY} DESTINATION ${LIBDIR} )

INSTALL (
    FILES
    ${PUBLIC_HEADERS}
    DESTINATION
    include/${PROJECT}
)

enable_testing()

if (TEST)

if ( DEFINED DIR_TESTS )
	#set enable testing
	foreach ( dir IN LISTS DIR_TESTS )
		message("add test ${dir}")
		add_subdirectory( ${dir} )
	endforeach()
endif()

if ( DEFINED DIR_TESTS_INTEGRATION )
	#set enable testing
	foreach ( dir IN LISTS DIR_TESTS_INTEGRATION )
		message("add integration test ${dir}")
		add_subdirectory( ${dir} )
	endforeach()
endif()

if ( DEFINED DIR_TESTS_TOOLS )
	#set enable testing
	foreach ( dir IN LISTS DIR_TESTS_TOOLS )
		message("add tools from ${dir}")
		file(GLOB cmakeFiles ${dir}/*.cmake)
		foreach(cmakeFile ${cmakeFiles})
			message("INCLUDE ${cmakeFile}")
			INCLUDE(${cmakeFile})
		endforeach(cmakeFile)
	endforeach()
endif()

endif() # END TEST


message(STATUS "")
message(STATUS "BUILD SUMMARY")
message(STATUS "  CMAKE_GENERATOR       : ${CMAKE_GENERATOR}")
message(STATUS "  Compiler ID           : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  Build type            : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build tests           : ${TEST}")
message(STATUS "  Build benchmarks      : ${BENCH}")
message(STATUS "  Sanitize flags        : ${SANITIZE}")
message(STATUS "  Thread Sanitize flags : ${TSANITIZE}")
message(STATUS "")
message(STATUS "  Install prefix : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Binary dir     : ${BINDIR}")
message(STATUS "  Lib dir        : ${LIBDIR}")
message(STATUS "")
