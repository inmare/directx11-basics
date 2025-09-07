#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// 불필요한 Win32 헤더파일 제거용
#define WIN32_LEAN_AND_MEAN

// 윈도우 생성, 파괴 및 다른 함수 사용 용도
#include <windows.h>

// 직접 생성할 클래스들
#include "inputclass.h"
#include "applicationclass.h"

// SystemClass 및 main.cpp에서 사용하는 Initialize, Shutdown, Run등 정의
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	// 윈도우 시스템 메세지를 읽을 함수 정의
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// input, application을 관리할 포인터
	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// 윈도우 시스템 메세지를 클래스 내부로 보낼 함수, 변수
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;

#endif // _SYSTEMCLASS_H_


