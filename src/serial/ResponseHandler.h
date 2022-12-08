#pragma once

#include <deque>
#include <functional>
#include <map>
#include <span>
#include <vector>

#include "Message.h"

namespace serial {

/*
 * This class handles the receiving of the messages from the Embedded via serial.
 */
class ResponseHandler
{
  public:
	using CallbackType = std::function<void(std::string)>;
	using CallbackContainer = std::vector<CallbackType>;

	void operator()(std::span<char> buffer);
	void run();

	void attach(Action action, const CallbackType &callback);
//	void detach(Action action, const CallbackType&);
	void deactivate() noexcept;

  private:
	void processChar(const char received_char);
	void checkResponse() const;

	bool receiving_response_{ false };// is true, when receiving a response valid from the device
	bool active_{ true };// remain true, while the run executing
	std::deque<char> read_messages_;// buffered read data
	std::deque<char> valid_response_;// buffer the valid response
	std::map<Action, CallbackContainer> key_callback_func_map_;
};

}// namespace serial
