/*
 * main.h
 *
 *  Created on: 22 апр. 2012
 *      Author: adik
 */

#ifndef MAIN_H_
#define MAIN_H_
#include <Arduino.h>

// Commands list
#define CMD_PING		0x00
#define CMD_MOVETO		0x01
#define CMD_MOVE		0x02
#define CMD_STOP		0x03
#define CMD_INFO		0x04
#define CMD_POSITION	0x05

//
#define PARALEL_RUN 2

//
#define SERIAL_WAIT_CMD 0
#define SERIAL_GET_CMD 1


int8_t process_cmd(const uint8_t *, uint8_t * );


#endif /* MAIN_H_ */
