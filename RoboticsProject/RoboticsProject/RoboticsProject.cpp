// RoboticsProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "ImageProcessor.h"
#include "StateMachine.h"
#include "SignInstance.h"
#include "NNHelpers.h"
#include "Motion.h"
#include "distSensor.h"

#ifdef __linux__
#include "opencv.hpp"
#include "raspicam_cv.h"
#else 
#include "opencv2\opencv.hpp"
#endif

using namespace cv;
using namespace std;

#ifdef __linux__
int main(int argc, char* argv[])
#else
int _tmain(int argc, _TCHAR* argv[])
#endif
{
#ifdef __linux__
		// Raspberry Pi Camera Test Code
	raspicam::RaspiCam_Cv rpicam;
	rpicam.set(CV_CAP_PROP_FRAME_WIDTH,160);
	rpicam.set(CV_CAP_PROP_FRAME_HEIGHT,120);
	//rpicam.set(CV_CAP_PROP_SATURATION, 100);
	if (!rpicam.open()) {
		cerr<<"Error opening the camera"<<endl;
		return -1;
	}
#else
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;
#endif
	
	Mat edges;
	namedWindow("focus", 3);
	namedWindow("original", 2);
	namedWindow("thresholded", 1);

	// instantiate object for image processing
	ImageProcessor imageProcessor;
	imageProcessor.perceptionArea = cv::Rect(50, 50, 210, 110);
	imageProcessor.proximityArea = cv::Rect(50, 160, 210, 70);

	StateMachine stateMachine(cv::Size(320, 240));

#ifdef __linux__
	distSensor distSensor;
#endif

	

	Mat thresholded;
	vector<Point> *sign_contour;
	SignInstance* detectedSign = NULL;

	for (;;)
	{
		Mat frame;
#ifdef __linux__
		rpicam.grab();
		rpicam.retrieve(frame);
#else
		cap >> frame; // get a new frame from camera
#endif
		imageProcessor.getThresholdedImage(&frame, &thresholded);
		sign_contour = imageProcessor.getLocationOfObject(&thresholded);
		Mat focus(Size(128, 128), CV_8UC3);

		if (sign_contour != NULL) { // object was found

			// -------------------------- PROCESS THE INPUT ------------------------//
			// isolate object
			Rect sign_location = boundingRect(*sign_contour);
			rectangle(frame, sign_location, Scalar(255, 0, 0), 2);
			Mat cropped_binary = (thresholded)(sign_location);

			// check which object it is
			detectedSign = imageProcessor.recognizeSign(&cropped_binary, sign_contour);
			stateMachine.FeedSign(detectedSign);
			//stateMachine.FeedDistanceSensor(distances);
			//stateMachine.FeedAngle(angle);


			Point2f mass_center = detectedSign->centerOfMass;
			focus = frame(sign_location);
			double arrow_angle = 0;

			// handle recognition ofdifferent object
			cout << "---> DETECTED OBJECT: ";
			
			switch (detectedSign->signType)
			{
			case WAYPOINT:
			//case SignTypeEnum::WAYPOINT:
				cout << "CIRCLE" << endl;
				break;
			case ARROW:
				cout << "ARROW [" << detectedSign->arrowAngle << " ]" << endl;
				break;
			case STOP:
				cout << "STOP" << endl;
				break;
			default:
				break;
			}

			CvPoint ptImageCenter = cvPoint(focus.size().width / 2, focus.size().height / 2);
			CvPoint ptMassCenter = cvPoint(mass_center.x, mass_center.y);

			circle(focus, ptMassCenter, 5, Scalar(0, 255, 0));
			circle(focus, ptImageCenter, 3, Scalar(0, 0, 255));
			line(focus, ptImageCenter, ptMassCenter, Scalar(0, 255, 0), 2);

			// stabilize image of an object
			resize(focus, focus, Size(128, 128));
			cv::imshow("focus", focus);

		}


		// --------------------- PERFORM STATE MACHINE TICK ---------------------//
		stateMachine.Tick();
		// ----------------------------- SHOW OUTPUT ----------------------------//
		cout << "[ OUT ] CURRENT STATE: ";
		switch (stateMachine.currentState) {
			case IDLE: cout << "IDLE" << endl; break;
			case WIGGLING: cout << "WIGGLING" << endl; break;
			case APPROACHING_SIGN: cout << "APPROACHING_SIGN" << endl; break;
			case EXECUTING_COMMAND: cout << "EXECUTING_COMMAND" << endl; break;
			case MARCHING_FORWARD: cout << "MARCHING_FORWARD" << endl; break;
			case GAME_OVER: cout << "GAME_OVER" << endl; break;
			default: cout << "Unknown state" << endl; break;
		}
		cout << "[ OUT ] Resulting linear speed: " << stateMachine.resultingLinear << endl;
		cout << "[ OUT ] Resulting angular speed: " << stateMachine.resultingAngular << endl;


		
		// draw perceptive area
		rectangle(frame, imageProcessor.perceptionArea, cv::Scalar(0, 255, 0));
		// draw proximity area
		rectangle(frame, imageProcessor.proximityArea, cv::Scalar(0, 0, 255));
		cv::imshow("thresholded", thresholded);
		cv::imshow("original", frame);

		char key_code;
		string file_path = "C:/Users/Igor/Documents/iwiggle/RoboticsProject/Debug/groundtruth/";
		if ((key_code = waitKey(30)) >= 0) {

			bool saving = false;

			// check which key is pressed
			switch (key_code) {
				// Q - arrow
				case 'q':
					//cout << "[KEY] Q" << endl;
					file_path += "arrow/";
					saving = true;
					break;

				// W - cross
				case 'w':
					file_path += "cross/";
					saving = true;
					//cout << "[KEY] W" << endl;
					break;

				// E - circle
				case 'e':
					file_path += "circle/";
					saving = true;
					//cout << "[KEY] E" << endl;
					break;
			
				// R - no sign
				case 'r':
					file_path += "nosign/";
					saving = true;
					//cout << "[KEY] R" << endl;
					break;

				default:
					break;
			}

			// if image needs to be saved localy (one of the four category keys was pressed
			if (saving) {
				// generate random filename
				string *randomName = NNHelpers::getRandomString(20);
				//cout << "[ FILENAME ]: " << *randomName << endl;
				file_path += *randomName;
				file_path += ".jpg";
				
				// save the image
				if (detectedSign != NULL) {
					cout << " [ FILE ] Writing to: " << file_path << " ---> ";
#ifdef __linux__
#else
					try {
						imwrite(file_path, *detectedSign->thresholdedMat);
					}
					catch (runtime_error& ex) {
						fprintf(stderr, "Exception: %s\n", ex.what());
						return 1;
					}
#endif
				}

				delete randomName;
			}
		}
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
#ifdef __linux__
	rpicam.release();
#endif
	return 0;
}

