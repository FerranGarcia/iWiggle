#pragma once

#include "Lcd.h"
#include "FSInterface.h"
#include "EnumsHeader.h"

class FeedbackModule
{
public:
	FeedbackModule(FeedbackMessageSetEnum messageSet);
	~FeedbackModule();

	// SOUND DRIVER
	Lcd lcdDriver;

	FSInterface *feedbackMessageSet;

	void BroadcastInfo(StateEnum oldState, StateEnum newState);
};

