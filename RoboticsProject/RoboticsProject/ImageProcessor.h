#pragma once

#ifdef __linux__
#include "opencv.hpp"
#else 
#include "opencv2\opencv.hpp"
#endif

#include "SignInstance.h"

using namespace cv;

class ImageProcessor
{
public:
	ImageProcessor();
	~ImageProcessor();

	void getThresholdedImage(Mat *input, Mat* output);
	vector<Point>* getLocationOfObject(Mat *binaryInput); // returns null if no object is found

	SignInstance* recognizeSign(Mat* croppedInput, vector<Point> *contour);

	double getArrowAngle(vector<Point> *contour, Mat *croppedImage); 
	float getArrowAngleFitLine(vector<Point> *contour, Rect sign_location, Point2f mass_center);
	Point2f getMassCenter(Mat *croppedImage);

	cv::Rect perceptionArea, proximityArea;
};

