#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

#include "Red.h"

class Pacman
{
public:

    struct Point {
        int x;
        int y;
    };

    Point turnTo = { 0, 0 };

    const int pacmanXStart = 13;
    const int pacmanYStart = 23;

    float targetPosX;
    float targetPosY;

    float pacmanX = 0, pacmanY = 0;//pacmans drawn pixel coordinates/the center of pacman when using translatef(pacmanX, pacmanY, 0)
    int pacmanGridX = pacmanYStart, pacmanGridY = pacmanXStart;//pacmans grid location
    float playerSpeed = 4.0f; // tiles per second

    void drawMouth();
    void drawPacMan();
    float constantInterpolation(float startPoint, float endPoint, float speed, float time);
    void updatePacman(float deltaTime);
};