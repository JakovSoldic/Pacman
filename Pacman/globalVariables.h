#pragma once
extern const int mapWidth;
extern const int mapHeight;

extern int maze[][28];

extern int previousTime;

enum Tiles {
    empty_space,//number 0 in the maze
    small_pellet,//number 1 in the maze
    big_pellet,//number 2 in the maze
    gate,//number 3 in the maze
    wall,//number 4 in the maze
    corner_tile,
};
