cmake_minimum_required(VERSION 3.13)
project(kcsdr_info)

# Get source files
file(GLOB_RECURSE SRC "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SRC})

# Link to librfnm
target_link_libraries(${PROJECT_NAME} PRIVATE kcsdr)

# Create install directive
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})