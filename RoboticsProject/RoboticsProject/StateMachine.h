#pragma once

#include "ImageProcessor.h"

enum StateEnum
{
	IDLE,
	WIGGLING,
	APPROACHING_SIGN,
	EXECUTING_COMMAND
};

class StateMachine
{
	double resultingAngular, resultingLinear;
	StateEnum currentState;

public:
	StateMachine();
	~StateMachine();


};

