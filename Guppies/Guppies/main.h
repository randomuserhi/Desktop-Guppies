#pragma once

struct Circle
{
	float x;
	float y;
	float vx;
	float vy;
	float r;
	float g;
	float b;
	float a;
	float radius;
};

#define Balls 10
extern Circle buffer[];

void Init();
void Update(float dt);