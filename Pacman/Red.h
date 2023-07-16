#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

class Blinky
{
public:

    struct Point {
        int x;
        int y;
    };

    Point turnTo = { 0, 0 };

    const int blinkyXStart = 13;
    const int blinkyYStart = 11;

    float blinkyTargetPosX;
    float blinkyTargetPosY;

    float blinkyX = 0, blinkyY = 0;//pacmans drawn pixel coordinates/the center of pacman when using translatef(pacmanX, pacmanY, 0)
    int blinkyGridX = blinkyYStart, blinkyGridY = blinkyXStart;//pacmans grid location
    float blinkySpeed = 4.0f; // tiles per second

    vector<std::pair<int, int>> pathCoordinates;
    int counter = 0;
    //void drawMouth();
    void drawBlinky();
    float constantInterpolation(float startPoint, float endPoint, float speed, float time);
    void setPath(int targetX, int targetY);
    void updateBlinky(float deltaTime);
};