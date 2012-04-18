/*
 * Target: AVR328P
 *
 *
 *
 * Links :
 * http://www.societyofrobots.com/robot_arm_tutorial.shtml
 *
 */
#include <main.h>
#include <Arduino.h>
#include <ArmMotor.h>



const uint8_t MOTOR_COUNT=4;

static ArmMotor 		*Motor[MOTOR_COUNT];
static ArmMotorStatus 	MotorStatus[MOTOR_COUNT] = {};
const  ArmMotorParams	MotorParams[MOTOR_COUNT] = {
	{4, A3, 30,  1000,  530, 0},
	{3, A2, 450, 1000, 	700, 90},
	{2, A1, 10,  700,  	530, -90},
	{1, A0, 235, 720,  	480, 0},
};


uint8_t i, m, j, r;

uint8_t incomingByte;
uint8_t serial_step = 0;
uint8_t recv_byte = 0;
uint8_t cmd_buffer[13];

/*
 *
 */
inline void process_serial(uint8_t *buffer)
{
	incomingByte = Serial.read();
	switch (serial_step) {
	case SERIAL_GET_CMD:
		buffer[recv_byte] = incomingByte;
		recv_byte++;
		if ( process_cmd(&recv_byte, buffer ) )
		{
			serial_step = SERIAL_WAIT_CMD;
		}
		break;
	case SERIAL_WAIT_CMD:
	default:
		if (incomingByte == '\0'){
			serial_step = SERIAL_GET_CMD;
			*buffer = '\0';
			recv_byte = 0;
		}
		break;
	}
}


/*
 *
 */
inline int8_t process_cmd(const uint8_t *recv_byte, uint8_t *buffer )
{
	switch (buffer[0])
	{
	case CMD_MOVETO:
		if (*recv_byte==4){
			buffer[*recv_byte] = '\0';

			uint8_t motornum = buffer[1];
			uint16_t position = (buffer[2]<<8) | buffer[3];

			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->moveTo(position);
			}

			Serial.print(" Motornum="); Serial.print(motornum);
			Serial.print(" position="); Serial.println(position);

			return 1;
		};
		break;

	case CMD_MOVE:
		if (*recv_byte==3){
			buffer[*recv_byte] = '\0';
			uint8_t  motornum = buffer[1];
			uint8_t direction = buffer[2];

			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->move(direction);
				Serial.write(0x12);
			}
			return 1;
		};
		break;

	case CMD_STOP:
		if (*recv_byte==2){
			buffer[*recv_byte] = '\0';

			uint8_t motornum = buffer[1];

			if ( motornum>=0 && motornum<MOTOR_COUNT ) {
				Motor[motornum]->release();
			}

			Serial.print(" Motornum="); Serial.print(motornum);
			Serial.print(" Distance="); Serial.print(MotorStatus[motornum].distance);
			Serial.print(" error="); Serial.print(MotorStatus[motornum].errorCode);
			Serial.print(" CurrentPos="); Serial.print(MotorStatus[motornum].curentPos);
			Serial.print(" TargetPos=");  Serial.println(MotorStatus[motornum].targetPos);

			return 1;
		};
		break;
	case CMD_PING:
	default:
		return 1;
		break;
	}

	return 0;
}


/*
 *
 *
 */
inline void setup()
{
	Serial.begin(115200);
	Serial.println("ARM control v000");

	for (i=0; i<MOTOR_COUNT; ++i) {
		Motor[i] = new ArmMotor(&MotorParams[i], &MotorStatus[i]);
		Motor[i]->park();
	}
}


inline void loop()
{
	// process serial commands
	if (Serial.available()) process_serial(cmd_buffer);

	// V1
	for (i=0, r=0; i<MOTOR_COUNT; ++i)
		if (!MotorStatus[i].completed)
		{
			Motor[i]->go();

			if (++r == PARALEL_RUN) break;
		}

//	delay(50);
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
