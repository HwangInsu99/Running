#pragma once
#include "Object.h"
class Man
{
public:
	static const int DIR_STOP = 0;
	static const int DIR_LEFT = 1;
	static const int DIR_RIGHT = 2;

	float G = 3.0f; // �߷�
	float V = 18.0f; // ���� �ӵ�
	float Y = 300.0f; // ����
	int MAX_JUMP_COUNT = 2;

	int count;
	CImage images[4];
	float x, y;
	int width, height;
	int direction;
	float v; // ���� ������ �ӵ�
	bool jump; // ��������Ȯ��
	int jumpCount;

	Man();
	void LoadImages();
	void Tick();
	void Draw(CDC& dc);
	void StartJUMP();
	bool Collide(Object* object);
	void Reset();
};

