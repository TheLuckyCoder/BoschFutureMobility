#include <functional>
#include <iostream>
#include <optional>

#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `BoschFutureMobility`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>


int main(int argc, const char **argv)
{
	try {
		// Use the default logger (stdout, multi-threaded, colored)
		spdlog::info("Hello, {}!", "World");

	} catch (const std::exception &e) {
		spdlog::error("Unhandled exception in main: {}", e.what());
	}
}
