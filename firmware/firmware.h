/*!
	@file      firmware.h
	@brief     Mainpage's markdown file
	@author    Kazuyuki TAKASE
	@copyright The MIT License - http://opensource.org/licenses/mit-license.php
*/

/*!
	@mainpage

	PLEN2 - Firmware for ESP8266
	===============================================================================

	Code name "Cytisus" (version 1.3.1) is now available!


	## Copyright (c) 2015,
	- [Kazuyuki TAKASE](https://github.com/junbowu), Project manager of the repository.
	- [PLEN Project Company Inc.](https://plen.jp)


	## API Guide
	Please check [here](https://plenproject.github.io/plen__firmware_for_Arduino).


	## Build Enviroment
	- Arduino IDE ver.1.8.5

	## License
	This software is released under the MIT License.
*/
\
/*!
	@note
	If you want to apply natural moving on PLEN, set the macro to "true".
	(The process is using a lot of calculation space, so the firmware becomes hard to input user action.)
*/
#define MPU_6050		true
#define WS2812_HEAD		true
#define WS2812_TORSO	true
#define BLE_SERIAL		true


