#include "stdafx.h"
#include "FSDebug.h"
#include "EnumsHeader.h"


FSDebug::FSDebug()
{
	// define state messages
	StateMessages[IDLE] = "IDLE";
	StateMessages[WIGGLING] = "WIGGLE";
	StateMessages[APPROACHING_SIGN] = "APPROACHING";
	StateMessages[EXECUTING_COMMAND] = "EXECUTING";
	StateMessages[MARCHING_FORWARD] = "MARCHING FORWARD";
	StateMessages[GAME_OVER] = "GAME OVER";

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


FSDebug::~FSDebug()
{
}
