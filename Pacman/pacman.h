#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>
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

    float targetPosX = 0;
    float targetPosY = 0;

    float pacmanX = 0, pacmanY = 0;
    int pacmanGridX = pacmanYStart, pacmanGridY = pacmanXStart;
    float playerSpeed = 6.0f;

    bool ateBigPellet = false;

    void drawMouth();
    void drawPacMan();
    void updatePacman(float deltaTime);
};