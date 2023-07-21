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

    float targetPosX;
    float targetPosY;

    float pacmanX = 0, pacmanY = 0;
    int pacmanGridX = pacmanYStart, pacmanGridY = pacmanXStart;
    float playerSpeed = 4.0f;

    float timer = 0;

    bool ateBigPellet = false;

    float portalX = 0;

    void drawMouth();
    void drawPacMan();
    float constantInterpolation(float startPoint, float endPoint, float speed, float time);
    void updatePacman(float deltaTime);
};