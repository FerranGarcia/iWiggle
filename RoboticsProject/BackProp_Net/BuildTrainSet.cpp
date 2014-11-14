#include "BuildTrainSet.h"

BuildTrainSet::BuildTrainSet(int argc, char* argv)
{
	this->argc = argc;
	this->argv = argv;
}

Mat BuildTrainSet::ComposeMatrix(){
	Mat img;
	Mat imageMatrix;

	FILE* f = 0;
	char _filename[1024];

	if (argc == 1)
	{
		printf("Usage: BackProp <train_list_filename>.txt & <test_list_filename>.txt\n");
	}
	img = imread(argv);

	if (img.data)
	{
		strcpy(_filename, argv);
	}
	else
	{
		f = fopen(argv, "rt");
		if (!f)
		{
			fprintf(stderr, "ERROR: the specified file could not be loaded\n");
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
			img = imread(filename,0);
		}
		printf("%s:\n", filename);
		if (!img.data)
			continue;

		//Resize before adding the row to the training matrix
		imageMatrix.push_back(img.reshape(0, 1));
		cout << img.rows << " " << img.cols << endl;

		imshow("training process", img);
		int c = waitKey(1) & 255;
		if (c == 'q' || c == 'Q' || !f)
			break;
	}

	cout << imageMatrix.rows << " " << imageMatrix.cols << endl;
	if (f)
		fclose(f);

	imageMatrix.convertTo(imageMatrix, CV_32F);

	return imageMatrix;
}

BuildTrainSet::~BuildTrainSet()
{
}
