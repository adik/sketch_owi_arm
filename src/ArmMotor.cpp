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

#define NOISE_DIFF	10 	//
#define NOISE_ITER	4 	//
#define SENSITIVITY 2	//

// Constructor
ArmMotor::ArmMotor(const ArmMotorParams *params, ArmMotorStatus *status,  uint8_t freq) : AF_DCMotor(params->motornum, freq)
{
	Params = params;
	Status = status;

	// set sensor pin
	pinMode(Params->sensor_pin, INPUT);

	// set default
	Status->curentPos = Params->min_range;
	Status->targetPos = Params->min_range;
	Status->direction = RELEASE;
}

void ArmMotor::_run(uint8_t direction)
{
	if ( Status->direction != direction )
	{
		AF_DCMotor::setSpeed(150);
		AF_DCMotor::run(direction);
		Status->direction = direction;
	}
}

void ArmMotor::go()
{
	static uint16_t old_pos    = 0;
	static uint8_t  noise_iter = 0;

	// Skip if motor is not running
	if ( Status->direction == RELEASE )
		goto skip_check;

	// Getting the current position
	Status->curentPos = analogRead(Params->sensor_pin);

	// Error
	if ( noise_iter >= NOISE_ITER ) {
		_run(RELEASE);
		noise_iter = 0;
		goto skip_check;
	}

	// Skip noise
	if ( abs(old_pos - Status->curentPos) >= NOISE_DIFF ) {
		++noise_iter;
		goto skip_check;
	}

	// Calculate
	Status->distance = abs(Status->targetPos - Status->curentPos);

	if ( Status->distance < SENSITIVITY ) {
		_run(RELEASE);
	}
	else if ( Status->curentPos < Status->targetPos ) {
		_run(FORWARD);
	}
	else if (Status->curentPos > Status->targetPos ) {
		_run(BACKWARD);
	};

	old_pos = Status->curentPos;

skip_check:
	return;
}

void ArmMotor::stop() {
	_run(RELEASE);
	Status->targetPos = Status->curentPos;
}

void ArmMotor::moveTo(uint16_t position) {
	Status->targetPos = position;
}

void ArmMotor::move(uint8_t direction) {
	switch (direction) {
	case FORWARD:
		Status->targetPos = Params->max_range;
		break;
	case BACKWARD:
		Status->targetPos = Params->min_range;
		break;
	}
}

void ArmMotor::calculateSpeed() {
	//AF_DCMotor::setSpeed( x * 255 / 1024 ; );
}
