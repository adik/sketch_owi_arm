/*
 * main.h
 *
 * Author: smirnov.arkady@gmail.com
 *
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

// The maximum number of parallel running motors
#define MAX_PARALLEL_RUN 2

int8_t serial_process_cmd(const uint8_t *, uint8_t *);


#endif /* MAIN_H_ */
