// RoboticsProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "opencv2\opencv.hpp"

#include "ImageProcessor.h"

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

	Mat thresholded;
	vector<Point> *sign_contour;


	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera

		/*
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		*/

		imageProcessor.getThresholdedImage(&frame, &thresholded);
		sign_contour = imageProcessor.getLocationOfObject(&thresholded);
		Mat focus(Size(128, 128), CV_8UC3);

		if (sign_contour != NULL) { // object was found

			// isolate object
			Rect sign_location = boundingRect(*sign_contour);
			rectangle(frame, sign_location, Scalar(255, 0, 0), 2);
			Mat cropped_binary = (thresholded)(sign_location);

			// check which object it is
			SignTypeEnum object_type = imageProcessor.recognizeSign(&cropped_binary, sign_contour);
			Point2f mass_center = imageProcessor.getMassCenter(&cropped_binary);
			focus = frame(sign_location);
			double arrow_angle = 0;

			// handle recognition ofdifferent object
			cout << "---> DETECTED OBJECT: ";
			
			switch (object_type)
			{
			case SignTypeEnum::WAYPOINT:
				cout << "CIRCLE" << endl;
				break;
			case SignTypeEnum::ARROW:

				cout << "ARROW [" << endl;

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
		
		cv::imshow("thresholded", thresholded);
		cv::imshow("original", frame);

		if (waitKey(30) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}

