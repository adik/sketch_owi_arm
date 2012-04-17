/*
 * Target: AVR328P
 * Crystal: 16.000Mhz
 */
#include <Arduino.h>
#include <ArmMotor.h>

// Commands list
#define CMD_PING	0x00
#define CMD_MOVETO	0x01
#define CMD_MOVE	0x02
#define CMD_STOP	0x03
#define CMD_INFO	0x04


static uint8_t incomingByte;
static uint8_t i, m, j;


const uint8_t MOTOR_COUNT=4;
static ArmMotor *Motor[MOTOR_COUNT];
static ArmMotorStatus MotorStatus[MOTOR_COUNT] = {};
const  ArmMotorParams MotorParams[MOTOR_COUNT] = {
	{1, A0, 10, 1000},
	{2, A1, 10, 1000},
	{3, A2, 10, 1000},
	{4, A3, 10, 1000},
};



// TODO
uint8_t nextByte() {
  unsigned long start = millis();
  while (millis() - start < 100) {
    if (Serial.available()) {
      return Serial.read();
    }
  }
  return 0x00;
}

inline void processSerial()
{
	uint8_t   motornum, direction;
	uint16_t  position;

	if (Serial.available())
	{
		incomingByte = Serial.read();

		switch (incomingByte) {
		case CMD_PING:
			Serial.write(0x10);
			break;
		case CMD_MOVETO:
			motornum = nextByte();
			position = (nextByte()<<8) | nextByte();
			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->moveTo(position);
				Serial.write(0x11);
			}
			break;
		case CMD_MOVE:
			motornum = nextByte();
			direction = nextByte();
			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->move(direction);
				Serial.write(0x12);
			}
			break;
		case CMD_STOP:
			motornum = nextByte();
			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->stop();
				Serial.write(0x13);
			}
			break;
		default:
			Serial.write(0xFF);
			break;
		}
	}
}

inline void setup()
{
	Serial.begin(9600);
	Serial.println("ARM control v000");

	for (i=0; i<MOTOR_COUNT; ++i) {
		Motor[i] = new ArmMotor(&MotorParams[i], &MotorStatus[i]);
	}

	Motor[0]->moveTo(500);
	Motor[1]->moveTo(500);
}


inline void loop()
{
	// process serial commands
	processSerial();

	// calculate speed, detect motor position
	for (i=0; i<MOTOR_COUNT; ++i) {
		Motor[i]->go();
	}
	delay(200);
}

////////// ---------------------------------- ////////
int main(void)
{
	init();
	setup();
	for (;;) {
		loop();
		//if (serialEventRun) serialEventRun();
	}
	return 0;
}
