cmake_minimum_required(VERSION 2.6)

set (PROJECT_NAME RTiOW)
project(${PROJECT_NAME})

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories("include")
file(GLOB_RECURSE SOURCES "src/*.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

