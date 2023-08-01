#pragma once
extern const int mapWidth;
extern const int mapHeight;

extern int maze[][28];

extern int previousTime;

extern int score;
extern int highScore;

extern bool isScatter;
extern int scatterCounter;

extern int ghostHomeX;
extern int ghostHomeY;

extern bool isIntroDone;

enum Tiles {
    empty_space,//number 0 in the maze
    small_pellet,//number 1 in the maze
    big_pellet,//number 2 in the maze
    gate,//number 3 in the maze
    wall,//number 4 in the maze
    teleport_tile,//number 5 in the maze
};

enum GameState {
    START_MENU,
    GAME,
    GAME_OVER_MENU
};

extern GameState currentState;

float constantInterpolation(float startPoint, float endPoint, float speed, float time);
void updateHighScore();