#pragma once

#include "opencv2\opencv.hpp"
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
	Point2f getMassCenter(Mat *croppedImage);

	cv::Rect perceptionArea, proximityArea;
};

