#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <array>
using namespace std;

#include <GL/GL.h>
#include <GL/freeglut.h>

float pacmanX = 0, pacmanY = 0;
float pacmanVX = 0, pacmanVY = 0;
int mouthDirection = 0, eyeDirection = 0;
int pacmanGridX = 23, pacmanGridY = 13;
float cellWidth = 600.0f / 28, cellHeight = 200.0f / 31;
int mapHeight = 31, mapWidth = 28;
//MAP WIDTH = 28, MAP HEIGHT = 31, TILE SIZE = 20, PACMAN_X_START = 23, PACMAN_Y_START = 13
//0 = empty space, 4 = wall, 3 = gate, 2 = big pellets, 1 = small pellets
int maze[31][28] = {
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
		{4,1,1,1,1,1,1,1,1,1,1,1,1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,4},
		{4,1,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,1,4},
		{4,2,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,2,4},
		{4,1,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,1,4},
		{4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4},
		{4,1,4,4,4,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,4,4,4,1,4},
		{4,1,4,4,4,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,4,4,4,1,4},
		{4,1,1,1,1,1,1,4,4,1,1,1,1,4,4,1,1,1,1,4,4,1,1,1,1,1,1,4},
		{4,4,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,4,4},
		{0,0,0,0,0,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,0,0,0,0,0},
		{0,0,0,0,0,4,1,4,4,1,1,1,1,1,1,1,1,1,1,4,4,1,4,0,0,0,0,0},
		{0,0,0,0,0,4,1,4,4,1,4,4,4,3,3,4,4,4,1,4,4,1,4,0,0,0,0,0},
		{4,4,4,4,4,4,1,4,4,1,4,0,0,0,0,0,0,4,1,4,4,1,4,4,4,4,4,4},
		{1,1,1,1,1,1,1,1,1,1,4,5,0,0,0,0,0,4,1,1,1,1,1,1,1,1,1,1},
		{4,4,4,4,4,4,1,4,4,1,4,0,0,0,0,0,0,4,1,4,4,1,4,4,4,4,4,4},
		{0,0,0,0,0,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,0,0,0,0,0},
		{0,0,0,0,0,4,1,4,4,1,1,1,1,1,1,1,1,1,1,4,4,1,4,0,0,0,0,0},
		{0,0,0,0,0,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,0,0,0,0,0},
		{4,4,4,4,4,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,4,4,4,4,4},
		{4,1,1,1,1,1,1,1,1,1,1,1,1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,4},
		{4,1,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,1,4},
		{4,1,4,4,4,4,1,4,4,4,4,4,1,4,4,1,4,4,4,4,4,1,4,4,4,4,1,4},
		{4,2,1,1,4,4,1,1,1,1,1,1,1,6,1,1,1,1,1,1,1,1,4,4,1,1,2,4},
		{4,4,4,1,4,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,4,1,4,4,4},
		{4,4,4,1,4,4,1,4,4,1,4,4,4,4,4,4,4,4,1,4,4,1,4,4,1,4,4,4},
		{4,1,1,1,1,1,1,4,4,1,1,1,1,4,4,1,1,1,1,4,4,1,1,1,1,1,1,4},
		{4,1,4,4,4,4,4,4,4,4,4,4,1,4,4,1,4,4,4,4,4,4,4,4,4,4,1,4},
		{4,1,4,4,4,4,4,4,4,4,4,4,1,4,4,1,4,4,4,4,4,4,4,4,4,4,1,4},
		{4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4},
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4},
};
bool grid[31][28];
pair<float, float> pairNumbers;
vector<pair<float, float>> walls;

float playerSpeed = 2.5; // tiles per second
int previousTime = 0;

