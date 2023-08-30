#pragma once
#ifndef TEAL_H
#define TEAL_H
#include <vector>
#include <array>
#include <cmath>

#include "globalVariables.h"
#include "BFS.h"
#include "Music.h"

class Inky
{
private:
    BFS bfsInky;
    Music musicPlayer;

    const int inkyXStart = 14;
    const int inkyYStart = 14;

    float inkyX = 0, inkyY = 0;
    int inkyGridX = inkyYStart, inkyGridY = inkyXStart;
    float inkySpeed = 4.0f;

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

    int inkyCornerX = 29;
    int inkyCornerY = 26;

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
    int getInkyXStart() { return inkyXStart; }
    int getInkyYStart() { return inkyYStart; }
    int getInkyGridX() { return inkyGridX; }
    int getInkyGridY() { return inkyGridY; }

    void drawCircle(float centerX, float centerY, float radiusX, float radiusY);
    void drawEyes();
    void drawInky();
    void getPath(int targetX, int targetY, int previousTileX, int previousTileY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status, int pacmanDirectionX, int pacmanDirectionY, int blinkyGridX, int blinkyGridY);
    void updateInky(float deltaTime);
    void setInkySpeed();
    void checkCollision(int targetX, int targetY);
    void resetInkyStats();
};
#endif