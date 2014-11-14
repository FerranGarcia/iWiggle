#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

class FSInterface
{
public:
	FSInterface();
	~FSInterface();

	std::map<int, string> StateMessages;
	std::map<int, string> NotificationSoundFiles;
	vector<string> BackgroundMusicFiles;
};

