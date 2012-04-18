/*
 * kinematic.cpp
 *
// http://www.circuitsathome.com/mcu/programming/robotic-arm-inverse-kinematics-on-arduino
//              ULMA    GRIPP..
//             +-------+------E
//     HUMERUS |
//             |
//            =0=
//     BASE    |
//     _______===_______
 */

#include <Arduino.h>

/* Arm dimensions( mm ) */
#define BASE_HGT 65 	//base height 2.65"
#define HUMERUS 90 		//shoulder-to-elbow "bone" 5.75"
#define ULNA 110 		//elbow-to-wrist "bone" 7.375"
#define GRIPPER 100 	//gripper (incl.heavy duty wrist rotate mechanism) length 3.94"

/* pre-calculations */
float hum_sq = HUMERUS*HUMERUS;
float uln_sq = ULNA*ULNA;

// analog input to potentiometer ratio
float pot_ratio = 1024/300;

/* arm positioning routine utilizing inverse kinematics */
/* z is height, y is distance from base center out, x is side to side. y,z can only be positive */
//void set_arm( uint16_t x, uint16_t y, uint16_t z, uint16_t grip_angle )
void set_arm( float x, float y, float z, float grip_angle_d)
{
  float grip_angle_r = radians( grip_angle_d ); //grip angle in radians for use in calculations
  /* Base angle and radial distance from x,y coordinates */
  float bas_angle_r = atan2( x, y );
  float rdist = sqrt(( x * x ) + ( y * y ));
  /* rdist is y coordinate for the arm */
  y = rdist;

  /* Grip offsets calculated based on grip angle */
  float grip_off_z = ( sin( grip_angle_r )) * GRIPPER;
  float grip_off_y = ( cos( grip_angle_r )) * GRIPPER;
  /* Wrist position */
  float wrist_z = ( z - grip_off_z ) - BASE_HGT;
  float wrist_y = y - grip_off_y;
  /* Shoulder to wrist distance ( AKA sw ) */
  float s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
  float s_w_sqrt = sqrt( s_w );
  /* s_w angle to ground */
  //float a1 = atan2( wrist_y, wrist_z );
  float a1 = atan2( wrist_z, wrist_y );
  /* s_w angle to humerus */
  float a2 = acos((( hum_sq - uln_sq ) + s_w ) / ( 2 * HUMERUS * s_w_sqrt ));
  /* shoulder angle */
  float shl_angle_r = a1 + a2;
  float shl_angle_d = degrees( shl_angle_r );
  /* elbow angle */
  float elb_angle_r = acos(( hum_sq + uln_sq - s_w ) / ( 2 * HUMERUS * ULNA ));
  float elb_angle_d = degrees( elb_angle_r );
  float elb_angle_dn = -( 180.0 - elb_angle_d );
  /* wrist angle */
  float wri_angle_d = ( grip_angle_d - elb_angle_dn ) - shl_angle_d;


  /*
  int16_t bas_pos = MotorParams[0].startPosition + degrees(bas_angle_r) * pot_ratio;
  int16_t shl_pos = MotorParams[1].startPosition + (MotorParams[1].startAngle - shl_angle_d ) * pot_ratio;
  int16_t elb_pos = MotorParams[2].startPosition + (MotorParams[2].startAngle - elb_angle_dn ) * pot_ratio;
  int16_t wri_pos = MotorParams[3].startPosition + (MotorParams[3].startAngle - wri_angle_d ) * pot_ratio;
  */

  /*
  Serial.print("bas_pos="); Serial.println(bas_pos);
  Serial.print("shl_pos="); Serial.println(shl_pos);
  Serial.print("elb_pos="); Serial.println(elb_pos);
  Serial.print("wri_pos="); Serial.println(wri_pos);
  */
}


/*
case CMD_POSITION:
	double coord_x, coord_y, coord_z, grip_angle;

	coord_x = get_next_byte();
	coord_y = get_next_byte();
	coord_z = get_next_byte();
	grip_angle = get_next_byte();

	set_arm(coord_x, coord_y, coord_z, grip_angle);

	Serial.print(" x, y, z, a = ");
	Serial.print(coord_x); Serial.print(coord_y);
	Serial.print(coord_z); Serial.print(grip_angle);
	Serial.println();


	break;
*/


/*
inline void setup()
{
	Serial.begin(115200);
	Serial.println("ARM debug");
}

inline void loop()
{

	Serial.print("A0="); Serial.println(analogRead(A0));
	Serial.print("A1="); Serial.println(analogRead(A1));
	Serial.print("A2="); Serial.println(analogRead(A2));
	Serial.print("A3="); Serial.println(analogRead(A3));

	delay(100);
}
*/
