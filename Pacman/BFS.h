#pragma once
#ifndef BFS_H
#define BFS_H
#include <vector>
#include <queue>
using namespace std;

#include "globalVariables.h"

class BFS
{
public:
    struct Node {
        int x;
        int y;
        Node* parent;

        Node(int x, int y, Node* parent = nullptr) : x(x), y(y), parent(parent) {}
    };

    int manhattanDistance(int x1, int y1, int x2, int y2);
    bool withinMapBounds(int x, int y);
    vector<Node*> getNeighbors(Node* node);
    void freeMemory(std::queue<Node*>& queue);
    vector<Node*> getPath(Node* target);
    vector<Node*> bfs(int startX, int startY, int targetX, int targetY, int prevX, int prevY);
};
#endif
