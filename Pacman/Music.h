#pragma once
#include <iostream>

class Music
{
public:
	bool introPlayed = false;
	bool movementSoundPlayed = false;
	
	void playIntro();
	void playMovementSound();
	void stopMovementSound();
	void playAteGhost();
	void playDeath();
};
