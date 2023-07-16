#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

#include "Red.h"
#include "globalVariables.h"
#include "pacman.h"
#include "AStar.cpp"
#include "BFS.cpp"

void Blinky::drawBlinky()
{

	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(blinkyX, blinkyY, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 0.0, 0.0); // set color to red
	glVertex3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float angle = PI * 2 * (float)i / (float)SEGMENTS;
		float x = cos(angle);
		float y = sin(angle);
		glVertex3f(x * 0.5, y * 0.5, 0.0);
	}
	glEnd();

	glColor3f(1.0, 0.0, 0.0); // set color to red
	glBegin(GL_POLYGON); // start drawing a polygon
	glVertex2f(-0.5f, -0.50f); // lower left vertex
	glVertex2f(0.5f, -0.50f); // lower right vertex
	glVertex2f(0.5f, 0.0f); // upper right vertex
	glVertex2f(-0.5f, 0.0f); // upper left vertex
	glEnd();
	glPopMatrix();
}

float Blinky::constantInterpolation(float startPoint, float endPoint, float speed, float time)
{
	float distance = endPoint - startPoint;
	float totalDuration = abs(distance) / speed;

	if (time >= totalDuration)
	{
		return endPoint;  // Reached the endpoint
	}
	else
	{
		float t = time / totalDuration;
		return startPoint + (distance * t);
	}
}

void Blinky::setPath(int targetX, int targetY)
{
	pathCoordinates.clear();
	counter = 0;
	BFS bfs;

	vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, targetX, targetY);
	reverse(path.begin(), path.end());

	for (BFS::Node* node : path) {
		pathCoordinates.emplace_back(node->x, node->y);
	}

}

void Blinky::updateBlinky(float deltaTime)
{
	int tempGridX = blinkyGridX;
	int tempGridY = blinkyGridY;
	if (counter < pathCoordinates.size()) {
		int x = pathCoordinates[counter].first;
		int y = pathCoordinates[counter].second;

		int targetGridX = x - tempGridX;
		int targetGridY = y - tempGridY;

		blinkyX = constantInterpolation(blinkyX, targetGridX, blinkySpeed, deltaTime);
		blinkyY = constantInterpolation(blinkyY, targetGridY, blinkySpeed, deltaTime);
		cout << "X: " << targetGridX << " Y:" << targetGridY << endl;
		if (abs(blinkyX - targetGridX) <= 0.01f && abs(blinkyY - targetGridY) <= 0.01f) {
			tempGridX = x;
			tempGridY = y;
			counter++;
		}
	}
}


//void Blinky::updateBlinky(float deltaTime)
//{
//	BFS bfs; // Create an instance of the BFS class
//	vector<BFS::Node*> path = bfs.bfs(blinkyGridX, blinkyGridY, player.pacmanGridX, player.pacmanGridY);
//	static size_t currentIndex = 0;  // Initialize with 0
//
//	if (!path.empty() && currentIndex < path.size())
//	{
//		size_t reversedIndex = path.size() - 1 - currentIndex;  // Reverse the index
//
//		BFS::Node* nextNode = path[reversedIndex];
//		++currentIndex;  // Increment the index to move to the next node
//
//		blinkyGridX = nextNode->x;
//		blinkyGridY = nextNode->y;
//		//blinkyGridX = blinkyNextTileX;
//		//blinkyGridY = blinkyNextTileY;
//
//		cout << "X: " << blinkyGridX << " Y:" << blinkyGridY << endl;
//		//cout << "target X: " << blinkyTargetPosX << " target y:" << blinkyTargetPosY << endl;
//		//blinkyX = constantInterpolation(blinkyX, blinkyTargetPosX, blinkySpeed, deltaTime);
//		//if (blinkyX == blinkyTargetPosX)
//		//{
//		//	blinkyGridX = blinkyNextTileX;
//		//}
//
//		//blinkyY = constantInterpolation(blinkyY, blinkyTargetPosY, blinkySpeed, deltaTime);
//		//if (blinkyY == blinkyTargetPosX)
//		//{
//		//	blinkyGridX = blinkyNextTileY;
//		//}	
//		delete nextNode;
//	}
//	//cout << "X: " << blinkyGridX << " Y:" << blinkyGridY << endl;
//	////cout << "target X: " << blinkyNextTileX << " target Y:" << blinkyNextTileY << endl;
//	////cout << "grid X: " << blinkyGridX << " grid Y:" << blinkyGridY << endl;
//	//blinkyX = constantInterpolation(blinkyX, blinkyTargetPosX, blinkySpeed, deltaTime);
//	//if (blinkyX == blinkyTargetPosX)
//	//{
//	//	blinkyGridX += blinkyTargetPosX;
//	//}
//
//	//blinkyY = constantInterpolation(blinkyY, blinkyTargetPosY, blinkySpeed, deltaTime);
//	//if (blinkyY == blinkyTargetPosY)
//	//{
//	//	blinkyGridY += blinkyTargetPosY;
//	//}
//}


//int dx = targetX - blinkyGridX;
//int dy = targetY - blinkyGridY;

//void Blinky::updateBlinky(float deltaTime)
//{
//	pathFinder.newPath(player.pacmanGridX, player.pacmanGridY, blinkyGridX, blinkyGridY);
//
//	PathFinding::sNode* nextNode = pathFinder.getNextNode();
//
//	if (nextNode != nullptr)
//	{
//		targetPosX = static_cast<float>(nextNode->x);
//		targetPosY = static_cast<float>(nextNode->y);
//	}
//
//	blinkyX = constantInterpolation(blinkyX, targetPosX, blinkySpeed, deltaTime);
//	blinkyY = constantInterpolation(blinkyY, targetPosY, blinkySpeed, deltaTime);
//
//}