//void redGhost() {
//
//	const int SEGMENTS = 32;
//	const float PI = 3.14159265359;
//
//	glPushMatrix();
//	glTranslatef(redGhostX, redGhostY, 0);
//	glBegin(GL_TRIANGLE_FAN);
//	glColor3f(1.0, 0.0, 0.0); // set color to red
//	glVertex3f(0.0, 0.0, 0.0);
//	for (int i = 0; i <= SEGMENTS; i++)
//	{
//		float angle = PI * 2 * (float)i / (float)SEGMENTS;
//		float x = cos(angle);
//		float y = sin(angle);
//		glVertex3f(x * 0.5, y * 0.5, 0.0);
//	}
//	glEnd();
//
//	glColor3f(1.0, 0.0, 0.0); // set color to red
//	glBegin(GL_POLYGON); // start drawing a polygon
//	glVertex2f(-0.5f, -0.50f); // lower left vertex
//	glVertex2f(0.5f, -0.50f); // lower right vertex
//	glVertex2f(0.5f, 0.0f); // upper right vertex
//	glVertex2f(-0.5f, 0.0f); // upper left vertex
//	glEnd();
//	glPopMatrix();
//}

//void moveRedGhost()
//{
//	cout << redGhostGridX << " " << redGhostGridY;
//	while (pacmanGridX != redGhostGridX && pacmanGridY != redGhostGridY)
//	{
//		if (pacmanGridY > redGhostGridY)//left
//		{
//			redGhostX -= 1;
//			redGhostGridY -= 1;
//		}
//		else//right
//		{
//			redGhostX += 1;
//			redGhostGridY += 1;
//		}
//
//		if (pacmanGridX > redGhostGridX)//up
//		{
//			redGhostY += 1;
//			redGhostGridX -= 1;
//		}
//		else//down
//		{
//			redGhostY -= 1;
//			redGhostGridX += 1;
//		}
//	}
//	//glutPostRedisplay();
//}

void drawMouth()
{
	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	if (mouthDirection == 0) {
		glRotatef(0, 0, 0, 1);
	}
	if (mouthDirection == 1) {
		glRotatef(180, 0, 0, 1);
	}
	if (mouthDirection == 2) {
		glRotatef(90, 0, 0, 1);
	}
	if (mouthDirection == 3) {
		glRotatef(-90, 0, 0, 1);
	}

	glBegin(GL_TRIANGLES);
	glColor3f(0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0.499, -0.37, 0);
	glVertex3f(0.499, 0.37, 0);
	glEnd();
	glPopMatrix();
}

void drawPacMan() 
{
	const int SEGMENTS = 32;
	const float PI = 3.14159265359;

	glPushMatrix();
	glTranslatef(pacmanX, pacmanY, 0);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 0.0); // yellow color
	glVertex3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= SEGMENTS; i++)
	{
		float angle = PI * 2 * (float)i / (float)SEGMENTS;
		float x = cos(angle);
		float y = sin(angle);
		glVertex3f(x * 0.5, y * 0.5, 0.0);
	}
	glEnd();
	glPopMatrix();
}

void updatePacman(float deltaTime)
{
	pacmanGridY = 13 + pacmanX;
	pacmanGridX = 23 + pacmanY;

	floor(pacmanGridX);
	floor(pacmanGridY);
	if (mouthDirection == 0)
	{
		//pacmanGridY = floor(13 + pacmanX);
		if (!grid[pacmanGridX][pacmanGridY + 1])
		{
			pacmanX += pacmanVX * deltaTime;
			//pacmanGridX = 23 - floor(pacmanY);
		}
	}
	if (mouthDirection == 1)
	{
		//pacmanGridY = ceil(13 + pacmanX);
		if (!grid[pacmanGridX][pacmanGridY])
		{
			pacmanX += pacmanVX * deltaTime;
			//pacmanGridY = 13 + ceil(pacmanX);
		}
	}
	if (mouthDirection == 2)
	{
		if (pacmanGridX > 0 && !grid[pacmanGridX - 1][pacmanGridY])
		{
			pacmanY += pacmanVY * deltaTime;
			//pacmanGridX = 23 - floor(pacmanY);
		}
	}
	if (mouthDirection == 3)
	{
		if (pacmanGridX < 31 - 1 && !grid[pacmanGridX + 1][pacmanGridY])
		{
			pacmanY += pacmanVY * deltaTime;
			//pacmanGridX = 23 - floor(pacmanY);
		}
	}
	//cout << "x: " << (pacmanX + 8) / 16 << endl;
	cout << "grid: " << pacmanGridX << endl;


}

