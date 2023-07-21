#pragma once
#include <vector>
#include <array>
#include <cmath>

class Blinky
{
public:
    const int blinkyXStart = 13;
    const int blinkyYStart = 11;

    float blinkyX = 0, blinkyY = 0;
    int blinkyGridX = blinkyYStart, blinkyGridY = blinkyXStart;
    float blinkySpeed = 4.0f;

    vector<std::pair<int, int>> pathCoordinates;
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

    int x = 0;
    int y = 0;

    bool isDead = false;
    bool isFrightened = false;
    bool hasReachedTarget = true;
    bool hasReachedHome = true;

    void drawBlinky();
    float constantInterpolation(float startPoint, float endPoint, float speed, float time);
    void getPathFrightened(int targetX, int targetY);
    void getPathDead(int targetX, int targetY);
    void getPathChase(int targetX, int targetY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status);
    void updateBlinky(float deltaTime);
    void eatenBigPellet();
    void setBlinkySpeed();
    void checkCollision(int targetX, int targetY);
};