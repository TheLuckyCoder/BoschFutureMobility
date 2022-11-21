find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# Generic test that uses conan libs
add_executable(robot main.cpp)
target_link_libraries(
  robot
  PUBLIC project_options project_warnings
  PRIVATE fmt::fmt spdlog::spdlog)

target_include_directories(robot PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
