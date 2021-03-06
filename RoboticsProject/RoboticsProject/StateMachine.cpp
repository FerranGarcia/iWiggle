#include "stdafx.h"
#include "StateMachine.h"

StateMachine::StateMachine(cv::Size cameraImageSize)
{
	this->currentState = WIGGLING;
	this->lastSeenSign = NULL;
	// save image size for control of the angular speed
	this->cameraImageSize = cameraImageSize;
	motion = Motion();

	// instantiate message set for the feedback
	feedbackModule = new FeedbackModule(FEEDBACK_NO_HUMAN_VOICE_PLAIN);
}

StateMachine::~StateMachine()
{
}

void StateMachine::FeedSign(SignInstance *inputSign) {

	if (this->lastSeenSign == NULL) delete this->lastSeenSign;
	this->lastSeenSign = inputSign;

}
void StateMachine::FeedDistanceSensor(unsigned int distanceReadings) {
	// if we get close to the wall, start wiggling
	if (distanceReadings < WALL_DETECTION_DISTANCE) {
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
			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, IDLE);

			currentState = MARCHING_FORWARD;
		}

		break;

	case WIGGLING :

		// rotate right
		resultingLinear = 0;
		resultingAngular = 4;

		// STATE SWITCH CONDITION
		// if last seen sign is reliable, start approaching it
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_DETECTION) {
			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, APPROACHING_SIGN);

			this->currentState = APPROACHING_SIGN;
		}

		break;

	case APPROACHING_SIGN :

		if (lastSeenSign != NULL) {
			// compute the resulting linear speed
			this->resultingLinear = 8;
			// compute the resulting angular speed
			double cameraHorCenter = this->cameraImageSize.width / 2;
			this->resultingAngular = -(lastSeenSign->location.x + lastSeenSign->location.width / 2 - cameraHorCenter)
				* (5.0 / cameraHorCenter);
		}

		// STATE SWITCH CONDITION
		// if last seen sign is inside close area, start executing command
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_CLOSE) {
			// allocate memory for the new sign, since this->lastSign gets deleted after 3 seconds
			this->targetSign = new SignInstance(lastSeenSign);

			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, EXECUTING_COMMAND);

			// start executing the command
			this->currentState = EXECUTING_COMMAND;
		}

		break;
	case EXECUTING_COMMAND :

		switch (this->targetSign->signType) {
		case STOP :

			motion.stopMotors();
#ifdef __linux__
			usleep(5000000); // wait 5 seconds for the next function call
#endif
			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, MARCHING_FORWARD);

			this->currentState = MARCHING_FORWARD;
			break;

		case WAYPOINT :

			std::cout << "//--------- GOAL REACHED! ----------//" << std::endl;

			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, GAME_OVER);

			this->currentState = GAME_OVER;
			break;

		case ARROW :
			
			double angleToTurn = this->lastSeenSign->arrowAngle - 90;
			angleToTurn = motion.constrainAngle(angleToTurn);
			std::cout << "//-------Angle to turn: " << angleToTurn << std::endl;

			float headingCurrent = motion.getHeading();
			this->resultingAngular = 5;

			std::cout << "Current robot Heading: " << headingCurrent << std::endl;
#ifdef __linux__
			usleep(1000000); // wait 1 seconds for the next function call
#endif
			std::cout << "//--------- Angle turn started --------//" << std::endl;
			motion.turnAngle(angleToTurn, 5);
			std::cout << "//--------- Angle turn completed ------//" << std::endl;
#ifdef __linux__
			usleep(1000000); // wait 1 seconds for the next function call
#endif

			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, MARCHING_FORWARD);

			this->resultingAngular = 0;
			this->currentState = MARCHING_FORWARD;
			break;
		}

		break;

	case MARCHING_FORWARD :

		this->resultingAngular = 0;
		this->resultingLinear = 8;

		// check exit conditions
		if (lastSeenSign != NULL && lastSeenSign->signArea == SIGN_AREA_DETECTION) {
			// broadcast notifications
			feedbackModule->BroadcastInfo(currentState, APPROACHING_SIGN);

			this->currentState = APPROACHING_SIGN;
		}

		break;

	case GAME_OVER:
		exit(0);
		break;
	}

#ifdef __linux__
	// Assign computed speeds by the State Machine to the motors
	motion.driveMotors(this->resultingLinear, this->resultingAngular);
	this->lastKnownAngle = motion.getHeading();
	std::cout << "[ OUT ] Last known angle: " << this->lastKnownAngle << std::endl;
#endif
}
