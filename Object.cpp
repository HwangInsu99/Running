#include "pch.h"
#include "Object.h"

Object::Object() {
	x = 500.0f;
	y = 295;
	speed = 10.0f;
	//3���� ������ ����
	RHeight = rand() % 3;
	if (RHeight == 0)
		y = 315;
	else if (RHeight == 1)
		y = 295;
	else if (RHeight == 2)
		y = 275;
}
void Object::Tick() {
	x -= speed;
	//������Ʈ�� ���� ���� ������ ���̸� ���Ͽ� �ٽ� �ҷ���
	if (x <= 0) {
		RHeight = rand() % 3;
		if (RHeight == 0)
			y = 315;
		else if (RHeight == 1)
			y = 295;
		else if (RHeight == 2)
			y = 275;
		x = 500.0f;
		//�ִ� �ӵ��� �����ϱ������� �ӵ�����
		if (speed <= 50.0f)
			speed += 1.5f;
	}
}
void Object::Reset()
{
	x = 500.0f;
	speed = 10.0f;
	RHeight = rand() % 3;
	if (RHeight == 0)
		y = 315;
	else if (RHeight == 1)
		y = 295;
	else if (RHeight == 2)
		y = 275;
}
void Object::GetRect(CRect& rect) {
	rect.SetRect(x, y, x + 20, 350);
}
void Object::Draw(CDC* pDC) {
	pDC->Rectangle(x, y, x + 20, 350);
}