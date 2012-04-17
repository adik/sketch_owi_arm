/*
 * ArmMotor.h
 *
 *  Created on: 7 апр. 2012
 *      Author: adik
 */

#ifndef ARMMOTOR_H_
#define ARMMOTOR_H_

#include <AFMotor.h>

struct ArmMotorParams {
	uint8_t		motornum;
	uint8_t 	sensor_pin;
	uint16_t	min_range;
	uint16_t	max_range;
};

struct ArmMotorStatus {
	uint16_t	curentPos;
	uint16_t 	targetPos;
	uint16_t	distance;

	uint8_t		direction;
};



class ArmMotor : public AF_DCMotor
{
 public:
	ArmMotor(const ArmMotorParams *, ArmMotorStatus *, uint8_t freq = MOTOR12_1KHZ);

	void moveTo(uint16_t);
	void move(uint8_t);
	void stop();


	//
	void go();
	void calculateSpeed();

 private:
	void _run(uint8_t);

	const ArmMotorParams *Params;
	ArmMotorStatus *Status;
};


#endif /* ARMMOTOR_H_ */
