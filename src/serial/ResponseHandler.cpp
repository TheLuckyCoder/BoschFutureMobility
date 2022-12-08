#include "ResponseHandler.h"

#include <spdlog/spdlog.h>

namespace serial {

/**
 * @name    operator()
 * @brief   copy the characters from input buffer to the message buffer
 *
 * @param [in]  buffer              pointer to the buffer
 * @param [in]  bytes_transferred   nr. byte transferred
 *
 * @retval none.
 */
void ResponseHandler::operator()(const std::span<char> buffer)
{
	for (const auto &byte : buffer) { read_messages_.push_back(byte); }
}


/**
 * @name    run
 * @brief   while the actice_ is true, the run executing cyclically,
 *          read a character from the message buffer and sends it to the processor.
 *
 * @retval none.
 *
 * Example Usage:
 * @code
 *    boost::thread t(boost::bind(&ResponseHandler::run, &responseHandler)
 * @endcode
 */
void ResponseHandler::run()
{
	while (active_) {
		if (!read_messages_.empty()) {
			const char l_c = read_messages_.front();
			read_messages_.pop_front();
			processChar(l_c);
		}
	}
}

/**
 * @name    attach
 * @brief   Attach the callback function  to the response key word.
 * This callback function will be called automatically, when will be received the key word
 *          feedback from the Embedded. More functions can be attach  to the one key word.
 *
 * @retval new object
 *
 * Example Usage:
 * @code
 *     l_responseHandler.attach(message::KEY,l_callbackFncObj)
 * @endcode
 */
void ResponseHandler::attach(const Action action, const CallbackType &callback)
{
	if (key_callback_func_map_.contains(action)) {
		CallbackContainer *l_container = &(key_callback_func_map_.at(action));
		l_container->push_back(callback);
	} else {
		CallbackContainer l_container;
		l_container.push_back(callback);
		key_callback_func_map_[action] = l_container;
	}
}


/**
 * @name    detach
 * @brief   After applying detach  on a certain function and message.
 * The callback function will not be called anymore after applying this.
 *
 * @retval new object
 *
 * Example Usage:
 * @code
 *      l_responseHandler.attach(message::KEY,l_callbackFncObj)
 * @endcode
 */
/*void ResponseHandler::detach(const Action action, const CallbackType &waiter)
{
	if (key_callback_func_map_.contains(action)) {
		CallbackContainer *l_container = (&key_callback_func_map_[action]);
		const auto iter = std::find(l_container->begin(), l_container->end(), waiter);
		if (iter != l_container->end()) {
			l_container->erase(iter);
		} else {
			spdlog::warn("Not found!");
		}

	} else {
		spdlog::warn("Container is empty!");
	}
}*/


/**
 * @name    processChar
 * @brief   Each received character is sent to this function.
 * 			If the char is '@', it signales the begining of the response.
 *          If the character is new line ('/r'), it signales the end of the response.
 *          If there is any other character, it appends it to the valid response attribute. .
 *
 * @retval new object
 *
 * Example Usage:
 * @code
 *      l_responseHandler.attach(message::KEY,l_callbackFncObj)
 * @endcode
 */
void ResponseHandler::processChar(const char received_char)
{
	if (received_char == '@') {
		receiving_response_ = true;
	} else if (received_char == '\r') {
		if (!valid_response_.empty()) {
			checkResponse();
			valid_response_.clear();
		}
		receiving_response_ = false;
	}

	if (receiving_response_) { valid_response_.push_back(received_char); }
}

/*
 * Response example: "@1:RESPONSECONTANT;;\r\n"
 */
void ResponseHandler::checkResponse() const
{
	const std::string full_response(valid_response_.begin(), valid_response_.end());
	const auto key = full_response.substr(1, 1);
	const auto response = full_response.substr(3, full_response.length() - 5);

	const auto action = std::stoi(key);
	if (action > 0) {
		const auto container = key_callback_func_map_.at(static_cast<Action>(action));
		for (const auto &element : container) { element(response); }
	}
}
void ResponseHandler::deactivate() noexcept { active_ = false; }

}// namespace serial
