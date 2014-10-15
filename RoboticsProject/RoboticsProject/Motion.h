#pragma once

#ifdef __linux__
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <linux/i2c.h>
#else
#include "stdafx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <iostream>
#include <fstream>

using namespace std;

class Motion
{
public:
	int fd;														// File descrition
	char *fileName;												// Name of the port we will be using
	int  address;												// Address of MD25 shifted one bit										
	char buf[10];												// Buffer for data being read/ written on the i2c bus

	FILE *port;


	Motion(void);
	void resetEncoders(void);
	long readEncoderValues(void);
	void driveMotors(int, int);
	void stopMotors(void);
	void turnLeft(int);
	void turnRight(int);
	
	~Motion(void);
};
