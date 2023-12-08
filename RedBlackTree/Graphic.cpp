#include "Graphic.h"

Graphic::Graphic()
	:
	hdc(nullptr),
	hwnd(nullptr)	
{
	
}

Graphic::~Graphic()
{
	ReleaseDC(hwnd, hdc);
}

void Graphic::Init(HWND _hwnd)
{
	hwnd = _hwnd;
	hdc = GetDC(hwnd);
}

void Graphic::draw()
{



	// 화면 클리어

	// present

}

void Graphic::clear()
{

}
