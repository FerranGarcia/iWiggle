#include <SFML\Audio.hpp>

/**
 * <summary>Class RobotSound implements basic playback functionality for the iWiggle. </summary>
 */
class RobotSound {

private:
	bool bgThreadRunning;					// Background thread playback status
	sf::SoundBuffer backgroundBuffer;		
	sf::SoundBuffer soundBuffer;
	sf::Sound sound;
	sf::Sound background;
	sf::Thread backgroundThread;
	std::string bgPath;
	std::string soundPath;
	void bgThread();						// Background thread function

public:
	RobotSound() : backgroundThread(&RobotSound::bgThread, this) {
		bgThreadRunning = false;
	}

	~RobotSound();

	void playSound(std::string);
	void playBackground(std::string);

	bool isBackgroundPlaying();
	bool isSoundPlaying();

};