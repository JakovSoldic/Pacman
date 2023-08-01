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
        int distance;
        Node* parent;

        Node(int x, int y, int distance, Node* parent = nullptr) : x(x), y(y), distance(distance), parent(parent) {}
    };

    bool isValidPosition(int x, int y)
    {
        return x >= 0 && x < mapHeight && y >= 0 && y < mapWidth;
    }

    vector<Node*> getNeighbors(Node* node, int prevX, int prevY)
    {
        vector<Node*> neighbors;

        int x = node->x;
        int y = node->y;

        int dir[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        for (int i = 0; i < 4; i++) {
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];

            if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall)
            {
                neighbors.push_back(new Node(newX, newY, node->distance + 1, node));
            }
        }

        return neighbors;
    }

    vector<Node*> bfs(int startX, int startY, int targetX, int targetY, int prevX, int prevY)
    {
        vector<Node*> path;
        queue<Node*> queue;
        vector<vector<bool>> visited(mapHeight, vector<bool>(mapWidth, false));

        Node* startNode = new Node(startX, startY, 0);
        queue.push(startNode);
        visited[startX][startY] = true;
        visited[prevX][prevY] = true;

        while (!queue.empty())
        {
            Node* currentNode = queue.front();
            queue.pop();

            if (currentNode->x == targetX && currentNode->y == targetY) {
                Node* node = currentNode;
                while (node != nullptr)
                {
                    path.push_back(node);
                    node = node->parent;
                }
                break;
            }

            vector<Node*> neighbors = getNeighbors(currentNode, prevX, prevY);
            for (Node* neighbor : neighbors)
            {
                if (!visited[neighbor->x][neighbor->y])
                {
                    visited[neighbor->x][neighbor->y] = true;
                    queue.push(neighbor);
                }
            }
        }

        //cout << "Path: ";
        //for (int i = path.size() - 1; i >= 0; i--) {
        //    cout << "(" << path[i]->x << ", " << path[i]->y << ") ";
        //}
        //cout << endl;

        while (!queue.empty())
        {
            delete queue.front();
            queue.pop();
        }

        return path;
    }

    int countValidDirections(int x, int y)
    {
        int validDirections = 0;

        int dir[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        for (int i = 0; i < 4; i++) {
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];

            if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall) 
                validDirections++;
        }

        return validDirections;
    }

    bool isCorner(int x, int y, int prevX, int prevY)
    {
        int validDirections = 0;
        int dx = x - prevX;
        int dy = y - prevY;
        int newX = 0;
        int newY = 0;

        if (dx != 0 && dy == 0)
        {
            int dir[2][2] = { {0, 1}, {0, -1} };
            for (int i = 0; i < 2; i++)
            {
                newX = x + dir[i][0];
                newY = y + dir[i][1];

                if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall)
                {
                    validDirections++;
                }
            }
        }

        if (dy != 0 && dx == 0)
        {
            int dir[2][2] = { {1, 0}, {-1, 0} };
            for (int i = 0; i < 2; i++)
            {
                newX = x + dir[i][0];
                newY = y + dir[i][1];

                if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall)
                {
                    validDirections++;
                }
            }
        }
        return validDirections == 1;
    }
};