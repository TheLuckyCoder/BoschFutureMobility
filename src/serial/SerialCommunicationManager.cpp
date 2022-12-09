#include "SerialCommunicationManager.h"

namespace serial {

constexpr auto DEFAULT_BAUD_RATE = 19200;

SerialCommunicationManager::SerialCommunicationManager(const unsigned int baud_rate,
  const std::string &device,
  ResponseHandler &response_handler)
  : response_handler_(response_handler),
	serial_port_(io_service_, baud_rate, device, response_handler_),
	io_service_thread_([&] { io_service_.run(); }),
	response_handler_thread_([&] { response_handler_.run(); })
{
}

SerialCommunicationManager::SerialCommunicationManager(ResponseHandler &response_handler)
  : SerialCommunicationManager(DEFAULT_BAUD_RATE, "/dev/ttyACM0", response_handler)
{}

SerialCommunicationManager::~SerialCommunicationManager()
{
	response_handler_.deactivate();
	io_service_thread_.join();
	response_handler_thread_.join();
}

void SerialCommunicationManager::send(const Message &message)
{
	serial_port_.write(message.serial_message);
}

}// namespace serial
