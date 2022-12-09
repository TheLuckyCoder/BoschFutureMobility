#pragma once

#include <asio.hpp>
#include <thread>

#include "ResponseHandler.h"
#include "SerialPortHandler.h"

namespace serial {

class SerialCommunicationManager
{
  public:
	SerialCommunicationManager(unsigned int baud_rate,
	  const std::string &device,
	  ResponseHandler &response_handler);
	explicit SerialCommunicationManager(ResponseHandler &response_handler);
	SerialCommunicationManager(const SerialCommunicationManager &) = delete;
	SerialCommunicationManager(SerialCommunicationManager &&) = delete;
	SerialCommunicationManager &operator=(const SerialCommunicationManager &) = delete;
	SerialCommunicationManager &operator=(SerialCommunicationManager &&) = delete;
	~SerialCommunicationManager();

	void send(const Message &message);

  private:
	ResponseHandler &response_handler_;
	asio::io_service io_service_{};
	SerialPortHandler serial_port_;
	std::thread io_service_thread_;
	std::thread response_handler_thread_;
};

}// namespace serial
