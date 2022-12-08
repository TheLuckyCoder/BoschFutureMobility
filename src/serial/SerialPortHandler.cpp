#include "SerialPortHandler.h"

#include <spdlog/spdlog.h>

/*
 * This class handles the sending of the messages to the Embedded via serial and it forwards the
 * received messages to the ResponseHandler
 */

namespace serial {
/**
 * @name    Class constructor
 * @brief
 *
 * Open a serial port and perform flush for both receive and transmit buffers
 *
 * @param [in] io_service  Object that provides core I/O functionality.
 * @param [in] baud        Baud rate.
 * @param [in] device      Device name (listed in /dev folder on RPi)
 * @param [in] responseHandler   Object that provides response message processing
 *
 * @retval none
 *
 * Example Usage:
 * @code
 *    SerialPortHandler c(io_service, boost::lexical_cast<unsigned int>(argv[1]), argv[2]);
 * @endcode
 */
SerialPortHandler::SerialPortHandler(asio::io_service &io_service,
  unsigned int baud,
  const std::string &device,
  ResponseHandler &responseHandler)
  : response_handler_(responseHandler), io_service_(io_service),
	serial_port_(io_service, device)
{
	if (!serial_port_.is_open()) {
		spdlog::error("Failed to open serial port {}", device);
		return;
	} else {
		spdlog::debug("Serial port {} opened", device);
	}

	serial_port_.set_option(
	  asio::serial_port_base::baud_rate{ baud });// set the baud rate after the port has been opened

	std::error_code error;
	flushSerialPort(flush_both, error);
	std::cout << "flush: " << error.message() << std::endl;

	readStart();
}

SerialPortHandler::~SerialPortHandler()
{
	// call the doClose function via the io service in the other thread
	asio::post(io_service_, [this] { closeSocket(std::error_code{}); });
}

/**
 * @name    write
 * @brief   Send data over UART.
 *
 *  pass the write data to the do_write function via the io service in the other thread
 *
 * @param [in] smg  Message to be sent.
 *
 * @retval none.
 *
 * Example Usage:
 * @code
 *    c.write(msg);
 * @endcode
 */
void SerialPortHandler::write(const std::string &message)
{
	asio::post(io_service_, [this, message] { callbackWriteString(message); });
}

// Start an asynchronous read and call readComplete when it completes or fails
void SerialPortHandler::readStart()
{
	serial_port_.async_read_some(asio::buffer(read_msg_, MAX_READ_LENGTH),
	  std::bind(&SerialPortHandler::readComplete,// NOLINT(modernize-avoid-bind)
		this,
		std::placeholders::_1,
		std::placeholders::_2));
}

// the asynchronous read operation has now completed or failed and returned an error
void SerialPortHandler::readComplete(const std::error_code &error, size_t bytes_transferred)
{
	if (!error) {// read completed, so process the data
		response_handler_({ read_msg_.begin(), read_msg_.begin() + bytes_transferred });
		readStart();// start waiting for another asynchronous read again
	} else {
		closeSocket(error);
	}
}

// callback to handle write call from outside this class
void SerialPortHandler::callbackWriteString(const std::string &message)
{
	const std::lock_guard lock{ write_mutex };
	bool const write_in_progress = write_msgs_.empty();
	std::copy(message.begin(), message.end(), std::back_inserter(write_msgs_));
	if (write_in_progress) { writeStart(); }
}

// Start an asynchronous write and call writeComplete when it completes or fails
void SerialPortHandler::writeStart()
{
	asio::async_write(serial_port_,
	  asio::buffer(&write_msgs_.front(), 1),
	  std::bind(&SerialPortHandler::writeComplete,// NOLINT(modernize-avoid-bind)
		this,
		std::placeholders::_1));
}

// the asynchronous read operation has now completed or failed and returned an error
void SerialPortHandler::writeComplete(const std::error_code &error)
{
	if (!error) {// write completed, so send next write data
		write_msgs_.pop_front();// remove the completed data
	} else {
		closeSocket(error);
	}
}

// something has gone wrong, so close the socket & make this object inactive
void SerialPortHandler::closeSocket(const std::error_code &error)
{
	if (error == asio::error::operation_aborted) {// if this call is the result of a timer cancel()
		return;// ignore it because the connection cancelled the timer
	}
	if (error) { spdlog::error("Error: {}", error.message()); }
	serial_port_.close();
}
/// @brief Flush a serial port's buffers.
///
/// @param what Determines the buffers to flush.
/// @param error Set to indicate what error occurred, if any.
void SerialPortHandler::flushSerialPort(flush_type what, std::error_code &error)
{
	if (0 == ::tcflush(serial_port_.lowest_layer().native_handle(), what)) {
		error = {};
	} else {
		error = { errno, asio::error::get_system_category() };
	}
}

}// namespace serial