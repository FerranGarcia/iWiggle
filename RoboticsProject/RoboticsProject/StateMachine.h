#pragma once

#include "ImageProcessor.h"
#include "Motion.h"
#include "FeedbackModule.h"
#include "EnumsHeader.h"

// when distance bellow this one is reported by distance sensor
// consider that you're close to the wall
#define WALL_DETECTION_DISTANCE 10.0

// when sensor reading has been detected more than this amount of miliseconds ago
// consider that it has been lost
#define SENSOR_TIME_RELIABILITY 3000

class StateMachine
{
	cv::Size cameraImageSize;

	// store desired angle of rotation for COMMAND_EXECUTION state
	// (the detected arrow angle)
	SignInstance *targetSign;

public:
	StateMachine(cv::Size cameraImageSize);
	~StateMachine();

	void FeedSign(SignInstance *inputSign);
	void FeedDistanceSensor(unsigned int distanceReadings);
	void FeedAngle(double currentAngle);

	void Tick();

	// PROPERTIES
	double resultingAngular, resultingLinear;
	StateEnum currentState;

	double lastKnownAngle;
	SignInstance *lastSeenSign;
	Motion motion;
	FeedbackModule *feedbackModule;
};

