#pragma once

#include "ImageProcessor.h"
#include "Motion.h"

// when distance bellow this one is reported by distance sensor
// consider that you're close to the wall
#define WALL_DETECTION_DISTANCE 10.0

// when sensor reading has been detected more than this amount of miliseconds ago
// consider that it has been lost
#define SENSOR_TIME_RELIABILITY 3000

enum StateEnum
{
	IDLE, // speeds = 0, waiting for the sign to go away from view
	WIGGLING, // rotating with linear speed = 0, waiting for sign to be put in view
	APPROACHING_SIGN, // linear speed = constant forward, adjusting angular speed to keep sign in the center
	EXECUTING_COMMAND, // approached the sign, rotating to align with angle
	MARCHING_FORWARD, // angular speed = 0, linear = constant
	GAME_OVER // game finished, robot doesn't do anything
};

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
};

