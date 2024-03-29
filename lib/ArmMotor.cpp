/*
 * ArmMotor.cpp
 *
 * Author: smirnov.arkady@gmail.com
 *
 */

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include <avr/io.h>
 #include "WProgram.h"
#endif

#include <ArmMotor.h>
#include <AFMotor.h>
#include "wiring_private.h"

#define DEFAULT_MOTOR_SPEED 150
#define DEFAULT_GRIP_SPEED  40

// Constructor
//
ArmMotor::ArmMotor(const ArmMotorParams *params, ArmMotorStatus *status,  uint8_t freq) : AF_DCMotor(params->motornum, freq)
{
	Params = params;
	Status = status;

	// set sensor pin
	pinMode(Params->sensorPin, INPUT);

	// set default
	Status->curentPos = Params->minPosition;
	Status->targetPos = Params->minPosition;
	Status->direction = BRAKE;
	Status->errorCode = NO_ERRORS;

	Status->completed = true;

	_errors_count = 0;

	_old_pos = analogRead(Params->sensorPin);
}

/*
 * check rotation rules
 */
void ArmMotor::check()
{
	uint8_t error = 0;

	//FIXIT:
	Status->direction = BRAKE;
	AF_DCMotor::setSpeed(DEFAULT_MOTOR_SPEED);

	// check rotation rules
	for (int i=0; i<6; ++i)
	{
		Status->curentPos = analogRead(Params->sensorPin);
		AF_DCMotor::run(FORWARD);
		delay(100);

		if (Status->curentPos - _old_pos <= 0)
			error++;

		if (error == 5) {
			brake();
			Status->errorCode = ERROR_SENSOR_NOT_WORKING;
			return ;//break;
		}

		_old_pos = Status->curentPos;
	}
}


/*
 * Parking
 */
void ArmMotor::park()
{
	moveTo(Params->startPosition);
}

/*
 *
 */
void ArmMotor::go()
{
	// Skip if brake was released
	if ( Status->direction == BRAKE )
		goto skip_check;

	// Getting the current position
	Status->curentPos = analogRead(Params->sensorPin);

	// Brake if error
	if ( _errors_count >= NOISE_COUNT ) {
		brake();
		Status->errorCode = ERROR_NOISES;
		_errors_count = 0;
		goto skip_check;
	}

	// Skip noise
	if ( abs(_old_pos - Status->curentPos) >= NOISE_DIFF ) {
		++_errors_count;
		goto skip_check;
	}

	// Calculate
	Status->distance = abs(Status->targetPos - Status->curentPos);

	if (Status->curentPos < Params->minPosition)
	{
		_run(FORWARD);
		//delay(10);
	}
	else if (Status->curentPos > Params->maxPosition)
	{
		_run(BACKWARD);
		//delay(10);
	}
	else if (Status->distance < SENSITIVITY)
	{
		//delay(10);
		_run(RELEASE);
		Status->completed = true;
	}
	else if (Status->curentPos < Status->targetPos)
	{
		_run(FORWARD);

		/*
		Serial.print(Params->motornum);
		Serial.print(" F: CurrentPos=");
		Serial.println(Status->curentPos);
		*/
	}
	else if (Status->curentPos > Status->targetPos)
	{
		_run(BACKWARD);

		/*
		Serial.print(Params->motornum);
		Serial.print(" B: CurrentPos=");
		Serial.println(Status->curentPos);
		*/
	};

	_old_pos = Status->curentPos;

skip_check:
	return;
}

void ArmMotor::release()
{
	_run( RELEASE );
	//FIXIT: momentum
	delay(100);
	Status->targetPos = setPosition(analogRead(Params->sensorPin));
	Status->completed = true;
}

void ArmMotor::brake()
{
	release();
	Status->direction = BRAKE;
}

void ArmMotor::moveTo(uint16_t position)
{
	Status->targetPos = setPosition(position);
	Status->completed = false;
	_run(RELEASE);
}

void ArmMotor::move(uint8_t direction)
{
	switch (direction) {
	case FORWARD:
		Status->targetPos = Params->maxPosition;
		_run(FORWARD);
		break;
	case BACKWARD:
		Status->targetPos = Params->minPosition;
		_run(BACKWARD);
		break;
	}
	Status->completed = false;
}

void ArmMotor::_run(uint8_t direction)
{
	if ( Status->direction != direction )
	{
		AF_DCMotor::setSpeed(DEFAULT_MOTOR_SPEED);
		AF_DCMotor::run(direction);
		Status->direction = direction;
	}
}

int16_t ArmMotor::setPosition(int16_t position) {
	return constrain(position, Params->minPosition, Params->maxPosition);
}



/*
 *
 *  Class for control the arm grip
 *
 */

#define GRIP_PWM_PIN	10  // use only pwm pins
#define GRIP_DIR_PIN	9
#define GRIP_INT_PIN	2


ArmGrip::ArmGrip(uint8_t freq)
{
	// setup pin mode
	pinMode(GRIP_PWM_PIN, OUTPUT);
	pinMode(GRIP_DIR_PIN, OUTPUT);

	// setup interrupt pin (int0 = pin 2 )
	attachInterrupt(0, ArmGrip::onInterput, FALLING);
}


void ArmGrip::run(uint8_t dir)
{
	switch (dir) {
	case FORWARD:
		digitalWrite(GRIP_DIR_PIN, HIGH);
		powerOn();
		break;
	case BACKWARD:
		digitalWrite(GRIP_DIR_PIN, LOW);
		powerOn();
		break;
	case BRAKE:
		powerOff();
		break;
	}
}

void ArmGrip::onInterput()
{
	run(BRAKE);
}


void ArmGrip::powerOn()
{
	/* sbi(TCCR1A, COM1A1);
	 * OCR1A = DEFAULT_GRIP_SPEED; */

	analogWrite(GRIP_PWM_PIN, DEFAULT_GRIP_SPEED);
}

void ArmGrip::powerOff()
{
	/*  from wiring_digital.c : turnOffPWM(uint8_t timer)
	 *
     *  case TIMER1A:  cbi(TCCR1A, 	COM1A1);    break;
     *  case TIMER1B:  cbi(TCCR1A,  COM1B1);    break;
     *
     *  or you can use digitalWrite(pin, LOW)
     *  or analogWrite(pin, LOW)  */

	digitalWrite(GRIP_PWM_PIN, LOW);
}





