#include <iostream>
using namespace std;

#include "BFS.h"

int BFS::manhattanDistance(int x1, int y1, int x2, int y2)
{
    return abs(x1 - x2) + abs(y1 - y2);
}

bool BFS::withinMapBounds(int x, int y)
{
    return x >= 0 && x < mapHeight && y >= 0 && y < mapWidth;
}

vector<BFS::Node*> BFS::getNeighbors(Node* node)
{
    vector<Node*> neighbors;

    int x = node->x;
    int y = node->y;

    int dir[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

    for (int i = 0; i < 4; i++)
    {
        int newX = x + dir[i][0];
        int newY = y + dir[i][1];

        if (withinMapBounds(newX, newY) && maze[newX][newY] != Tiles::wall)
            neighbors.push_back(new Node(newX, newY, node));
    }

    return neighbors;
}

void BFS::freeMemory(queue<Node*>& queue)
{
    while (!queue.empty())
    {
        delete queue.front();
        queue.pop();
    }
}

vector<BFS::Node*> BFS::getPath(Node* target)
{
    vector<Node*> path;

    while (target != nullptr)
    {
        path.push_back(target);
        target = target->parent;
    }

    reverse(path.begin(), path.end());

    return path;
}

vector<BFS::Node*> BFS::bfs(int startX, int startY, int targetX, int targetY, int prevX, int prevY)
{
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

        int currentTile = manhattanDistance(current->x, current->y, targetX, targetY);
        int closestTile = manhattanDistance(closest->x, closest->y, targetX, targetY);

        if (currentTile < closestTile)
            closest = current;

        if (current->x == targetX && current->y == targetY)
        {
            freeMemory(queue);
            return getPath(current);
        }

        vector<Node*> neighbours = getNeighbors(current);
        for (Node* i : neighbours)
        {
            if (!visited[i->x][i->y])
            {
                visited[i->x][i->y] = true;
                queue.push(i);
            }
        }
    }
    freeMemory(queue);
    return getPath(closest);
}
