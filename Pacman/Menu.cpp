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

void Menu::displayWonText()
{
    string scoreText = "SCORE: " + std::to_string(score);
    string highScoreText = "HIGHSCORE: " + std::to_string(highScore);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("YOU WON", 11, -7);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(highScoreText.c_str(), 10, -9);
    drawText(scoreText.c_str(), 10, -11);
    drawText("EXIT GAME (2)", 10,  -13);
}

void Menu::displayOverText()
{
    string scoreText = "SCORE: " + std::to_string(score);
    string highScoreText = "HIGHSCORE: " + std::to_string(highScore);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("GAME OVER", 11, -7);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(highScoreText.c_str(), 10, -9);
    drawText(scoreText.c_str(), 10, -11);
    if (lives > 0)
        drawText("RETRY (1)", 10, -13);
    drawText("EXIT GAME (2)", 10, -15);
}

void Menu::displayStartText()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 0.0f);
    drawText("PACMAN", 11, -7);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText("START GAME (1)", 10, -9);
    drawText("EXIT GAME (2)", 10, -11);
}

void Menu::displayScore()
{
    string scoreText = "Score: " + std::to_string(score);
    string highScoreText = "Highscore: " + std::to_string(highScore);
    string livesText = "Lives: " + std::to_string(lives);

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(scoreText.c_str(), 0, 1);
    drawText(highScoreText.c_str(), 10, 1);
    drawText(livesText.c_str(), 25, 1);
}