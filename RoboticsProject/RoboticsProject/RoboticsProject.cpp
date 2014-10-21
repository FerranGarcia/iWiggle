// RoboticsProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "opencv2\opencv.hpp"

#include "ImageProcessor.h"
#include "StateMachine.h"
#include "SignInstance.h"

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return -1;

	Mat edges;
	namedWindow("focus", 3);
	namedWindow("original", 2);
	namedWindow("thresholded", 1);

	// instantiate object for image processing
	ImageProcessor imageProcessor;
	imageProcessor.perceptionArea = cv::Rect(50, 50, 210, 110);
	imageProcessor.proximityArea = cv::Rect(50, 160, 210, 70);

	StateMachine stateMachine(cv::Size(320, 240));

	Mat thresholded;
	vector<Point> *sign_contour;

	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera

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
			SignInstance* detectedSign = imageProcessor.recognizeSign(&cropped_binary, sign_contour);

			stateMachine.FeedSign(detectedSign);
			//stateMachine.FeedDistanceSensor(distances);
			//stateMachine.FeedAngle(angle);

			// --------------------- PERFORM STATE MACHINE TICK ---------------------//
			stateMachine.Tick();

			// ----------------------------- SHOW OUTPUT ----------------------------//

			cout << "[ OUT ] CURRENT STATE: ";
			switch (stateMachine.currentState) {
				case IDLE: cout << "IDLE" << endl; break;
				case WIGGLING: cout << "WIGGLING" << endl; break;
				case APPROACHING_SIGN: cout << "APPROACHING_SIGN" << endl; break;
				case EXECUTING_COMMAND: cout << "EXECUTING_COMMAND" << endl; break;
			}
			cout << "[ OUT ] Resulting linear speed: " << stateMachine.resultingLinear << endl;
			cout << "[ OUT ] Resulting angular speed: " << stateMachine.resultingAngular << endl;

			Point2f mass_center = detectedSign->centerOfMass;
			focus = frame(sign_location);
			double arrow_angle = 0;

			// handle recognition ofdifferent object
			cout << "---> DETECTED OBJECT: ";
			
			switch (detectedSign->signType)
			{
			case SignTypeEnum::WAYPOINT:
				cout << "CIRCLE" << endl;
				break;
			case SignTypeEnum::ARROW:

				cout << "ARROW [" ;

				// suppose it's an arrow, extract angle
				arrow_angle = imageProcessor.getArrowAngle(sign_contour, &cropped_binary);
				cout << arrow_angle << " ]" << endl;

				break;

			case SignTypeEnum::STOP:
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
		
		// draw perceptive area
		rectangle(frame, imageProcessor.perceptionArea, cv::Scalar(0, 255, 0));
		// draw proximity area
		rectangle(frame, imageProcessor.proximityArea, cv::Scalar(0, 0, 255));
		cv::imshow("thresholded", thresholded);
		cv::imshow("original", frame);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

