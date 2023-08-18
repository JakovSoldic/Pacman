#pragma once
#ifndef PINK_H
#define PINK_H
#include <vector>
#include <array>
#include <cmath>

#include "globalVariables.h"
#include "BFS.h"
#include "Music.h"

class Pinky
{
private:
    BFS bfsPinky;
    Music musicPlayer;

    const int pinkyXStart = 13;
    const int pinkyYStart = 14;

    float pinkyX = 0, pinkyY = 0;
    int pinkyGridX = pinkyYStart, pinkyGridY = pinkyXStart;
    float pinkySpeed = 4.5f;

    int prevGridX = 0;
    int prevGridY = 0;

    vector<pair<int, int>> pathCoordinates;
    int counter = 0;
    bool animationComplete = true;

    int targetGridX = 0;
    int targetGridY = 0;

    float targetPosX = 0;
    float targetPosY = 0;

    int previousTargetX = 0;
    int previousTargetY = 0;

    int randomGridX = 0;
    int randomGridY = 0;

    int pinkyCornerX = 1;
    int pinkyCornerY = 26;

    int x = 0;
    int y = 0;

    bool isDead = false;
    bool isFrightened = false;
    bool hasReachedTarget = true;
    bool hasReachedHome = true;
    bool hasReachedTeleport = true;

    bool leftTeleporter = false;
    bool rightTeleporter = false;

public:
    int getPinkyXStart() { return pinkyXStart; }
    int getPinkyYStart() { return pinkyYStart; }
    int getPinkyGridX() { return pinkyGridX; }
    int getPinkyGridY() { return pinkyGridY; }

    void drawCircle(float centerX, float centerY, float radiusX, float radiusY);
    void drawPinky();
    void getPath(int targetX, int targetY);
    void getPathChase(int targetX, int targetY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status, int pacmanDirectionX, int pacmanDirectionY);
    void updatePinky(float deltaTime);
    void setPinkySpeed();
    void checkCollision(int targetX, int targetY);
    void resetPinkyStats();
};
#endif