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

        Node(int x, int y, int distance, Node* parent = nullptr): x(x), y(y), distance(distance), parent(parent) {}
    };

    bool isValidPosition(int x, int y) 
    {
        return x >= 0 && x < mapWidth && y >= 0 && y < mapHeight;
    }

    vector<Node*> getNeighbors(Node* node) 
    {
        vector<Node*> neighbors;

        int x = node->x;
        int y = node->y;

        // Check all four directions (up, down, left, right)
        int dir[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

        for (int i = 0; i < 4; i++) {
            int newX = x + dir[i][0];
            int newY = y + dir[i][1];

            if (isValidPosition(newX, newY) && maze[newX][newY] != Tiles::wall) {
                neighbors.push_back(new Node(newX, newY, node->distance + 1, node));
            }
        }

        return neighbors;
    }

    vector<Node*> bfs(int startX, int startY, int targetX, int targetY) 
    {
        vector<Node*> path;
        queue<Node*> queue;
        vector<vector<bool>> visited(mapHeight, vector<bool>(mapWidth, false));

        // Create the start node
        Node* startNode = new Node(startX, startY, 0);
        queue.push(startNode);
        visited[startX][startY] = true;

        while (!queue.empty()) 
        {
            Node* currentNode = queue.front();
            queue.pop();

            // Check if the target is reached
            if (currentNode->x == targetX && currentNode->y == targetY) {
                // Trace back the path from the target node to the start node
                Node* node = currentNode;
                while (node != nullptr) 
                {
                    path.push_back(node);
                    node = node->parent;
                }
                break;
            }

            // Get the neighbors of the current node
            vector<Node*> neighbors = getNeighbors(currentNode);
            for (Node* neighbor : neighbors) 
            {
                if (!visited[neighbor->x][neighbor->y]) 
                {
                    visited[neighbor->x][neighbor->y] = true;
                    queue.push(neighbor);
                }
            }
        }
        // Print the path
        //cout << "Path: ";
        //for (int i = path.size() - 1; i >= 0; i--) {
        //    cout << "(" << path[i]->x << ", " << path[i]->y << ") ";
        //}
        //cout << endl;
        //Clean up memory
        while (!queue.empty()) 
        {
            delete queue.front();
            queue.pop();
        }

        return path;
    }
};