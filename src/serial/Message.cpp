#include "Message.h"

#include <fmt/core.h>
#include <utility>


// Backported from C++23
template<class Enum> constexpr std::underlying_type_t<Enum> to_underlying(Enum enumeration) noexcept
{
	return static_cast<std::underlying_type_t<Enum>>(enumeration);
}

namespace serial {

/**
 * @name    speed
 * @brief
 *
 * Construct the string to be sent, associated to speed action.
 *
 * @param [in] velocity  Velocity.
 *
 * @retval Complete string for send command.
 *
 * Example Usage:
 * @code
 *    speed(1.234);
 * @endcode
 */
Message Message::speed(const float velocity) noexcept
{
	return { fmt::format("#{}:{:.2f};;\r\n", to_underlying(Action::SPEED), velocity) };
}

/**
 * @name    steering
 * @brief
 *
 * Construct the string to be sent, associated to steering action.
 *
 * @param [in] angle     Angle.
 *
 * @retval Complete string for send command.
 *
 * Example Usage:
 * @code
 *    steering(5.678);
 * @endcode
 */
Message Message::steer(const float angle) noexcept
{
	return { fmt::format("#{}:{:.2f};;\r\n", to_underlying(Action::STEERING), angle) };
}

/**
 * @name    brake
 * @brief
 *
 * Provide the cmd key associated to an action.
 *
 * @param [in] angle     Angle.
 *
 * Construct the string to be sent, associated to an action.
 *
 * Example Usage:
 * @code
 *    brake(1.234);
 * @endcode
 */
Message Message::brake(const float angle) noexcept
{
	return { fmt::format("#{}:{:.2f};;\r\n", to_underlying(Action::BRAKE), angle) };
}

/**
 * @name    enablePid
 * @brief
 *
 * Construct the string to be sent, associated to pid activating.
 *
 * @param [in] enable     Set PID active or not.
 *
 * @retval Complete string for send command.
 *
 * Example Usage:
 * @code
 *    enablePid(true);
 * @endcode
 */
Message Message::enablePid(const bool enable) noexcept
{
	return { fmt::format("#{}:{:d};;\r\n", to_underlying(Action::PID_ACTIVATION), enable) };
}

/**
 * @name    enableEncoderPublisher
 * @brief
 *
 * Construct the string to be sent, associated to encoder publisher activating.
 *
 * @param [in] enable     Set ENPB active or not.
 *
 * @retval Complete string for send command.
 *
 * Example Usage:
 * @code
 *    enableEncoderPublisher(true);
 * @endcode
 */
Message Message::enableEncoderPublisher(const bool enable) noexcept
{
	return { fmt::format("#{}:{:d};;\r\n", to_underlying(Action::ENCODER_PUBLISHER), enable) };
}


/**
 * @name    pidParameters
 * @brief
 *
 * Construct the string to be sent, associated to pid settig.
 *
 * @param [in] kProportional          Param kProportional.
 * @param [in] kIntegral          Param kIntegral.
 * @param [in] kDerivative          Param kDerivative.
 * @param [in] kFeedforward          Param kFeedforward.
 *
 * @retval Complete string for send command.
 *
 * Example Usage:
 * @code
 *    pidParameters(1.234567,7.654321,12.12121212,3.4567890);
 * @endcode
 */
Message Message::pidParameters(const float kProportional,
  const float kIntegral,
  const float kDerivative,
  const float kFeedforward) noexcept
{
	return { fmt::format("#{}:{:.5f};{:.5f};{:.5f};{:.5f};;\r\n",
	  to_underlying(Action::PID_TUNING),
	  kProportional,
	  kIntegral,
	  kDerivative,
	  kFeedforward) };
}

}// namespace serial