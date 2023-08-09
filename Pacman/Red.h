#pragma once
#include <vector>
#include <array>
#include <cmath>

class Blinky
{
public:
    const int blinkyXStart = 13;
    const int blinkyYStart = 11;

    const int blinkyXHome = 14;
    const int blinkyYHome = 12;

    float blinkyX = 0, blinkyY = 0;
    int blinkyGridX = blinkyYStart, blinkyGridY = blinkyXStart;
    float blinkySpeed = 5.0f;

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

    int blinkyCornerX = 1;
    int blinkyCornerY = 1;

    int x = 0;
    int y = 0;

    bool isDead = false;
    bool isFrightened = false;
    bool hasReachedTarget = true;
    bool hasReachedHome = true;
    bool hasReachedTeleport = true;

    bool leftTeleporter = false;
    bool rightTeleporter = false;

    void drawEllipse(float centerX, float centerY, float radiusX, float radiusY);
    void drawBlinky();
    void getPath(int targetX, int targetY);
    void getPathChase(int targetX, int targetY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status);
    void updateBlinky(float deltaTime);
    void setBlinkySpeed();
    void checkCollision(int targetX, int targetY);
};