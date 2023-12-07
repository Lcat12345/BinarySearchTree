#pragma once
#include <Windows.h>

class Graphic
{
public:
	Graphic();
	~Graphic();

	void Init(HWND _hwnd);
	void draw();

private:
	HDC hdc;
	HWND hwnd;
};

