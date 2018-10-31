/*!
	@file      Pin.h
	@brief     Management namespace of pin mapping.
	@author    Kazuyuki TAKASE
	@copyright The MIT License - http://opensource.org/licenses/mit-license.php
*/

#pragma once

#ifndef PLEN2_PIN_H
#define PLEN2_PIN_H

namespace PLEN2
{
	/*!
		@brief Management namespace of pin mapping

		Please give the standard Arduino libraries the methods returning values.
		The methods are evaluated at compile time, so there is no overhead at runtime.

		@note
		It helps your understanding that to refer the PLEN2's circuit and schematic.
		-> https://github.com/plenproject/plen__baseboard

		@sa
		Arduino Micro's pin mapping -> http://arduino.cc/en/Hacking/PinMapping32u4
	 */
	namespace Pin
	{
		// Warning D1 and D2 are used as SDA & SCL don't use them here
		//! @brief Output of PWM, for servo 12
		inline static const int PWM_OUT_12()			{ return D6; }
		//! @brief Output of PWM, for servo 14
		inline static const int PWM_OUT_14()			{ return D7; }
		//! @brief Output for WS2812 leds
		inline static const int PIXEL_PIN()				{ return D3; }
		//! @brief Input of BLE serial
		inline static const int BLE_RX()				{ return D4; }
		//! @brief Output of BLE serial
		inline static const int BLE_TX()				{ return D5; }
	}
}

#endif // PLEN2_PIN_H
