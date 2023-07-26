#pragma once
#include <vector>
#include <array>
#include <cmath>

class Clyde
{
public:
    const int clydeXStart = 15;
    const int clydeYStart = 14;
    const int radius = 8;

    float clydeX = 0, clydeY = 0;
    int clydeGridX = clydeYStart, clydeGridY = clydeXStart;
    float clydeSpeed = 4.0f;

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
    bool hasReachedTarget = true;
    bool hasReachedHome = true;
    bool hasReachedCorner = true;

    void drawEllipse(float centerX, float centerY, float radiusX, float radiusY);
    void drawClyde();
    bool isWithinRadius(int pacmanGridX, int pacmanGridY, int clydeGridX, int clydeGridY, int radius);
    void getPathFrightened(int targetX, int targetY);
    void getPathDead(int targetX, int targetY);
    void getPathChase(int targetX, int targetY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status);
    void updateClyde(float deltaTime);
    void setClydeSpeed();
    void checkCollision(int targetX, int targetY);
};