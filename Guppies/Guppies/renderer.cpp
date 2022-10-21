#include "Deep.h"
#include "renderer.h"
#include "main.h"

int w = 1920;
int h = 1080;

void DrawCircle(float oriX, float oriY, float radius)
{
	glBegin(GL_POLYGON);
	int resolution = 20;
	for (int i = 0; i <= resolution; i++) {
		float angle = 2.0f * 3.141592654f * i / resolution;
		float x = cos(angle) * radius;
		float y = sin(angle) * radius;
		glVertex2f(oriX + x, oriY + y);
	}
	glEnd();
}

BOOL Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	/*glBegin(GL_TRIANGLES);                              // Drawing Using Triangles
	glColor4f(1.0f, 0.0f, 0.0f, 0);                      // Set The Color To Red
	glVertex3f(0.0f, 1.0f, 0.0f);                  // Top
	glColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
	glVertex3f(-1.0f, -1.0f, 0.0f);                  // Bottom Left
	glColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
	glVertex3f(1.0f, -1.0f, 0.0f);                  // Bottom Right
	glEnd();*/

	glLoadIdentity();                           // Reset The Projection Matrix
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, w, 0, h, -1, 1);                      // Set Up An Ortho Screen

	glTranslated(0, 0, 0);
	glScaled(1, 1, 0); // Scales from bottom left, bruh

	for (int i = 0; i < Balls; ++i)
	{
		glColor4f(buffer[i].r, buffer[i].g, buffer[i].b, buffer[i].a);
		DrawCircle(buffer[i].x, buffer[i].y, buffer[i].radius);
	}

	glPopMatrix();
	glFlush();

	return 0;
}