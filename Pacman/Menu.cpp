#include <GL/freeglut.h>
#include <string>
#include <iostream>
using namespace std;

#include "globalVariables.h"
#include "Menu.h"

void Menu::drawText(const char* text, int x, int y)
{
    glRasterPos2i(x, y);
    for (const char* p = text; *p; p++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }
}

void Menu::displayOverText()
{
    text1X = 11;
    text1Y = -7;
    text2X = 9;
    text2Y = -9;
    text3X = 9;
    text3Y = -11;

    string scoreText = "SCORE: " + std::to_string(score);
    string highScoreText = "HIGHSCORE: " + std::to_string(highScore);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("GAME OVER", text1X, text1Y);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(highScoreText.c_str(), text2X, text2Y);
    drawText(scoreText.c_str(), text2X, text2Y - 2);
    drawText("EXIT GAME (1)", text3X, text3Y - 2);
}

void Menu::displayStartText()
{
    text1X = 11;
    text1Y = -7;
    text2X = 9;
    text2Y = -9;
    text3X = 9;
    text3Y = -11;


    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("PACMAN", text1X, text1Y);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("START GAME (1)", text2X, text2Y);
    drawText("EXIT GAME (2)", text3X, text3Y);
}

void Menu::displayScore()
{
    text1X = 0;
    text1Y = 1;
    text2X = 10;
    text2Y = 1;

    string scoreText = "Score: " + std::to_string(score);
    string highScoreText = "Highscore: " + std::to_string(highScore);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(scoreText.c_str(), text1X, text1Y);
    drawText(highScoreText.c_str(), text2X, text2Y);
}