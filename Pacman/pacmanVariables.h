#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <cstdint>

enum Tiles{
    empty_space,//number 0 in the maze
    small_pellet,//number 1 in the maze
    big_pellet,//number 2 in the maze
    gate,//number 3 in the maze
    wall//number 4 in the maze
};

const int mapWidth = 28;
const int mapHeight = 31;

float pacmanX = 0, pacmanY = 0;//pacmans drawn pixel coordinates/the center of pacman when using translatef(pacmanX, pacmanY, 0)
float pacmanVX = 0, pacmanVY = 0;//pacmans velocity in a given direction
int mouthDirection = 0;//pacmans mouth direction represented by a number, 0 = right, 1 = left, 2 = up, 3 = down
int pacmanGridX = 23, pacmanGridY = 13;//pacmans maze location represented by the number 6 in the maze. The location of the 6 in the maze is [23][13]
int previousTime = 0;//used for deltaTime. Check display() function in main.cpp
float playerSpeed = 2.5; // tiles per second