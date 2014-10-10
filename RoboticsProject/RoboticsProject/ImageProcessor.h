#pragma once

#include "opencv2\opencv.hpp"

using namespace cv;

// types of objects
enum SignTypeEnum
{
	NO_SIGN,
	STOP,
	WAYPOINT,
	ARROW
};

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();

	Mat *getThresholdedImage(Mat *input);
	vector<Point>* getLocationOfObject(Mat *binaryInput); // returns null if no object is found
	SignTypeEnum recognizeSign(Mat* croppedInput, vector<Point> *contour);

	double getArrowAngle(vector<Point> *contour, Mat *croppedImage); 
	Point2f getMassCenter(Mat *croppedImage);
};

