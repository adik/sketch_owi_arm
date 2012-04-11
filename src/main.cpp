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



static uint8_t incomingByte;
static uint8_t i, m, j;

const uint8_t MOTOR_COUNT=4;
const uint8_t motor_sensor[MOTOR_COUNT] = {A0, A1, A2, A3};

ArmMotor * motor[MOTOR_COUNT];


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
				motor[motornum]->moveTo(position);
				Serial.write(0x11);
			}
			break;
		case CMD_MOVE:
			motornum = nextByte();
			direction = nextByte();
			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				motor[motornum]->move(direction);
				Serial.write(0x12);
			}
			break;
		case CMD_STOP:
			motornum = nextByte();
			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				motor[motornum]->stop();
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
		motor[i] = new ArmMotor(i+1, motor_sensor[i]);
	}

	motor[0]->moveTo(500);
	motor[1]->moveTo(500);
}


inline void loop()
{
	// process serial commands
	processSerial();

	// calculate speed, detect motor position
	for (i=0; i<MOTOR_COUNT; ++i) {
		motor[i]->go();
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
