#pragma once

enum FeedbackMessageSetEnum {
	FEEDBACK_DEBUG_MSGS = 0,
	FEEDBACK_HUMAN_VOICE_PLAIN,
	FEEDBACK_NO_HUMAN_VOICE_PLAIN,
	FEEDBACK_FUNNY
};

enum StateEnum
{
	IDLE = 0, // speeds = 0, waiting for the sign to go away from view
	WIGGLING, // rotating with linear speed = 0, waiting for sign to be put in view
	APPROACHING_SIGN, // linear speed = constant forward, adjusting angular speed to keep sign in the center
	EXECUTING_COMMAND, // approached the sign, rotating to align with angle
	MARCHING_FORWARD, // angular speed = 0, linear = constant
	GAME_OVER // game finished, robot doesn't do anything
};


