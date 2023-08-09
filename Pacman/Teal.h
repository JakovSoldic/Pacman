#pragma once
#include <vector>
#include <array>
#include <cmath>

class Inky
{
public:
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
    bool hasReachedTarget = true;
    bool hasReachedHome = true;
    bool hasReachedTeleport = true;

    bool leftTeleporter = false;
    bool rightTeleporter = false;

    void drawCircle(float centerX, float centerY, float radiusX, float radiusY);
    void drawInky();
    void getPath(int targetX, int targetY);
    void getPathChase(int targetX, int targetY);
    void setPath(int pacmanTargetX, int pacmanTargetY, bool status, int pacmanDirectionX, int pacmanDirectionY, int blinkyGridX, int blinkyGridY);
    void updateInky(float deltaTime);
    void setInkySpeed();
    void checkCollision(int targetX, int targetY);
};