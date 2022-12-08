find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

# Generic test that uses conan libs
add_executable(robot
        main.cpp
        serial/Message.cpp
        serial/Message.h
        serial/ResponseHandler.cpp
        serial/ResponseHandler.h
        serial/ResponseHandler.cpp
        serial/SerialPortHandler.h
        serial/SerialPortHandler.cpp
        serial/SerialCommunicationManager.h
        serial/SerialCommunicationManager.cpp)
target_link_libraries(
        robot
        PUBLIC project_options project_warnings
        PRIVATE fmt::fmt spdlog::spdlog asio::asio)

target_include_directories(robot PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