void movePacMan(unsigned char key, int x, int y) 
{
	switch (key) {
	case 27:  // escape key
		exit(0);
		break;
	case 'd'://right
		mouthDirection = 0;
		pacmanVX = playerSpeed;
		pacmanVY = 0;
		//if (pacmanGridY < 28 - 1 && !grid[pacmanGridX][pacmanGridY + 1]) {
		//	pacmanX += 1;
		//	pacmanGridY += 1;
		//}
		break;
	case 'a'://left
		mouthDirection = 1;
		pacmanVX = -playerSpeed;
		pacmanVY = 0;

		//if (pacmanGridY > 0 && !grid[pacmanGridX][pacmanGridY - 1]) {
		//	pacmanX -= 1;
		//	pacmanGridY -= 1;
		//}
		break;
	case 'w'://up
		mouthDirection = 2;
		pacmanVY = playerSpeed;
		pacmanVX = 0;

		//if (pacmanGridX > 0 && !grid[pacmanGridX - 1][pacmanGridY]) {
		//	pacmanY += 1;
		//	pacmanGridX -= 1;
		//}
		break;
	case 's'://down
		mouthDirection = 3;
		pacmanVY = -playerSpeed;
		pacmanVX = 0;

		//if (pacmanGridX < 31 - 1 && !grid[pacmanGridX + 1][pacmanGridY]) {
		//	pacmanY -= 1;
		//	pacmanGridX += 1;
		//}
		break;
	}




	glutPostRedisplay();
}

void drawTiles(void) 
{
    for (int i = 0; i < 31; i++) //loop through the height of the map
    {
        for (int j = 0; j < 28; j++) //loop through the width of the map
        {
			if (maze[i][j] == 4)
			{
				glPushMatrix(); //push the matrix so that our translations only affect this tile
				glTranslatef(j, -i, 0); //translate the tile to where it should belong

				glColor3f(0, 0, 1);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glEnd();

				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(0.500000, -0.500000, 0.500000);
				glVertex3f(0.500000, -0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, -0.500000);
				glVertex3f(0.500000, 0.500000, 0.500000);
				glEnd();
				
				glColor3f(0, 128, 128);
				glBegin(GL_POLYGON);
				glVertex3f(-0.500000, -0.500000, -0.500000);
				glVertex3f(-0.500000, -0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, 0.500000);
				glVertex3f(-0.500000, 0.500000, -0.500000);
				glEnd();
			}
			if (maze[i][j] == 1) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0); 

				glColor3f(255,255,255);
				glutSolidSphere(0.13f, 10, 10);
			}
			
			if (maze[i][j] == 2) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glColor3f(1, 0, 0);
				glutSolidSphere(0.3f, 10, 10);
			}
			
			if (maze[i][j] == 3) 
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glBegin(GL_LINES);//horizontalno
				glColor3f(255, 255, 255);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(-0.5f, 0.5f);
				glEnd();
			}

			if (maze[i][j] == 5)
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);

				glEnd();
			}

			if (maze[i][j] == 6)
			{
				glPushMatrix();
				glTranslatef(j, -i, 0);
				drawMouth();
				drawPacMan();
			}

            glPopMatrix();
        }
    }
}

void display(void) 
{
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	float deltaTime = (currentTime - previousTime) / 1000.0f; // convert to seconds
	previousTime = currentTime;
    glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
    glTranslatef(-13.5, 15, -30); //translate back a bit to view the map correctly

    drawTiles(); 
	updatePacman(deltaTime);


    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void saveTileLocation()
{
	for (int i = 0; i < 31; i++) //loop through the height of the map
	{
		for (int j = 0; j < 28; j++) //loop through the width of the map
		{
			if (maze[i][j] == 4 || maze[i][j] == 3)
			{
				pairNumbers = make_pair(i,j);
				walls.push_back(pairNumbers);
				grid[i][j] = true;
			}
			if (maze[i][j] == 6)
			{
				cout << i << " " << j << endl;
				pacmanGridX = i;
				pacmanGridY = j;
			}
			if (maze[i][j] == 5)
			{
				//redGhostGridX = i;
				//redGhostGridY = j;
			}
		}
	}
}

int main(int argc, char** argv) 
{
	saveTileLocation();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(600, 200);
    glutCreateWindow("A basic OpenGL Window");
    glutDisplayFunc(display);
	glutKeyboardFunc(movePacMan);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
