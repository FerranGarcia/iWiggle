#include "stdafx.h"
#include "ImageProcessor.h"


ImageProcessor::ImageProcessor()
{
}


ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::getThresholdedImage(Mat *input, Mat* result) {
	
	//resize(*input, *input, Size(320, 240));

	GaussianBlur(*input, *input, Size(1, 1), 1.0);

	double erosion_size = 3;

	Mat *input_hsv = new Mat();

	//convert BGR image to HSV for segmentation
	cvtColor(*input, *input_hsv, CV_BGR2HSV);

	vector<Mat> channels;
	split(*input_hsv, channels);
	Mat channelH = channels[0];;
	Mat channelS = channels[1];
	Mat channelV = channels[2];

	Mat hue1, hue2, sat, val1, val2;
	threshold(channelH, hue1, 5, 255, THRESH_BINARY_INV);
	threshold(channelH, hue2, 170, 255, THRESH_BINARY);

	threshold(channelS, sat, 100, 255, THRESH_BINARY);

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

vector<Point>* ImageProcessor::getLocationOfObject(Mat *binaryInput) {

	vector<Point> *ret = NULL;

	double min_area_threshold = binaryInput->size().area() * 0.01;

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

	returnSign->centerOfMass = getMassCenter(croppedInput);
	returnSign->centerOfImage = cv::Point2f(croppedInput->size().width / 2, croppedInput->size().height / 2);

	double distance = sqrt(pow(returnSign->centerOfImage.x - returnSign->centerOfMass.x, 2) + pow(returnSign->centerOfImage.y - returnSign->centerOfMass.y, 2));

	if (distance > croppedInput->size().width * 0.05) {
		returnSign->signType = ARROW;
	}
	else { // distinguish between circle and cross
		double total_area = croppedInput->size().area();
		double contour_area = cvCountNonZero(new IplImage(*croppedInput));
		double ratio = contour_area / total_area;

		std::cout << "Area ratio is " << ratio << std::endl;

		if (ratio > 0.6) returnSign->signType = WAYPOINT;
		else returnSign->signType = STOP;
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




