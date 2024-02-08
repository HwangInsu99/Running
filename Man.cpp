#include "pch.h"
#include "Man.h"
#include "Object.h"

Man::Man() {
	count = 0;
	x = 50;
	y = Y;
	width = 40;
	height = 40;
	direction = DIR_STOP;
	jump = false;
	v = 0;
	jumpCount = 0;
}

void Man::LoadImages() {
	images[0].Load(L"images\\2.png");
	images[1].Load(L"images\\1.png");
	images[2].Load(L"images\\j1.png");
	images[3].Load(L"images\\j2.png");
}
void Man::StartJUMP() {
	if (jumpCount <= MAX_JUMP_COUNT) {
		jump = true;
		v = V;
		jumpCount++;
	}
}
void Man::Tick() {
	count++;
	switch (direction) {
	case DIR_LEFT:
		if (x != 0)
			x -= 5;
		break;
	case DIR_RIGHT:
		if (x != 500)
			x += 5;
		break;
	}
	if (jump) {
		y -= v;
		v -= G;
		if (y >= Y) {
			jump = false;
			y = Y;
			jumpCount = 0;
		}
	}
}
bool Man::Collide(Object* object) {
	CRect cacRect;
	object->GetRect(cacRect);
	CRect dinoRect((int)x, (int)y, (int)x + width, (int)y + height);
	CRect diff;
	return diff.IntersectRect(&cacRect, &dinoRect);
}
void Man::Reset()
{
	count = 0;
	x = 50;
	y = Y;
	width = 40;
	height = 40;
	direction = DIR_STOP;
	jump = false;
	v = 0;
	jumpCount = 0;
}
void Man::Draw(CDC& dc) {
	if (jump) {
		if (v > 0.0f) {
			images[2].Draw(dc, x, y, width, height);
		}
		else
			images[3].Draw(dc, x, y, width, height);
	}
	else
		images[count % 2].Draw(dc, x, y, width, height);

}
