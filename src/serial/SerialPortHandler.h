#pragma once

#include "ResponseHandler.h"
#include <asio.hpp>
#include <iostream>
#include <string>

namespace serial {

/// @brief Different ways a serial port may be flushed.
enum flush_type { flush_receive = TCIFLUSH, flush_send = TCOFLUSH, flush_both = TCIOFLUSH };

class SerialPortHandler
{
  public:
	SerialPortHandler(asio::io_service &io_service,
	  unsigned int baud,
	  const std::string &device,
	  ResponseHandler &responseHandler);
	~SerialPortHandler();

	void write(const std::string &message);

  private:
	// maximum amount of data to read in one operation
	static constexpr int MAX_READ_LENGTH = 512;

	void readStart();
	void readComplete(const std::error_code &error, size_t bytes_transferred);
	void callbackWriteString(const std::string &message);
	void writeStart();
	void writeComplete(const std::error_code &error);
	void closeSocket(const std::error_code &error);
	void flushSerialPort(flush_type what, std::error_code &error);

	ResponseHandler &response_handler_;// object response handler processes the response received
	std::mutex write_mutex;// object avoids the parallel writing

	asio::io_service &io_service_;// the main IO service that runs this connection
	asio::serial_port serial_port_;// the serial port this instance is connected to
	std::deque<char> write_msgs_;// buffered write data
  public:
	std::array<char, MAX_READ_LENGTH> read_msg_{};// data read from the socket
};

}// namespace serial
