find_package(GTest REQUIRED)

#Scan source dir for standart source files and exclude main.cpp
aux_source_directory( ../${DIR_SOURCES} SOURCES_TEST )
list(REMOVE_ITEM SOURCES_TEST "../${DIR_SOURCES}/main.cpp")

#Add sources from dir
#set( SOURCES
#    ../${DIR_SOURCES}/rational.cpp
#)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
	../${DIR_INCLUDES}
	${GTEST_INCLUDE_DIRS}
)

set( TEST_LIBRARIES
	${GTEST_LIBRARIES}
	Threads::Threads
)

if ( SOURCES_TEST )
	add_library( test${PROJECT} STATIC ${SOURCES_TEST} )
	list( APPEND TEST_LIBRARIES test${PROJECT} )
endif()

# Build tests
file(GLOB testFiles *_test.cpp)
foreach(testFile ${testFiles})
	file(RELATIVE_PATH test_file ${CMAKE_CURRENT_SOURCE_DIR} ${testFile})
	string(REGEX REPLACE "^([0-9a-zA-Z_\\-]+)_test\.c.*$" "\\1" test "${test_file}")
	add_executable( test_${test} ${test_file} )
	target_link_libraries ( test_${test} ${TEST_LIBRARIES} )
	add_test(
		NAME test_${test}
		COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${test}
	)
endforeach(testFile)

#foreach( t IN LISTS TESTS )
#add_executable( test_${t} test_${t}.cpp )
#target_link_libraries ( test_${t} ${LIBRARIES} )
#add_test(
   #NAME test_${t}
   #COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_${t}
#)
#endforeach(t)

# Build benchmarks
if ( BENCH )
set( BENCH_LIBRARIES
        #benchmark
		Threads::Threads
)

if ( SOURCES_TEST )
#       add_library( test${PROJECT} STATIC ${SOURCES_TEST} )
	list( APPEND BENCH_LIBRARIES test${PROJECT} )
endif()

file(GLOB testFiles *_bench.cpp)
foreach(testFile ${testFiles})
	file(RELATIVE_PATH test_file ${CMAKE_CURRENT_SOURCE_DIR} ${testFile})
	string(REGEX REPLACE "^([0-9a-zA-Z_\\-]+)_bench\.c.*$" "\\1" test "${test_file}")
	add_executable( bench_${test} ${test_file} ${SOURCES_TEST} )
	target_link_libraries ( bench_${test} ${BENCH_LIBRARIES} )
	add_bench(
		bench_${test}
		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bench_${test}
	)
endforeach(testFile)
endif() # BENCH
