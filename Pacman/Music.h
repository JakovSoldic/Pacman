#pragma once
#ifndef MUSIC_H
#define MUSIC_H
#include <iostream>

class Music
{
private:
	bool introPlayed = false;
	
public:
	bool movementSoundPlayed = false;

	void playIntro();
	void playMovementSound();
	void stopMovementSound();
	void playAteGhost();
	void playDeath();
	void stopDeath();
};
#endif