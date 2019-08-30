cmake_minimum_required(VERSION 3.14)
project(PQuantum VERSION 0.1.0.0)

set(CMAKE_VERBOSE_MAKEFILE YES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
-Wall -pedantic -Wextra")

if (CMAKE_USE_LIBCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif ()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS NO)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS log program_options REQUIRED)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

set(${PROJECT_NAME}_BUILD_TESTS YES)
set(${PROJECT_NAME}_BUILD_EXAMPLES NO)

file(TO_NATIVE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/examples ${PROJECT_NAME}_EXAMPLES_DIR)

configure_file(config.cmake include/config.hpp)

include_directories(${${PROJECT_NAME}_BINARY_DIR}/include)
include_directories(src)

add_subdirectory(src)

if (${PROJECT_NAME}_BUILD_TESTS)
	enable_testing()
	add_subdirectory(test)
endif ()

if (${PROJECT_NAME}_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif ()

