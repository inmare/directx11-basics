#include "systemclass.h";

// object pointer�� ������ �� null�� ����,
// shutdown�ÿ� ���� object�� �ν��ϰ� ���ֵ��� ��
SystemClass::SystemClass()
{
	m_Input = 0;
	m_Application = 0;
}

// ���� ������
// ������� ������ �����ؼ� ���� ����
SystemClass::SystemClass(const SystemClass& other) 
{
}

// �Ҹ���
// ���Ǵ� �ص����� ���� �۾��� Shutdown()�Լ����� ó��
// �����쿡���� ������ �Լ� ȣ���� �ȵǴ� ��찡 ����
SystemClass::~SystemClass() 
{ 
}

// �ʱ�ȭ �Լ�
bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	// ������ api �ʱ�ȭ
	InitializeWindows(screenWidth, screenHeight);

	// input object ���� �� �ʱ�ȭ
	// Ű���� �Է��� �����
	m_Input = new InputClass;
	m_Input->Initialize();

	// application class object ���� �� �ʱ�ȭ
	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}

// ������ ����ϴ� �Լ�
void SystemClass::Shutdown()
{
	// applicaion class object ����
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}
	// input object ����
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// window ����
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// message ���� �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	// quit message�� �����쳪 ��������� ���� ������ ���� ����
	while (!done) 
	{
		// ������ message ����
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT)
		{
			// �����찡 ������� ��ȣ�� ������ ������
			done = true;
		}
		else
		{
			// �׷��� ���� ��쿡�� ��� frame ó��
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

// Frame �Լ�: application�� ���� ��� ó���� �̷������ ��
// ������� �Է��� ���ؼ� �׸����� ���θ� �Ǵ���
bool SystemClass::Frame()
{
	bool result;

	// ����ڰ� escŰ�� �������� �Ǵ�
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}
	// frame processing
	result = m_Application->Frame();
	if (!result)
	{
		return false;
	}

	return true;
}

// �Է� ���� key�� ���� ó���ϴ� MessageHandler �Լ�
// CALLBACK�� ������ �ý����� �Լ��� ���� ȣ���ϱ� ���� �Ծ��� �����ٴ� �ǹ�
// �ü���� �ʿ��� �� �� �Լ��� �θ� �� �ְ� ��
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// keydown ó��
		case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		// keyup ó��
		case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		// �׿ܿ��� ������� �ʱ⿡ default message handler�� ������
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc = WNDCLASSEX();
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �� object�� �ܺ� pointer�� ������
	// �̸� ���ؼ� �ܺ��Լ��� WinProc���� Ŭ������ ������ �� �ִ� �ּҸ� ������ ��
	ApplicationHandle = this;
	// application�� instance �Ҵ�
	m_hinstance = GetModuleHandle(NULL);
	// application�� �̸� �ο�
	m_applicationName = L"Engine";

	// �⺻���� ������ ����
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// ������ Ŭ������ ����ϱ�
	RegisterClassEx(&wc);
	// Ŭ���̾�Ʈ ����ũž ȭ�� ũ��
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// ��üȭ���� ���� �ƴ� ���� ��ũ�� ���� ����
	if (FULL_SCREEN) {
		// ��üȭ���̸� ��ũ�� ũ�⸦ ����ũž ȭ�� ũ��� �����ϰ� 32bit�� ����
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// display ������ ��ü ȭ������ ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// ������ ��ġ�� ���� ���� ����
		posX = posY = 0;
	}
	else
	{
		// ��üȭ���� �ƴѰ�� �����츦 ȭ�� �߾ӿ� 800x600ũ��� ����
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ��ũ�� ������ ���� ������ �ĺ���(handle) �����ϱ�
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�� �߾ӿ� �������� ��Ŀ�� ���߱�
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// ���콺 Ŀ�� �����
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ�� ���̱�
	ShowCursor(true);

	// ��üȭ�� ����� ��� ���÷��� ���� ��ġ��
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// ������ ����
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// application �ν��Ͻ� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// class�� ���ϴ� pointer �ʱ�ȭ
	ApplicationHandle = NULL;

	return;
}

// ������ �޼����� ó���ϴ� �Լ��� SystemClass ���� ���ο� �ۼ��ؼ� �ڵ带 ����ϰ� ������
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// ������ �ı���
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// �����찡 ������
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// �ٸ� �޼����� SystemClass�� message handler�� ������
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}