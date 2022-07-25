add_executable(em_window_demo em_window_demo.cpp)
set_target_properties(em_window_demo PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF)

target_link_libraries(em_window_demo PRIVATE emerald)
