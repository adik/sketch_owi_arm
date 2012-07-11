/*
 * ArmMotor.h
 *
 * Author: smirnov.arkady@gmail.com
 *
 */

#ifndef ARMMOTOR_H_
#define ARMMOTOR_H_

#include <AFMotor.h>


#define NOISE_DIFF	100 //
#define NOISE_COUNT	10 	//
#define SENSITIVITY 10	//

#define NO_ERRORS					0
#define ERROR_SENSOR_NOT_WORKING	1
#define ERROR_NOISES				2


struct ArmMotorParams {
	uint8_t		motornum;
	uint8_t 	sensorPin;

	int16_t		minPosition;
	int16_t		maxPosition;

	int16_t		startPosition;
	int16_t		startAngle;
};

struct ArmMotorStatus {
	int16_t		curentPos;
	int16_t		targetPos;
	int16_t		distance;

	uint8_t		direction;

	uint8_t		errorCode;

	boolean		completed;
};



class ArmMotor : public AF_DCMotor
{
 public:
	ArmMotor(const ArmMotorParams *, ArmMotorStatus *, uint8_t freq = MOTOR12_8KHZ);

	void check();
	void park();

	void moveTo(uint16_t);
	void move(uint8_t);
	void release();
	void brake();

	//
	void go();

 private:
	void 	_run(uint8_t);
	int16_t setPosition(int16_t);

	const ArmMotorParams *Params;
		  ArmMotorStatus *Status;

	uint8_t _errors_count;
	int16_t _old_pos;
};


class ArmGrip {
 public:
	ArmGrip(uint8_t freq = MOTOR12_8KHZ);
	void run(uint8_t);
	static void onInterput() ;
 private:
	void powerOn();
	void powerOff();
};

#endif /* ARMMOTOR_H_ */
