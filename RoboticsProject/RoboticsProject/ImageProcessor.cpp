#include "stdafx.h"
#include "ImageProcessor.h"


ImageProcessor::ImageProcessor()
{
}


ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::getThresholdedImage(Mat *input, Mat* result) {
	
	//???No need to resize image since we resizing input from PiCam
	if ((*input).cols > 320){
		resize(*input, *input, Size(320, 240));
	}

	GaussianBlur(*input, *input, Size(1, 1), 1.0);

	double erosion_size = 1;

	Mat *input_hsv = new Mat();

	//convert BGR image to HSV for segmentation
	cvtColor(*input, *input_hsv, CV_BGR2HSV);

	vector<Mat> channels;
	split(*input_hsv, channels);
	Mat channelH = channels[0];;
	Mat channelS = channels[1];
	Mat channelV = channels[2];

	Mat hue1, hue2, sat, val1, val2;
	threshold(channelH, hue1, 10, 255, THRESH_BINARY_INV);
	threshold(channelH, hue2, 170, 255, THRESH_BINARY);

	threshold(channelS, sat, 40, 255, THRESH_BINARY);

	//equalizeHist(channelV, channelV);

	threshold(channelV, val1, 40, 255, THRESH_BINARY);
	threshold(channelV, val2, 230, 255, THRESH_BINARY_INV);

	// combine results
	*result = Mat((hue1 | hue2) & sat & val1 & val2);
	
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(*result, *result, element);
	dilate(*result, *result, element);

	dilate(*result, *result, element);
	erode(*result, *result, element);

	delete input_hsv;

}

void ImageProcessor::getThresholdedImageYCrCb(Mat *input, Mat* result) {

	double threshConst = 0.6;
	double erosion_size = 1;

	//???No need to resize image since we resizing input from PiCam
	if ( (*input).cols > 320){
		resize(*input, *input, Size(320, 240));
	}
	
	Mat *input_YCrCb = new Mat();

	//convert BGR image to YCrCb for segmentation
	cvtColor(*input, *input_YCrCb, CV_BGR2YCrCb);

	// Splitting.
	vector<Mat> channels;
	split(*input_YCrCb, channels); //channels[1] = channelCr
	
	// Thresholding
	*result = channels[1] > 255 * threshConst;

	// Structuring element for closing
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(*result, *result, element);
	dilate(*result, *result, element);

	delete input_YCrCb;
}

vector<Point>* ImageProcessor::getLocationOfObject(Mat *binaryInput) {

	vector<Point> *ret = NULL;

	double min_area_threshold = binaryInput->size().area() * 0.005;

	int largest_area = 0;
	int largest_contour_index = 0;

	vector< vector<Point> > contours; // Vector for storing contour
	vector<Vec4i> hierarchy;

	Mat copyOfInput;
	binaryInput->copyTo(copyOfInput);
	findContours(copyOfInput, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE); // Find the contours in the image

	for (int i = 0; i< contours.size(); i++) {// iterate through each contour. 
		double a = contourArea(contours[i], false);  //  Find the area of contour

		// check the size
		if (a < min_area_threshold) continue;

		if (a > largest_area){
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
			ret = new vector<Point>(contours[i]); // Find the bounding rectangle for biggest contour
		}
	}

	return ret;
}

SignInstance* ImageProcessor::recognizeSign(Mat* croppedInput, vector<Point> *contour) {

	SignInstance *returnSign = new SignInstance();
	returnSign->thresholdedMat = new Mat(*croppedInput);

	returnSign->centerOfMass = getMassCenter(croppedInput);
	returnSign->centerOfImage = cv::Point2f(croppedInput->size().width / 2, croppedInput->size().height / 2);

	double distance = sqrt(pow(returnSign->centerOfImage.x - returnSign->centerOfMass.x, 2) + pow(returnSign->centerOfImage.y - returnSign->centerOfMass.y, 2));

	if (distance > croppedInput->size().width * 0.05) {
		returnSign->signType = ARROW;
		returnSign->arrowAngle = this->getArrowAngle(contour, croppedInput);
	}
	else { // distinguish between circle and cross
		double total_area = croppedInput->size().area();
		double contour_area = cvCountNonZero(new IplImage(*croppedInput));
		double ratio = contour_area / total_area;

		std::cout << "Area ratio is " << ratio << std::endl;

		if (ratio > 0.65) returnSign->signType = WAYPOINT;
		else returnSign->signType = STOP;
	}

	returnSign->location = boundingRect(*contour);

	// compute the area in which the sign is
	cv::Point2f massGlobalPos(returnSign->location.x + returnSign->centerOfMass.x, returnSign->location.y + returnSign->centerOfMass.y);
	returnSign->signArea = SIGN_AREA_FAR; 
	if (this->perceptionArea.contains(massGlobalPos)) {
		returnSign->signArea = SIGN_AREA_DETECTION;
	}
	if (this->proximityArea.contains(massGlobalPos)) {
		returnSign->signArea = SIGN_AREA_CLOSE;
	}

	return returnSign;
}

Point2f ImageProcessor::getMassCenter(Mat *croppedImage) {
	// compute moment
	Moments mu = moments(*croppedImage);
	Point2f center_of_mass = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	return center_of_mass;
}


