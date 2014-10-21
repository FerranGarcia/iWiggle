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

class distSensor
{
public:

	int fd;														// File descrition
	char *fileName;												// Name of the port we will be using
	int  address;												// Address of SRF08 shifted one bit
	char buf[10];												// Buffer for data being read/ written on the i2c bus

	FILE *port;

	distSensor(void);
	~distSensor(void);
	unsigned int getDistance(void);


};