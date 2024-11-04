cmake_minimum_required(VERSION 3.11.2)

project(POCSAG-Discord-Bot VERSION 1.0 DESCRIPTION "A Discord Bot to track POCSAG msgs")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#create executable
add_executable(${PROJECT_NAME}
	src/discord/main.cpp
)

# find DPP package
find_package(DPP REQUIRED)

# Link DPP package
target_link_libraries(${PROJECT_NAME}
	${DPP_LIBRARIES}
)

# Include DPP dirs
target_include_directories(${PROJECT_NAME} PRIVATE
	${DPP_INCLUDE_DIR}
)

# Set C++ Version
set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED OFF
)
