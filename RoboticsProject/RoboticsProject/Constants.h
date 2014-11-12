
class Constants {

public:
	Constants() {};
	Constants(char* filename);
	~Constants() {};

	/* Integer constants */
	static int IMG_WIDTH;			/* Image acquisition width (RPi) / Image resize width (Webcam)*/
	static int IMG_HEIGHT;			/* Image acquisition height (RPi) / Image resize height (Webcam)*/
	static int FOC_WIDTH;			/* Focus window width */
	static int FOC_HEIGHT;			/* Focus window height */

	static int THRESH_H_LOW;		/* Lower threshold for Hue channel */
	static int THRESH_H_HIGH;		/* Higher threshold for Hue channel */
	static int THRESH_S_LOW;		/* Lower threshold for Saturation channel */
	static int THRESH_V_LOW;		/* Lower value for Value channel */
	static int THRESH_V_HIGH;		/* Higher value for Value channel */
	static int EROSION_SIZE;		/* Erosion size for the binary image */

	static int VEL_LINEAR;			/* Linear velocity value */
	static int VEL_ANGULAR;			/* Angular velocity value */
	static int VEL_SCALE;			/* Velucity scale (up/down) */

	static int ADDR_LCD;			/* I2C LCD Screen address */
	static int ADDR_DISTS;			/* I2C Distance sensor address */
	static int ADDR_MOTION;			/* I2C Motor controller address*/

	/* Double constants*/
	static double AREA_RATIO;		/* Area ration for detection */

};

int loadConstants(char*);


