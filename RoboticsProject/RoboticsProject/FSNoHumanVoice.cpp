#include "stdafx.h"
#include "FSNoHumanVoice.h"
#include "EnumsHeader.h"


FSNoHumanVoice::FSNoHumanVoice()
{
	// define state messages
	StateMessages[IDLE] = "OK, let's move on...";
	StateMessages[WIGGLING] = "Please, I need help!";
	StateMessages[APPROACHING_SIGN] = "Oh, what's this?";
	StateMessages[EXECUTING_COMMAND] = "OK, I'm on it...";
	StateMessages[MARCHING_FORWARD] = "I want circle!";
	StateMessages[GAME_OVER] = "Yaaaaaaay! ^^";

	// define sound paths
	NotificationSoundFiles[IDLE] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";
	NotificationSoundFiles[WIGGLING] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";
	NotificationSoundFiles[APPROACHING_SIGN] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";
	NotificationSoundFiles[EXECUTING_COMMAND] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";
	NotificationSoundFiles[MARCHING_FORWARD] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";
	NotificationSoundFiles[GAME_OVER] = "C:\\Users\\Igor\\Music\\bottom_a_sharp.wav";

	// define background music files
	this->BackgroundMusicFiles.push_back("C:\\Users\\Igor\\Music\\bottom_a_sharp.wav");
}


FSNoHumanVoice::~FSNoHumanVoice()
{
}
