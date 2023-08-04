#pragma once
#include <iostream>

class GameController
{
public:
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

	//controllers for each character
	void pacmanController(float deltaTime);
	void blinkyController(float deltaTime);
	void pinkyController(float deltaTime);
	void inkyController(float deltaTime);
	void clydeController(float deltaTime);
};
