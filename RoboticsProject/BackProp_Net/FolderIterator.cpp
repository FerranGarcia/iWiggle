#include "FolderIterator.h"

FolderIterator::FolderIterator(void(*MyCallback)(cv::Mat*))
{
	this->FileLoadedEvent = MyCallback;
}

FolderIterator::~FolderIterator()
{
}

void FolderIterator::StartIterating(char *dir_name) {

	DIR *dir;
	struct dirent *ent;
	/* open directory stream */
	dir = opendir(dir_name);
	if (dir != NULL) {

		/* print all the files and directories within directory */
		while ((ent = readdir(dir)) != NULL) {

			char *fullPath = new char[strlen(dir_name) + strlen(ent->d_name) + 1];
			sprintf(fullPath, "%s/%s", dir_name, ent->d_name);

			// check is it folder or file
			struct stat fileStat;
			if (stat(fullPath, &fileStat) == -1)
			{
				printf("Unable to stat file: %s\n", fullPath);
				continue;
			}

			int n = strlen(fullPath);
			char *pos = strchr(fullPath, '.');

			if ((fileStat.st_mode & S_IFMT) == S_IFDIR)
			{
				// folder

				// if it contains dot
				if (pos != NULL) {
					// skip, it's back folder
					continue;
				}

				// recurse into it if ot's ok
				//printf("Recurse into:  %s\n", fullPath);
				//system("pause");
				StartIterating(fullPath);
				continue;
			}
			else {
				// file

				int indexOfDot = (int)(pos - fullPath);
				int lengthOfExt = n - indexOfDot;
				char *ext = new char[lengthOfExt];
				strncpy(ext, pos, lengthOfExt + 1);

				// check extension
				if (strcmp(ext, ".png") == 0) {
					//printf("Invoking for \n%s\n", fullPath);

					//cv::imread(fullpath);
					cv::Mat *fileToPassOn = new cv::Mat(cv::imread(fullPath));
					
					this->FileLoadedEvent(fileToPassOn);
				}

			}

			//printf("%s\n", ent->d_name);
		}

		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
		return;
	}

}
