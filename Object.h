#pragma once
class Object
{
public:
	float x;
	float speed;
	int y;
	int RHeight;

	Object();
	void GetRect(CRect& rect);
	void Draw(CDC* pDC);
	void Tick();
	void Reset();
};

