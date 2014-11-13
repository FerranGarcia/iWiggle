#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat img;
	Mat imageMatrix;

	FILE* f = 0;
	char _filename[1024];

	if (argc == 1)
	{
		printf("Usage: BackProp (<image_filename> | <image_list>.txt)\n");
		return 0;
	}
	img = imread(argv[1]);

	if (img.data)
	{
		strcpy(_filename, argv[1]);
	}
	else
	{
		f = fopen(argv[1], "rt");
		if (!f)
		{
			fprintf(stderr, "ERROR: the specified file could not be loaded\n");
			return -1;
		}
	}

	namedWindow("training process", 1);

	for (;;)
	{
		char* filename = _filename;
		if (f)
		{
			if (!fgets(filename, (int)sizeof(_filename) - 2, f))
				break;

			if (filename[0] == '#')
				continue;
			int l = (int)strlen(filename);
			while (l > 0 && isspace(filename[l - 1]))
				--l;
			filename[l] = '\0';
			img = imread(filename);
		}
		printf("%s:\n", filename);
		if (!img.data)
			continue;

		//Resize before adding the row to the training matrix
		imageMatrix.push_back(img.reshape(0, 1));
		cout << img.rows << " " << img.cols << endl;

		imshow("training process", img);
		int c = waitKey(0) & 255;
		if (c == 'q' || c == 'Q' || !f)
			break;
	}

	cout << imageMatrix.rows << " " << imageMatrix.cols << endl;
	if (f)
		fclose(f);
	return 0;
}
