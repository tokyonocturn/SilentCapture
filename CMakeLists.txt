cmake_minimum_required(VERSION 3.22...3.26)

project(silent-capture VERSION 1.0.0)

add_library(silent-capture MODULE
	src/notification-hider.cpp
)

target_link_libraries(silent-capture PRIVATE
	OBS::libobs
	OBS::frontend-api
)

set_target_properties(silent-capture PROPERTIES PREFIX "")

target_compile_features(silent-capture PRIVATE cxx_std_17)
