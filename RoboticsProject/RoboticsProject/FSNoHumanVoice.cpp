#include "stdafx.h"
#include "FSNoHumanVoice.h"
#include "EnumsHeader.h"


FSNoHumanVoice::FSNoHumanVoice()
{
	// define state messages
	StateMessages[IDLE] = "  IDLE";
	StateMessages[WIGGLING] = "  Waiting";
	StateMessages[APPROACHING_SIGN] = "  Approaching sign ";
	StateMessages[EXECUTING_COMMAND] = "  Executing";
	StateMessages[MARCHING_FORWARD] = "  Marching forward";
	StateMessages[GAME_OVER] = "  Game Over!";

	// define sound paths
	NotificationSoundFiles[IDLE] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/idle.wav";
	NotificationSoundFiles[WIGGLING] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/wiggling.wav";
	NotificationSoundFiles[APPROACHING_SIGN] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/detection.wav";
	NotificationSoundFiles[EXECUTING_COMMAND] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/executing.wav";
	NotificationSoundFiles[MARCHING_FORWARD] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/forward.wav";
	NotificationSoundFiles[GAME_OVER] = "/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/game_over.wav";

	// define background music files
	this->BackgroundMusicFiles.push_back("/home/pi/Desktop/RoboticsProject/sounds/no_human_voice/forward.wav");
}


FSNoHumanVoice::~FSNoHumanVoice()
{
}
