/*
 * ArmMotor.h
 *
 *  Created on: 7 апр. 2012
 *      Author: adik
 */

#ifndef ARMMOTOR_H_
#define ARMMOTOR_H_

#include <AFMotor.h>


class ArmMotor : public AF_DCMotor
{
 public:
	ArmMotor(uint8_t mnum, uint8_t pin, uint8_t freq = MOTOR12_1KHZ);

	void moveTo(uint16_t);
	void move(uint8_t);
	void stop();


	//
	void go();
	void calculateSpeed();

 private:
	uint8_t 	_sensor_pin;
	uint8_t 	_mnum;

	//
	uint16_t 	_currentPosition;
	uint16_t 	_targetPosition;
	uint16_t 	_distance;
	boolean 	_stoped;

};


#endif /* ARMMOTOR_H_ */
