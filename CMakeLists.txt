cmake_minimum_required(VERSION 3.9)

project(emerald VERSION 1.0
	DESCRIPTION "Minimalistic Windowing Library")

add_library(emerald STATIC
	src/win/window.cpp)
set_target_properties(emerald PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF)

target_include_directories(emerald PUBLIC src)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
	message(WARNING "Detected a linux-y operating system. Support for this platform is incomplete.")
	target_compile_definitions(emerald PUBLIC -DELD_LINUX=1 -DELD_WIN=0)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
	message(WARNING "Detected Windows operating system. Support for this platform is incomplete.")
	target_compile_definitions(emerald PUBLIC -DELD_LINUX=0 -DELD_WIN=1)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
	message(FATAL_ERROR "Cannot build Emerald because the detected operating system was macOS, which is not supported at this time.")
else()
	message(FATAL_ERROR "Cannot build Emerald because the operating system running on this machine was not recognised.")
endif()

add_subdirectory(demo)
