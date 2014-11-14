// NeuralNet.cpp : Defines the entry point for the console application.
//
#define DEBUG
#include "CBackProp.h"
#include "BuildTrainSet.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	#ifdef _DEBUG
		argc = 3;
		argv[1] = "trainlist.txt";
		argv[2] = "testlist.txt";
	#endif

	/*
	// prepare XOR traing data
	double data[][4]={
				0,	0,	0,	0,
				0,	0,	1,	1,
				0,	1,	0,	1,
				0,	1,	1,	0,
				1,	0,	0,	1,
				1,	0,	1,	0,
				1,	1,	0,	0,
				1,	1,	1,	1 };

	// prepare test data	
	double testData[][3]={
								0,      0,      0,
                                0,      0,      1,
                                0,      1,      0,
                                0,      1,      1,
                                1,      0,      0,
                                1,      0,      1,
                                1,      1,      0,
                                1,      1,      1};

	*/

	//Load the Image Class 1 + Create Matrix + Normalization
	Mat trainingSet;
	BuildTrainSet TrainMat(2, argv[1]);
	trainingSet = TrainMat.ComposeMatrix();
	
	Mat class1 = Mat::ones(trainingSet.rows, 1 ,CV_32F);
	class1.at<float>(1, 0) = 0;
	Mat class2 = Mat::ones(trainingSet.rows, 1, CV_32F);
	class2.at<float>(0, 0) = 0;
	Mat trainingSetData;

	cout << class1.rows << " " << class1.cols << " class matrix" << endl;
	cout << trainingSet.rows << " " << trainingSet.cols << trainingSet.type()<< " training matrix" << endl;
	
	cv::normalize(trainingSet, trainingSet, 0, 1, NORM_MINMAX, -1);
	cv::hconcat(trainingSet, class1, trainingSetData);
	cv::hconcat(trainingSetData, class2, trainingSetData);

	// allocate and initialize a 2d float array
	double **m = new double*[trainingSetData.rows];
	for (int r = 0; r < trainingSetData.rows; ++r)
	{
		m[r] = new double[trainingSetData.cols];
		for (int c = 0; c < trainingSetData.cols; ++c)
		{
			m[r][c] = trainingSetData.at<float>(r, c);

			cout << m[r][c] << " ";
		}
		cout << endl;
	}

	//Load testing
	Mat testSet;
	BuildTrainSet TestMat(argc, argv[2]);
	testSet = TestMat.ComposeMatrix();
	cv::normalize(testSet, testSet, 0, 1, NORM_MINMAX, -1);

	// allocate and initialize a 2d float array
	double **testData = new double*[testSet.rows];
	for (int r = 0; r < testSet.rows; ++r)
	{
		testData[r] = new double[testSet.cols];
		for (int c = 0; c < testSet.cols; ++c)
		{
			testData[r][c] = testSet.at<float>(r, c);
			cout << testData[r][c] << " ";
		}
		cout << endl;
	}
	system("pause");
	// defining a net with 4 layers having 3,3,2, and 1 neuron respectively,
	// the first layer is input layer i.e. simply holder for the input parameters
	// and has to be the same size as the no of input parameters, in our example 3
	int numLayers = 3, lSz[3] = { trainingSetData.cols-2, 100, 2 };

	
	// Learing rate - beta
	// momentum - alpha
	// Threshhold - thresh (value of target mse, training stops once it is achieved)
	//double beta = 0.3, alpha = 0.1, Thresh =  0.00001;
	double beta = 0.3, alpha = 0.1, Thresh = 0.0001;

	
	// maximum no of iterations during training
	long num_iter = 2000000;

	
	// Creating the net
	CBackProp *bp = new CBackProp(numLayers, lSz, beta, alpha);
	
	cout << endl <<  "Now training the network...." << endl;	
	for (int i = 0; i < num_iter; i++)
	{

		bp->bpgt(m[i % trainingSetData.rows], &m[i % trainingSetData.rows][trainingSetData.cols-2]);

		//cout << "Neuron output: " << bp->Out(0) <<  " ............ ";
		double myMSE = bp->mse(&m[i % trainingSetData.rows][trainingSetData.cols-2]);
		cout << "MSE: " << myMSE << endl;
		if (myMSE < Thresh) {
			cout << endl << "Network Trained. Threshold value achieved in " << i << " iterations." << endl;
			cout << "MSE:  " << bp->mse(&m[i % trainingSetData.rows][trainingSetData.cols-2])
				<< endl << endl;
			break;
		}
		if (i % (num_iter / 10) == 0)
			cout << endl << "MSE:  " << bp->mse(&m[i % trainingSetData.rows][trainingSetData.cols-2])
			<< "... Training..." << endl;



		if (i == num_iter)
			cout << endl << i << " iterations completed..."
			<< "MSE: " << bp->mse(&m[(i - 1) % trainingSetData.rows][trainingSetData.cols-2]) << endl;
	}

	cout << bp->Out(0) << endl;
	cout << bp->Out(1) << endl;

	cout<< "Now using the trained network to make predctions on test data...." << endl << endl;	
	for ( int i = 0 ; i < testSet.rows ; i++ )
	{

		double *currImage = testData[i];
		//for (int p = 0; p < testSet.rows; p++) {

		//}
		bp->output_debug = true;
		bp->ffwd(testData[i]);
		cout << bp->Out(0) << endl;
		cout << bp->Out(1) << endl;
		cout << endl;

		system("pause");

	}
	system("pause");

	return 0;
}