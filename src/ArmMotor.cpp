/*
 * ArmMotor.cpp
 *
 *  Created on: 7 апр. 2012
 *      Author: adik
 */
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include <avr/io.h>
 #include "WProgram.h"
#endif

#include <ArmMotor.h>
#include <AFMotor.h>

#define STOP_RANGE 10

// Constructor
ArmMotor::ArmMotor(uint8_t mnum, uint8_t pin, uint8_t freq) : AF_DCMotor(mnum, freq)
{
	// set sensor pin
	_sensor_pin = pin;
	pinMode(_sensor_pin, INPUT);

	//
	_currentPosition = 0;
	_targetPosition = 0;
	_stoped = true;

	_mnum = mnum;

	// stop motor
	//AF_DCMotor::setSpeed(100);
	//AF_DCMotor::run(FORWARD);

}

void ArmMotor::go()
{
	static uint8_t _chkupd = 0;

	if (_stoped)
		goto skip_check;

	_currentPosition = analogRead(_sensor_pin);

	if ( _chkupd == _currentPosition)
		goto skip_check;

//
	_distance = abs(_targetPosition - _currentPosition);

	AF_DCMotor::setSpeed( 150 );

	if ( _distance < STOP_RANGE ) {
		AF_DCMotor::run(RELEASE);
		_stoped = true;
	}
	else if (_currentPosition < _targetPosition ) {
		AF_DCMotor::run(FORWARD);
	}
	else if (_currentPosition > _targetPosition ) {
		AF_DCMotor::run(BACKWARD);
	};

skip_check:
	_chkupd = _currentPosition;
}

void ArmMotor::stop() {
	_stoped = true;
	_targetPosition = _currentPosition;

	AF_DCMotor::run(RELEASE);
}

void ArmMotor::moveTo(uint16_t position) {
	_stoped = false;
	_targetPosition = position;
}

void ArmMotor::move(uint8_t direction) {
	_stoped = false;

	//AF_DCMotor::run(direction);
	switch (direction) {
	case FORWARD:
		_targetPosition = 1000;
		break;
	case BACKWARD:
		_targetPosition = 10;
		break;
	}
}

void ArmMotor::calculateSpeed() {
	//AF_DCMotor::setSpeed( x * 255 / 1024 ; );
}
