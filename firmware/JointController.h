/*!
	@file      JointController.h
	@brief     Management class of joints.
	@author    Kazuyuki TAKASE
	@copyright The MIT License - http://opensource.org/licenses/mit-license.php
*/

#ifndef PLEN2_JOINT_CONTROLLER_H
#define PLEN2_JOINT_CONTROLLER_H

typedef struct _ServoData {
	int MIN;
	int MAX;
	int HOME;
	char pin;
}ServoData;

namespace PLEN2
{
	class JointController;
}

class PLEN2::JointController
{
public:
	enum {
		SUM = 18, //!< Summation of the servos controllable.

		ANGLE_MIN     = -800, //!< Min angle of the servos. // TODO : fix it for MG90S
		ANGLE_MAX     =  800, //!< Max angle of the servos. // TODO : fix it for MG90S
		ANGLE_NEUTRAL =    0  //!< Neutral angle of the servos.
	};

private:
	//! @brief Initialized flag's address on internal EEPROM
	inline static const int INIT_FLAG_ADDRESS()     { return 0; }

	//! @brief Initialized flag's value
	inline static const unsigned char INIT_FLAG_VALUE()       { return 2; }

	//! @brief Head-address of joint settings on internal EEPROM
	inline static const int SETTINGS_HEAD_ADDRESS() { return 1; }

	/*!
		@brief Management class of joint setting
	*/
	class JointSetting
	{
	public:
		int MIN;  //!< Setting about min angle.
		int MAX;  //!< Setting about max angle.
		int HOME; //!< Setting about home angle.
		unsigned char pin;
		/*!
			@brief Constructor
		*/
		JointSetting()
			: MIN(ANGLE_MIN)
			, MAX(ANGLE_MAX)
			, HOME(ANGLE_NEUTRAL)
			, pin(0)
		{
			// noop.
		}
	};

	JointSetting m_SETTINGS[SUM];
public:
    inline static const int PWM_FREQ()    { return 60;  }

	//! @brief PWM width that to make min angle
	inline static const int PWM_MIN()     { return 100;  } // for cheap chinese MG90S

	//! @brief PWM width that to make max angle
	inline static const int PWM_MAX()     { return 550;  } // for cheap chinese MG90S

	//! @brief PWM width that to make neutral angle
	inline static const int PWM_NEUTRAL() { return PWM_MIN() + (PWM_MAX() - PWM_MIN());  } 
    
	/*!
		@brief Finished flag of PWM output procedure 1 cycle

		@attention
		The instance should be a private member normally.
		It is a public member because it is only way to access from Timer 1 overflow interruption vector,
		so you must not access it from other functions basically.
	*/
	volatile static bool m_1cycle_finished;

	/*!
		@brief PWM buffer

		@attention
		The instance should be a private member normally.
		It is a public member because it is only way to access from Timer 1 overflow interruption vector,
		so you must not access it from other functions basically.
	*/
	static int m_pwms[SUM];

	/*!
		@brief Constructor
	*/
	JointController();

	void Init();

	/*!
		@brief Load the joint settings

		The method reads joint settings from internal EEPROM.
		If the EEPROM has no settings, the method also writes the default values.

		@sa
		JointController.cpp::Shared::m_SETTINGS_INITIAL

		@attention
		The method should call in constructor normally,
		but initialized timing of any interruption is indefinite, so might get deadlock.
		(The method uses serial communication and internal EEPROM accessing, so it happens interruption.)
	*/
	void loadSettings();

	/*!
		@brief Reset the joint settings

		Write default settings to internal EEPROM.
	*/
	void resetSettings();

	/*!
		@brief Get min angle of the joint given

		@param [in] joint_id Please set joint id you want to get min angle.

		@return Reference of min angle a joint expressed by **joint_id** has.
		@retval -32768 Argument error. (**joint_id** is invalid.)
	*/
	const int& getMinAngle(unsigned char joint_id);

	/*!
		@brief Get max angle of the joint given

		@param [in] joint_id Please set joint id you want to get max angle.

		@return Reference of max angle a joint expressed by **joint_id** has.
		@retval -32768 Argument error. (**joint_id** is invalid.)
	*/
	const int& getMaxAngle(unsigned char joint_id);

	/*!
		@brief Get home angle of the joint given

		@param [in] joint_id Please set joint id you want to get home angle.

		@return Reference of home angle a joint expressed by **joint_id** has.
		@retval -32768 Argument error. (**joint_id** is invalid.)
	*/
	const int& getHomeAngle(unsigned char joint_id);

	/*!
		@brief Set min angle of the joint given

		@param [in] joint_id Please set joint id you want to define min angle.
		@param [in] angle    Please set angle that has steps of degree 1/10.

		@return Result
	*/
	bool setMinAngle(unsigned char joint_id, int angle);

	/*!
		@brief Set max angle of the joint given

		@param [in] joint_id Please set joint id you want to define max angle.
		@param [in] angle    Please set angle that has steps of degree 1/10.

		@return Result
	*/
	bool setMaxAngle(unsigned char joint_id, int angle);

	/*!
		@brief Set home angle of the joint given

		@param [in] joint_id Please set joint id you want to define home angle.
		@param [in] angle    Please set angle that has steps of degree 1/10.

		@return Result
	*/
	bool setHomeAngle(unsigned char joint_id, int angle);

	/*!
		@brief Set angle of the joint given

		@param [in] joint_id Please set joint id you want to set angle.
		@param [in] angle    Please set angle that has steps of degree 1/10.

		@return Result

		@attention
		<b>angle</b> might not be setting actually.
		It is setting after trimming by user defined min-max value or servo's range,
		so please consider it when writing a unit test.
	*/
	bool setAngle(unsigned char joint_id, int angle);

	/*!
		@brief Set angle to "angle-diff + home-angle" of the joint given

		@param [in] joint_id   Please set joint id you want to set angle-diff.
		@param [in] angle_diff Please set angle-diff that has steps of degree 1/10.

		@return Result

		@attention
		<b>angle_diff</b> might not be setting actually.
		It is setting after trimming by user defined min-max value or servo's range,
		so please consider it when writing a unit test.
	*/
	bool setAngleDiff(unsigned char joint_id, int angle_diff);

	/*!
		@brief Dump the joint settings

		Output result like JSON format below.
		@code
		[
			{
				"max": <integer>,
				"min": <integer>,
				"home": <integer>
			},
			...
		]
		@endcode
	*/
	void dump();

    static void updateAngle();

    static void updateLeds();
};

#endif // PLEN2_JOINT_CONTROLLER_H
