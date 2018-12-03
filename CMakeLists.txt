cmake_minimum_required(VERSION 2.6)

set (PROJECT_NAME pathtracer)
project(${PROJECT_NAME})

include(ExternalProject)
ExternalProject_Add(
  cxxopts
  URL https://github.com/jarro2783/cxxopts/archive/v2.1.1.zip
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/cxxopts
  INSTALL_COMMAND "")
ExternalProject_Get_Property(cxxopts SOURCE_DIR)
set (CXXOPTS_SOURCE_DIR ${SOURCE_DIR})
  
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories("include" "${CXXOPTS_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  LINKER_LANGUAGE CXX
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

