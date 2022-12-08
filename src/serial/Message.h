#pragma once

#include <string>

namespace serial {


// enum for defining the actions that can be performed
enum class Action : uint8_t {
	SPEED = 1,
	STEERING = 2,
	BRAKE = 3,
	PID_ACTIVATION = 4,
	ENCODER_PUBLISHER = 5,
	PID_TUNING = 6,
	NONE = 7
};

struct Message
{
	std::string serial_message;

	static Message speed(float velocity) noexcept;
	static Message steer(float angle) noexcept;
	static Message brake(float angle) noexcept;
	static Message enablePid(bool enable) noexcept;
	static Message enableEncoderPublisher(bool enable) noexcept;
	static Message pidParameters(float kProportional, float kIntegral, float kDerivative, float kFeedforward) noexcept;
};

}// namespace serial
