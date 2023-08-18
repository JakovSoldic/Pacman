#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>
class Pacman
{
private:
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
    float playerSpeed = 5.0f;

    bool ateBigPellet = false;

public:
    int getPacmanXStart() { return pacmanXStart; }
    int getPacmanYStart() { return pacmanYStart; }
    int getPacmanGridX() { return pacmanGridX; }
    int getPacmanGridY() { return pacmanGridY; }
    int getPacmanTurnToX() { return turnTo.x; }
    int getPacmanTurnToY() { return turnTo.y; }
    bool getAteBigPellet() { return ateBigPellet; }

    void setAteBigPellet(bool status) { ateBigPellet = status; }
    void setTurnTo(int x, int y) { turnTo = { x, y }; }

    void drawMouth();
    void drawPacMan();
    void updatePacman(float deltaTime);
    void resetPacmanStats();
};