#include "distSensor.h"

distSensor::distSensor(void)
{
													
	this->fileName = (char *)"/dev/i2c-1";								// Name of the port we will be using
	this->address = 0x70;										// Address of the SRF08 shifted right 1 bit
	unsigned char buf[10];										// Buffer for data being read/ written on the i2c bus

#ifdef __linux__
	this->port = fopen(fileName, "r+");							// Open port for reading and writing

	if (this->port == NULL) {									// Open port for reading and writing
		printf("Failed to open i2c port\n");
		exit(1);
	}

	this->fd = fileno(this->port);

	if (ioctl(this->fd, I2C_SLAVE, address) < 0) {					// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	this->buf[0] = 0;													// Commands for performing a ranging on the SRF08
	this->buf[1] = 81;

	if ((write(this->fd, this->buf, 2)) != 2) {								// Write commands to the i2c port
		printf("Error writing to i2c slave primera part\n");
		exit(1);
	}

	usleep(750000);	
	this->buf[0] = 0;													// This is the register we wish to read from

	if ((write(this->fd, this->buf, 1)) != 1) {								// Send register to read from
		printf("Error writing to i2c slave sgona part\n");
		exit(1);
	}

	if (read(this->fd, this->buf, 4) != 4) {								// Read back data into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
	else {
		unsigned char highByte = this->buf[2];
		unsigned char lowByte = this->buf[3];
		unsigned int result = (highByte << 8) + lowByte;			// Calculate range
		printf("Software v: %u \n", buf[0]);
		printf("Light: %u \n", buf[1]);
		printf("Range was: %u\n", result);
	}

#else 
	cout << "Initialization of the i2c for the ultrasonic sensor" << endl;
#endif
}

unsigned int distSensor::getDistance(void)
{
	unsigned int result = 0;
#ifdef __linux__
	this->buf[0] = 0;													// Commands for performing a ranging on the SRF08
	this->buf[1] = 81;

	if ((write(this->fd, this->buf, 2)) != 2) {								// Write commands to the i2c port
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	usleep(750000);	
	this->buf[0] = 0;													// This is the register we wish to read from

	if ((write(this->fd, this->buf, 1)) != 1) {								// Send register to read from
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	if (read(this->fd, this->buf, 4) != 4) {								// Read back data into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
	else {
		unsigned char highByte = this->buf[2];
		unsigned char lowByte = this->buf[3];
		result = (highByte << 8) + lowByte;			// Calculate range
	}

#else
	cout << "Obtaining measurement from ultrasonic sensor" << endl;
#endif

	return result;
}

distSensor::~distSensor(void)
{}