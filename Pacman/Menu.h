#pragma once
#include <GL/freeglut.h>
#include <string>
#include <iostream>
using namespace std;

class Menu
{
public:

    float text1X, text1Y, text2X, text2Y, text3X, text3Y;

    void drawText(const char* text, int x, int y);
    void displayWonText();
    void displayOverText();
    void displayStartText();
    void displayScore();
};
