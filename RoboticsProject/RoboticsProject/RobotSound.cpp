#include <iostream>
#include "RobotSound.h"

using namespace std;

/**
 * <summary> Default destructor. </summary>
 */
RobotSound::~RobotSound(){
	// Terminate the background if playing
	if (background.getStatus() == sf::Sound::Playing)
		backgroundThread.terminate();
}

/** 
 * <summary> Accessor of the backgroundPlaying. </summary>
 */
bool RobotSound::isBackgroundPlaying() {
	return background.getStatus() == sf::Sound::Playing ? true : false;
}

/**
* <summary> Accessor of the soundPlaying. </summary>
*/
bool RobotSound::isSoundPlaying() {
	return sound.getStatus() == sf::Sound::Playing ? true : false;;
}

/**
 * <summary> Plays sound with the specified id. </summary>
 */
void RobotSound::playSound(string path) {

	if (!soundBuffer.loadFromFile(path))
		std::cout << "Failed to load sound" << std::endl;

	// It is supposed that the program will continue to run after the play() call.
	// So, we do not wait until the sound is played (The sound is played in the separate thread by default)
	sound.setBuffer(soundBuffer);
	sound.play();
}

/**
 * <summary> Launches the background playback thread. </summary>
 */
void RobotSound::playBackground(string path) {

	bgPath = path;

	if (!bgThreadRunning) {

		if (!backgroundBuffer.loadFromFile(bgPath)) {
			std::cout << "Failed to play background!" << std::endl;
			return;
		}

		background.setBuffer(backgroundBuffer);
		background.setLoop(true);
		background.play();

		backgroundThread.launch();
		bgThreadRunning = true;
	}

}

/**
 * <summary> Background playback thread function. </summary>
 */
void RobotSound::bgThread() {

	// TODO:
	// Read the backgroundNumber here and get the file to play
	string lastBgPath = bgPath;

	while (true) {
		// If sound is playing and background is playing, pause the background
		if (sound.getStatus() == sf::Sound::Playing && background.getStatus() == sf::Sound::Playing)
			background.pause();
		// If background is paused and no sound is playing, restore the background playback
		else if (sound.getStatus() != sf::Sound::Playing && background.getStatus() == sf::Sound::Paused)
			background.play();

		// Check if the background is switched
		/**
		if (!strcmp(bgPath.c_str(),lastBgPath.c_str())) {
			background.stop();
			if (backgroundBuffer.loadFromFile("sound.wav"))
				std::cout << "Failed to play background!" << std::endl;
			background.setBuffer(backgroundBuffer);
		}
		*/
		
	}
}