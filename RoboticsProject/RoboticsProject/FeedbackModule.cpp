#include "stdafx.h"
#include "FeedbackModule.h"
#include "FSDebug.h"

FeedbackModule::FeedbackModule(FeedbackMessageSetEnum messageSet)
{
	switch (messageSet) {
	case FEEDBACK_DEBUG_MSGS:
		feedbackMessageSet = new FSDebug();
		break;

	case FEEDBACK_HUMAN_VOICE_PLAIN :

		break;

	case FEEDBACK_NO_HUMAN_VOICE_PLAIN :

		break;

	case FEEDBACK_FUNNY :

		break;
	}
}

FeedbackModule::~FeedbackModule()
{
}

void FeedbackModule::BroadcastInfo(StateEnum oldState, StateEnum newState) {

	// take correct messages
	string lcdMessage = this->feedbackMessageSet->StateMessages[newState];
	string notificationSoundFile = this->feedbackMessageSet->NotificationSoundFiles[newState];
	//string backMusicSoundFile = this->feedbackMessageSet->BackgroundMusicFiles[newState];

	// output to LCD
	lcdDriver.sendText(lcdMessage.c_str());

	// PLAY AUDIO HERE !!!
}
