#pragma once
#include <string>
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
/* "readdir" etc. are defined here. */
#include "dirent.h"
/* limits.h defines "PATH_MAX". */
#include <limits.h>

using namespace std;
class FolderIterator
{
	void(*FileLoadedEvent)(cv::Mat*);
	char *dir_name;

public:
	FolderIterator(void(*FileLoadedEvent)(cv::Mat*));
	~FolderIterator();

	void StartIterating(char *dir_name);
};

