#include "Motion.h"

Motion::Motion(void)
{
	this->fileName = (char *)"/dev/i2c-1";
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
   // reset encoders before start driving;
   resetEncoders();
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

long int Motion::readEncoderValueLeft(void) {

	long encoder = 0;
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
		encoder = (this->buf[0] <<24) + (this->buf[1] << 16) + (this->buf[2] << 8) + this->buf[3];	// Put encoder values together
	}

	#else 
		cout << "[ INFO ] Reading encoder value left." << endl;
	#endif
    // due to wheel mounting it has to be multiplied by -1 
	//to have a correct encoder readings
    encoder *= -1;
	return encoder;
}

long int Motion::readEncoderValueRight(void) {

	long encoder = 0;
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
		encoder = (this->buf[4] <<24) + (this->buf[5] << 16) + (this->buf[6] << 8) + this->buf[7];	// Put encoder values together
}

#else 
	cout << "[ INFO ] Reading encoder value right." << endl;
#endif
    // due to wheel mounting it has to be multiplied by -1 
	//to have a correct encoder readings
    encoder *= -1;
	return encoder;
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

void Motion::turnAngle(int angle, int speed){

#ifdef __linux__
	int angularSpeed;
	int encoderCount;

	angularSpeed = (angle > 0) ? speed : -speed;
	encoderCount = rint( angle* wheelBase / (2*wheelRadius) ) ;

	resetEncoders();											// Reset the encoder values to 0

	while (readEncoderValueLeft() < encoderCount) {						// Check the value of encoder 1 and stop after it has traveled a set distance
    //printf("Left: %lu , Right: %lu \n",readEncoderValueLeft(), readEncoderValueRight());
		driveMotors(0, angularSpeed);
		usleep(2000);
	}
  stopMotors();

#else
	cout << "Turning given angle." << endl;
#endif

}

float Motion::getHeading(void){
	float heading = 9999;
#ifdef __linux__
	// Read encoders
	long int counterLeft = readEncoderValueLeft();
	long int counterRight = readEncoderValueRight();
	// get difference
	int counterDiff = counterRight - counterLeft;
	
	//calculate heading in angles
	heading = wheelRadius / wheelBase * counterDiff;
	// wrap to range (-180;180]
	heading = ((heading + 180) % 360) - 180;
#else 
	cout << "[ INFO ] Returning robot heading" << endl;
#endif
	return heading;
}

Motion::~Motion(void){
}
