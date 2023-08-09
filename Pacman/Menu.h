#pragma once
#include <GL/freeglut.h>
#include <string>
#include <iostream>
using namespace std;

class Menu
{
public:
    void drawText(const char* text, int x, int y);
    void displayWonText();
    void displayOverText();
    void displayStartText();
    void displayScore();
};
