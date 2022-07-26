cmake_minimum_required(VERSION 3.21)

project(emerald VERSION 1.0
	DESCRIPTION "Minimalistic Windowing Library")
include(cmake/setup.cmake)
add_subdirectory(lib)

hdk_add_library(
	TARGET emerald
	SOURCES
		src/eld/api/context.hpp
		src/eld/api/draw.hpp
		src/eld/api/window.hpp
		src/eld/win/context.cpp
		src/eld/win/context.hpp
		src/eld/win/window.cpp
		src/eld/win/window.hpp
		src/eld/context.hpp
		src/eld/draw.cpp
		src/eld/draw.hpp
		src/eld/software_renderer.cpp
		src/eld/software_renderer.hpp
		src/eld/window.hpp
	)

target_include_directories(emerald PUBLIC src)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	configure_linux(emerald)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
	configure_win32(emerald)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	message(FATAL_ERROR "Cannot build Emerald because the detected operating system was macOS, which is not supported at this time.")
else()
	message(FATAL_ERROR "Cannot build Emerald because the operating system running on this machine was not recognised.")
endif()

if(${EMERALD_BUILDCONFIG} MATCHES "debug")
	configure_debug(emerald)
elseif(${EMERALD_BUILDCONFIG} MATCHES "release")
	configure_release(emerald)
endif()

add_subdirectory(demo)
