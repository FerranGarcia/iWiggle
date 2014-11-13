#pragma once

#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>

using namespace cv;
using namespace std;

class BuildTrainSet
{
public:

	int argc;
	char* argv;

	BuildTrainSet(int argc, char* argv);
	Mat BuildTrainSet::ComposeMatrix();
	~BuildTrainSet();
};

