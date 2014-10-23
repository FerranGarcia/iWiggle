#include "stdafx.h"
#include "SignInstance.h"


SignInstance::SignInstance()
{
	// default sign type: NO_SIGN
	this->signType = NO_SIGN;

	// record the time of apearance (constructor call)
	this->timestamp = clock();

	reliabilityCount = 0;
}


SignInstance::~SignInstance()
{
}

SignInstance::SignInstance(SignInstance *original) {

	this->signType = original->signType;
	this->location = cv::Rect(original->location);
	//thresholdedMat = new cv::Mat(*(original->thresholdedMat));
	//rawImage = new cv::Mat(*(original->rawImage));

	timestamp = original->timestamp;

	signArea = original->signArea;

	centerOfMass = cv::Point2f(original->centerOfMass);
	centerOfImage = cv::Point2f(original->centerOfImage);

	reliabilityCount = original->reliabilityCount;

	arrowAngle = original->arrowAngle;
}
