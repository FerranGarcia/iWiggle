#include "Motion.h"

Motion::Motion(void)
{
	this->fileName = "/dev/i2c-1";
	this->address = 0x58;

	printf("**** MD25 test program ****\n");
	
	// skip opening port on windows
#ifdef __linux__
	this->port = fopen(fileName,"r+");										// Open port for reading and writing
	if (this->port == NULL){
		printf("Failed to open i2c port\n");
		exit(1);
	}

	this->fd = fileno(this->port);

	if (ioctl(this->fd, I2C_SLAVE, address) < 0) {						// Set the port options and set the address of the device we wish to speak to
		printf("Unable to get bus access to talk to slave\n");
		exit(1);
	}

	if ((write(this->fd, this->buf, 1)) != 1) {							// Send regiter to read software from
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	
	if (read(this->fd, this->buf, 1) != 1) {							// Read back data into buf[]
		printf("Unable to read from slave modafucka\n");
		exit(1);
	}
	else {
		printf("Software version: %u\n", this->buf[0]);
	}

	this->buf[0] = 15;
	this->buf[1] = 3;

	if ((write(this->fd, this->buf, 2)) != 2) {							// Change Mode
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else 

#endif
}

void Motion::resetEncoders(void) {
#ifdef __linux__
	this->buf[0] = 16;												// Command register
	this->buf[1] = 32;												// command to set decoders back to zero

	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else 
	cout << "[ INFO ] Reseting encoders." << endl;
#endif
}

long Motion::readEncoderValues (void) {

	long encoder1 = 0, encoder2 = 0;
#ifdef __linux__
	
	this->buf[0] = 2;													// register for start of encoder values
	
	if ((write(this->fd, this->buf, 1)) != 1) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	
	if (read(this->fd, this->buf, 8) != 8) {								// Read back 8 bytes for the encoder values into buf[]
		printf("Unable to read from slave\n");
		exit(1);
	}
	else {
		encoder1 = (this->buf[0] <<24) + (this->buf[1] << 16) + (this->buf[2] << 8) + this->buf[3];	// Put encoder values together
		encoder2 = (this->buf[4] <<24) + (this->buf[5] << 16) + (this->buf[6] << 8) + this->buf[7];
		printf("Encoder 1: %08lX   Encoder 2: %08lX\n",encoder1, encoder2);
	}

	#else 
		cout << "[ INFO ] Reading encoder values." << endl;
	#endif
		return encoder1;
}

void Motion::driveMotors(int linear, int angular){

#ifdef __linux__

	this->buf[0] = 0;														// Register to set speed of motor 1
	this->buf[1] = linear;													// speed to be set
	
	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	
	this->buf[0] = 1;														// motor 2 speed
	this->buf[1] = angular;												
	
	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else
	cout << "[ INFO ] Setting linear speed to " << linear << " and angular speed to " << angular << "." << endl;
#endif
}

void Motion::stopMotors(void){

#ifdef __linux__

	this->buf[0] = 0;													
	this->buf[1] = 0;														// A speed of 128 stops the motor
	
	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
	
	this->buf[0] = 1;													
	this->buf[1] = 0;												
	
	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else
	cout << "[ INFO ] Stopping motors." << endl;
#endif
}

void Motion::turnLeft(int speed){

#ifdef __linux__
	this->buf[0] = 0;													
	this->buf[1] = speed;

	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	this->buf[0] = 1;													
	this->buf[1] = -speed;

	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else
	cout << "[ INFO ] Turning left." << endl;
#endif
}

void Motion::turnRight(int speed){

#ifdef __linux__
	this->buf[0] = 1;													
	this->buf[1] = speed;

	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}

	this->buf[0] = 0;													
	this->buf[1] = speed;

	if ((write(this->fd, this->buf, 2)) != 2) {
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#else
	cout << "Turning right." << endl;
#endif
}

Motion::~Motion(void)
{}
