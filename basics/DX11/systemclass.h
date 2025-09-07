#pragma once

#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// ���ʿ��� Win32 ������� ���ſ�
#define WIN32_LEAN_AND_MEAN

// ������ ����, �ı� �� �ٸ� �Լ� ��� �뵵
#include <windows.h>

// ���� ������ Ŭ������
#include "inputclass.h"
#include "applicationclass.h"

// SystemClass �� main.cpp���� ����ϴ� Initialize, Shutdown, Run�� ����
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	// ������ �ý��� �޼����� ���� �Լ� ����
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// input, application�� ������ ������
	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// ������ �ý��� �޼����� Ŭ���� ���η� ���� �Լ�, ����
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemClass* ApplicationHandle = 0;

#endif // _SYSTEMCLASS_H_


