#include "stdafx.h"
#include "StateMachine.h"


StateMachine::StateMachine(cv::Size cameraImageSize)
{
	this->currentState = WIGGLING;

	// save image size for control of the angular speed
	this->cameraImageSize = cameraImageSize;
}

StateMachine::~StateMachine()
{
}

void StateMachine::FeedSign(SignInstance *inputSign) {

	if (this->lastSeenSign == NULL) delete this->lastSeenSign;
	this->lastSeenSign = inputSign;

}
void StateMachine::FeedDistanceSensor(vector<double> distanceReadings) {
	// if we get close to the wall, start wiggling
	if (distanceReadings[0] < WALL_DETECTION_DISTANCE) {
		this->currentState = WIGGLING;
	}
}
void StateMachine::FeedAngle(double currentAngle) {

	// store last reported angle localy
	this->lastKnownAngle = currentAngle;

}

void StateMachine::Tick() {

	// ensure that last seen sign is recently seen
	if (this->lastSeenSign != NULL) {
		// compute miliseconds
		double signAge = double(clock() - lastSeenSign->timestamp) / CLOCKS_PER_SEC;
		if (signAge > SENSOR_TIME_RELIABILITY) {
			delete lastSeenSign;
			lastSeenSign = NULL;
		}
	}
	switch (this->currentState) {

	case IDLE:

		this->resultingAngular = 0;
		this->resultingLinear = 0;

		// state exit condition
		if (this->lastSeenSign == NULL) {
			currentState = MARCHING_FORWARD;
		}

		break;

	case WIGGLING :

		// rotate right
		resultingLinear = 0;
		resultingAngular = 100;

		// STATE SWITCH CONDITION
		// if last seen sign is reliable, start approaching it
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_DETECTION) {
			this->currentState = APPROACHING_SIGN;
		}

		break;

	case APPROACHING_SIGN :

		if (lastSeenSign != NULL) {
			// compute the resulting linear speed
			this->resultingLinear = 70;
			// compute the resulting angular speed
			double cameraHorCenter = this->cameraImageSize.width / 2;
			this->resultingAngular = (lastSeenSign->location.x + lastSeenSign->location.width / 2 - cameraHorCenter)
				* (100.0 / cameraHorCenter);
		}

		// STATE SWITCH CONDITION
		// if last seen sign is inside close area, start executing command
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_CLOSE) {
			// allocate memory for the new sign, since this->lastSign gets deleted after 3 seconds
			this->targetSign = new SignInstance(lastSeenSign);
			// start executing the command
			this->currentState = EXECUTING_COMMAND;
		}

		break;
	case EXECUTING_COMMAND :

		switch (this->targetSign->signType) {
		case STOP :

			break;

		case WAYPOINT :

			std::cout << "//--------- GOAL REACHED! ----------//" << std::endl;
			this->currentState = GAME_OVER;
			break;
		}


		break;

	case MARCHING_FORWARD :

		this->resultingAngular = 0;
		this->resultingLinear = 100;

		// check exit conditions
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_DETECTION) {
			this->currentState = APPROACHING_SIGN;
		}

		break;

	case GAME_OVER:

		break;
	}

}