float radian_to_degree(double  ENTER) {
	double Pi = 3.14159265;
	float degrees = (ENTER * 180) / Pi;
	return degrees;
}

double ImageProcessor::getArrowAngle(vector<Point> *contour, Mat *croppedImage) {

	Rect boundingBox = boundingRect(*contour);

	Point2f center_of_mass = getMassCenter(croppedImage);
	
	double angle = atan2(croppedImage->size().height / 2 - center_of_mass.y, center_of_mass.x - croppedImage->size().width / 2);
	
	// convert to degrees
	angle = radian_to_degree(angle);

	// correct negative angles
	if (angle < 0) angle += 360;

	return angle;
}

double ImageProcessor::getArrowAngleFitLine(vector<Point> *contour, Rect sign_location, Point2f mass_center){

	Vec4f newLine;
	Vec3f data;
	Point2f P0;		//point that lies on the line
	Point2f P1;		//(center of rectangle) - P0
	Point2f P2;		//(center of mass) - P0
	Point2f rectangleProj;
	Point2f massProj;

	fitLine(*contour, newLine, CV_DIST_L2, 0, 0.01, 0.01);
	float vx = newLine[0];
	float vy = newLine[1];
	float x = newLine[2];
	float y = newLine[3];
	int lefty = int((-x*vy / vx) + y);
	int righty = int(((320 - x)*vy / vx) + y);
	//std::cout << vx << " " << vy << " " << x << " " << y << " " << lefty << " " << righty << std::endl;
	float angle = atan2((lefty - righty), 320);

	angle = radian_to_degree(angle);

	/*--------- PROJECTION OF RECTANGLE CENTER AND CENTER OF MASS INTO THE LINE ----------------
	 * In order to project the two points we will use some linear algebra
	 *
	 *    v * v'
	 *   ------- P1 + P0 = projected point
	 *    v'* v
	 *
	 * where: - v is the vector that represents the line
	 *        - P1 is the point we want to project shifted with respects one point of the line
	 *		  - P0 is the point of the line were we take our 'imaginary' (0,0)
	 *------------------------------------------------------------------------------------------*/

	//we know that our line crosses data[1] (left Y) at x=0
	P0.x = 0;
	P0.y = -lefty;	//lefty

	P1.x = (sign_location.x + sign_location.width / 2) - P0.x;		//shifting the (0,0) to a known point (P0) of the line
	P1.y = -(sign_location.y + sign_location.height / 2) - P0.y;	//shifting the (0,0) to a known point (P0) of the line

	P2.x = mass_center.x + sign_location.x - P0.x;					//shifting the (0,0) to a known point (P0) of the line
	P2.y = -(mass_center.y + sign_location.y) - P0.y;				//shifting the (0,0) to a known point (P0) of the line

	float vec1 = 319;					//vector which describes the line
	float vec2 = -(righty - lefty);		//note that we need to change the sign since (0,0) is on the top-left corner. Y-axis is reversed

	// v*v' gives a matrix as a result which is computed below
	//float mat11 = pow(vec1, 2);
	//float mat12 = vec1 * vec2;
	float mat21 = vec1 * vec2;
	float mat22 = pow(vec2, 2);

	//v'*v is a scalar normalisation factor of the matrix
	float normalisation = pow(vec1, 2) + pow(vec2, 2);

	//mat11 = mat11 / normalisation;
	//mat12 = mat12 / normalisation;
	mat21 = mat21 / normalisation;
	mat22 = mat22 / normalisation;

	// Projection of the points undoind the shift previously done. Note that only Y is necessary to be corrected since P0.x = 0
	//rectangleProj.x = mat11*P1.x + mat12*P1.y;
	rectangleProj.y = -(mat21*P1.x + mat22*P1.y) - P0.y;

	//massProj.x = mat11*P2.x + mat12*P2.y;
	massProj.y = -(mat21*P2.x + mat22*P2.y) - P0.y;

	//correction of the angle using the information of the projected points
	if (angle < 0)
	{
		angle = angle + 180;
	}
	if (massProj.y > rectangleProj.y)
	{
		angle = angle + 180;
	}

	return angle;

}


Mat ImageProcessor::getDefaultPerspectiveTransform()
{
	Point2f source_points[4], dest_points[4];
	int IMG_WIDTH = 320;
	int IMG_HEIGHT = 240;

	//top points
	source_points[0] = Point2f(0.0, 0.0);
	source_points[1] = Point2f(IMG_WIDTH, 0.0);
	//bottom points
	source_points[2] = Point2f(IMG_WIDTH, IMG_HEIGHT);
	source_points[3] = Point2f(0.0, IMG_HEIGHT);

	//top points
	dest_points[0] = Point2f(0.0, 0.0);
	dest_points[1] = Point2f(IMG_WIDTH, 0.0);
	//bottom points
	dest_points[2] = Point2f(IMG_WIDTH - 0.0, IMG_HEIGHT);
	dest_points[3] = Point2f(0, IMG_HEIGHT);

	Mat transform_matrix;
	transform_matrix = getPerspectiveTransform(source_points, dest_points);
	return transform_matrix;
}

