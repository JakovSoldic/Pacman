#pragma once
#ifndef ORANGE_H
#define ORANGE_H
#include <vector>
#include <array>
#include <cmath>

#include "globalVariables.h"
#include "BFS.h"
#include "Music.h"

class Clyde
{
private:
    BFS bfsClyde;
    Music musicPlayer;

    const int clydeXStart = 15;
    const int clydeYStart = 14;
    const int radius = 8;

    float clydeX = 0, clydeY = 0;
    int clydeGridX = clydeYStart, clydeGridY = clydeXStart;
    float clydeSpeed = 3.5f;

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

    int clydeCornerX = 29;
    int clydeCornerY = 1;

    int x = 0;
    int y = 0;

    bool isDead = false;
    bool isFrightened = false;
    bool hasReachedRandomTile = true;
    bool hasReachedTeleport = true;

    bool leftTeleporter = false;
    bool rightTeleporter = false;

    int eyesXPos = 0;
    int eyesYPos = 0;

public:
    int getClydeXStart() { return clydeXStart; }
    int getClydeYStart() { return clydeYStart; }
    int getClydeGridX() { return clydeGridX; }
    int getClydeGridY() { return clydeGridY; }

    void drawCircle(float centerX, float centerY, float radiusX, float radiusY);
    void drawEyes();
    void drawClyde();
    bool isWithinRadius(int pacmanGridX, int pacmanGridY, int clydeGridX, int clydeGridY, int radius);
    void getPath(int targetX, int targetY, int previousTileX, int previousTileY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status);
    void updateClyde(float deltaTime);
    void setClydeSpeed();
    void checkCollision(int targetX, int targetY);
    void resetClydeStats();
};
#endif