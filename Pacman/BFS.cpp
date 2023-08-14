#include <iostream>
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

    int manhattanDistance(int x1, int y1, int x2, int y2)
    {
        return abs(x1 - x2) + abs(y1 - y2);
    }

    bool isValidPosition(int x, int y)
    {
        return x >= 0 && x < mapHeight && y >= 0 && y < mapWidth;
    }

    vector<Node*> getNeighbors(Node* node)
    {
        vector<Node*> neighbors;

        int x = node->x;
        int y = node->y;

        int dir[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        for (int i = 0; i < 4; i++) 
        {
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];

            if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall)
                neighbors.push_back(new Node(newX, newY, node));
        }

        return neighbors;
    }

    void freeMemory(queue<Node*>& queue)
    {
        while (!queue.empty())
        {
            delete queue.front();
            queue.pop();
        }
    }

    vector<Node*> getPath(Node* target)
    {
        vector<Node*> path;

        while (target != nullptr)
        {
            path.push_back(target);
            target = target->parent;
        }

        return path;
    }

    vector<Node*> bfs(int startX, int startY, int targetX, int targetY, int prevX, int prevY)
    {
        vector<Node*> path;
        queue<Node*> queue;
        vector<vector<bool>> visited(mapHeight, vector<bool>(mapWidth, false));

        Node* start = new Node(startX, startY, nullptr);
        Node* closest = new Node(startX, startY, nullptr);

        queue.push(start);

        visited[startX][startY] = true;
        visited[prevX][prevY] = true;

        while (!queue.empty())
        {
            Node* current = queue.front();
            queue.pop();

            int closestCurr = manhattanDistance(current->x, current->y, targetX, targetY);
            int closestClos = manhattanDistance(closest->x, closest->y, targetX, targetY);

            if (closestCurr < closestClos)
                closest = current;

            if (current->x == targetX && current->y == targetY)
            {
                freeMemory(queue);
                return getPath(current);
            }

            vector<Node*> neighbors = getNeighbors(current);
            for (Node* neighbor : neighbors)
            {
                if (!visited[neighbor->x][neighbor->y])
                {
                    visited[neighbor->x][neighbor->y] = true;
                    queue.push(neighbor);
                }
            }
        }
        freeMemory(queue);
        return getPath(closest);
    }

   
};