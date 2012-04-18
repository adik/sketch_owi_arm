/*
 *
 */
var BASE_HGT = 65
var HUMERUS = 90
var ULNA = 110
var GRIPPER = 100

var hum_sq = HUMERUS*HUMERUS
var uln_sq = ULNA*ULNA;

var pot_ratio = 1024/300;

startPositions = [530, 700, 530, 480]
startAngles = [0, 90, -90, 0]


function degrees(val) {
	return (val*(180/Math.PI))
}
function radians(val) {
	return (val*(Math.PI/180))
}
	
function calculate_angles(x, y, z, grip_angle_d) {
	grip_angle_r = radians( grip_angle_d )
	
	/* Base angle and radial distance from x,y coordinates */
	var bas_angle_r = Math.atan2( x, y );
	var bas_angle_d = degrees(bas_angle_r)
	var rdist = Math.sqrt(( x * x ) + ( y * y ));
	/* rdist is y coordinate for the arm */
	var y = rdist;
	/* Grip offsets calculated based on grip angle */
	var grip_off_z = ( Math.sin( grip_angle_r )) * GRIPPER;
	var grip_off_y = ( Math.cos( grip_angle_r )) * GRIPPER;
	/* Wrist position */
	var wrist_z = ( z - grip_off_z ) - BASE_HGT;
	var wrist_y = y - grip_off_y;
	/* Shoulder to wrist distance ( AKA sw ) */
	var s_w = ( wrist_z * wrist_z ) + ( wrist_y * wrist_y );
	var s_w_sqrt = Math.sqrt( s_w );
	/* s_w angle to ground */
	var a1 = Math.atan2( wrist_z, wrist_y );
	/* s_w angle to humerus */
	var a2 = Math.acos((( hum_sq - uln_sq ) + s_w ) / ( 2 * HUMERUS * s_w_sqrt ));
	/* shoulder angle */
	var shl_angle_r = a1 + a2;
	var shl_angle_d = degrees( shl_angle_r );
	/* elbow angle */
	var elb_angle_r = Math.acos(( hum_sq + uln_sq - s_w ) / ( 2 * HUMERUS * ULNA ));
	var elb_angle_d = degrees( elb_angle_r );
	var elb_angle_dn = -( 180.0 - elb_angle_d );
	/* wrist angle */
	var wri_angle_d = ( grip_angle_d - elb_angle_dn ) - shl_angle_d;

	return [bas_angle_d, shl_angle_d, elb_angle_dn, wri_angle_d]
}

function calculate_coords(angles) 
{
	var bas_pos = startPositions[0] + angles[0] * pot_ratio;
	var shl_pos = startPositions[1] + (startAngles[1] - angles[1] ) * pot_ratio;
	var elb_pos = startPositions[2] + (startAngles[2] - angles[2] ) * pot_ratio;
	var wri_pos = startPositions[3] + (startAngles[3] - angles[3] ) * pot_ratio;
	
	return [bas_pos, shl_pos, elb_pos, wri_pos]
}