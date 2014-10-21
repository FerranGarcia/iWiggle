#pragma once

#define SIGN_RELIABILITY_COUNT 10

#include <ctime>
#include "opencv2\opencv.hpp"

// types of objects
enum SignTypeEnum
{
	NO_SIGN,
	STOP,
	WAYPOINT,
	ARROW
};

enum SignRegion
{
	SIGN_AREA_FAR,
	SIGN_AREA_DETECTION,
	SIGN_AREA_CLOSE
};

class SignInstance
{

public:
	SignInstance();
	~SignInstance();

	SignTypeEnum signType;
	cv::Rect location;
	cv::Mat *thresholdedMat;
	cv::Mat *rawImage;

	clock_t timestamp;

	SignRegion signArea;

	cv::Point2f centerOfMass;
	cv::Point2f centerOfImage;

	int reliabilityCount;
};

