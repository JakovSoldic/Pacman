#pragma once
#include <iostream>
#include <chrono>
using namespace std;
using namespace chrono;

class GameController
{
public:

	steady_clock::time_point bigPelletTime;
	steady_clock::time_point scatterTime;

	bool startScatter = false;
	bool endScatter = true;

	//stuff for drawing on the screen
	void drawPacman();
	void drawBlinky();
	void drawPinky();
	void drawInky();
	void drawClyde();
	void drawCube();
	void drawSmallPellet();
	void drawBigPellet();
	void drawGate();

	//keyboard control for game screens
	void movePacMan(unsigned char key, int x, int y);
	void keyboardMenu(unsigned char key, int x, int y);
	void keyboardGameOver(unsigned char key, int x, int y);

	//timers
	void frightenedDuration();
	void startScatterDuration();
	void endScatterDuration();

	//controllers for each character
	void pacmanController(float deltaTime);
	void blinkyController(float deltaTime);
	void pinkyController(float deltaTime);
	void inkyController(float deltaTime);
	void clydeController(float deltaTime);

	//game state controllers
	bool checkGameState();
	void resetGameState();

};
