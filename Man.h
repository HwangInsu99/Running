#pragma once
#include "Object.h"
class Man
{
public:
	static const int DIR_STOP = 0;
	static const int DIR_LEFT = 1;
	static const int DIR_RIGHT = 2;

	float G = 3.0f; // 중력
	float V = 18.0f; // 점프 속도
	float Y = 300.0f; // 지면
	int MAX_JUMP_COUNT = 2;

	int count;
	CImage images[4];
	float x, y;
	int width, height;
	int direction;
	float v; // 점프 상태의 속도
	bool jump; // 정프상태확인
	int jumpCount;

	Man();
	void LoadImages();
	void Tick();
	void Draw(CDC& dc);
	void StartJUMP();
	bool Collide(Object* object);
	void Reset();
};

