#include <iostream>
//#include <Eigen/Dense>

#include "Deep.h"

#include "main.h"

inline float frand()
{
	return ((double)rand() / (RAND_MAX));
}

Circle buffer[Balls];

void Init()
{
	for (int i = 0; i < Balls; ++i)
	{
		buffer[i].x = frand() * 1920.0f;
		buffer[i].y = frand() * 1080.0f;
		buffer[i].vx = (frand() * 2.0f - 1.0f) * frand() * 1000.0f + 100.0f;
		buffer[i].vy = (frand() * 2.0f - 1.0f) * frand() * 1000.0f + 100.0f;

		buffer[i].r = 0; // frand();
		buffer[i].g = 0; // frand();
		buffer[i].b = 0; // frand();
		buffer[i].a = 1; // frand() * 0.5f + 0.5f;

		buffer[i].radius = frand() * 50.0f + 10.0f;
	}
}

void Update(float dt)
{
	for (int i = 0; i < Balls; ++i)
	{
		buffer[i].x += buffer[i].vx * dt;
		buffer[i].y += buffer[i].vy * dt;

		if (buffer[i].x < 0.0f)
		{
			buffer[i].x = 0;
			buffer[i].vx *= -1.0f;
		}
		else if (buffer[i].x > 1920.0f)
		{
			buffer[i].x = 1920.0f;
			buffer[i].vx *= -1.0f;
		}

		if (buffer[i].y < 0.0f)
		{
			buffer[i].y = 0;
			buffer[i].vy *= -1.0f;
		}
		else if (buffer[i].y > 1080.0f)
		{
			buffer[i].y = 1080.0f;
			buffer[i].vy *= -1.0f;
		}
	}
}