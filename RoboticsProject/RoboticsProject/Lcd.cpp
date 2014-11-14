#include "Lcd.h"

Lcd::Lcd(void)
{
	this->fileName = (char *)"/dev/i2c-1";
	this->address = 0x63;

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

	buf[0] = 0;													// Commands to clear the screen
	buf[1] = 12;
	
	if ((write(fd, buf, 2)) != 2) {								// clear the screen
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#endif
}

void Lcd::sendText(const char* text){

	// textString1 will be the top line of text, the leading space is sent to the LCD03
	// because text must be sent to the command register (0), however if a non zero is recieved
	// first anything after that point is displayed on the screen. The leading space
	// ensures all text will be displayed.

#ifdef __linux__
	if ((write(fd, text, strlen(text))) != strlen(text)) {					
		printf("Error writing to i2c slave\n");
		exit(1);
	}
#endif
}

Lcd::~Lcd(void)
{}
