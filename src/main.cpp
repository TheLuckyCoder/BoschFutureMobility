
#include <spdlog/spdlog.h>

// This file will be generated automatically when you run the CMake configuration step.
// It creates a namespace called `BoschFutureMobility`.
// You can modify the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

#include "serial/SerialCommunicationManager.h"

void print(const std::string &msg) { spdlog::info(msg); }

void testSerial()
{
	using namespace serial;

	// Create a resnponse handler object
	ResponseHandler response_handler;
	// Create a communication manager object
	SerialCommunicationManager communication_manager(response_handler);

	// Attach the callback function to the following messages.
	response_handler.attach(Action::STEERING, [](auto msg) { print(msg); });
	response_handler.attach(Action::SPEED, &print);
	response_handler.attach(Action::BRAKE, &print);
	response_handler.attach(Action::PID_ACTIVATION, &print);

	communication_manager.send(Message::speed(1.0F));
}

int main()
{
	try {
		testSerial();
	} catch (const std::exception &e) {
		spdlog::error("Unhandled exception in main: {}", e.what());
	}
}
