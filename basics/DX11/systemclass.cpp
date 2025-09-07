#include "systemclass.h";

// object pointer를 생성할 때 null로 설정,
// shutdown시에 없는 object로 인식하고 없애도록 함
SystemClass::SystemClass()
{
	m_Input = 0;
	m_Application = 0;
}

// 복사 생성자
// 사용하지 않지만 정의해서 복사 방지
SystemClass::SystemClass(const SystemClass& other) 
{
}

// 소멸자
// 정의는 해두지만 실제 작업은 Shutdown()함수에서 처리
// 윈도우에서는 실제로 함수 호출이 안되는 경우가 있음
SystemClass::~SystemClass() 
{ 
}

// 초기화 함수
bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = 0;
	screenHeight = 0;

	// 윈도우 api 초기화
	InitializeWindows(screenWidth, screenHeight);

	// input object 생성 후 초기화
	// 키보드 입력을 담당함
	m_Input = new InputClass;
	m_Input->Initialize();

	// application class object 생성 후 초기화
	m_Application = new ApplicationClass;

	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}
	return true;
}

// 정리를 담당하는 함수
void SystemClass::Shutdown()
{
	// applicaion class object 정리
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}
	// input object 정리
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// window 정리
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	// message 구조 초기화
	ZeroMemory(&msg, sizeof(MSG));

	done = false;
	// quit message가 윈도우나 사용자한테 오기 전까지 루프 실행
	while (!done) 
	{
		// 윈도우 message 관리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		if (msg.message == WM_QUIT)
		{
			// 윈도우가 닫으라는 신호를 보내면 나가기
			done = true;
		}
		else
		{
			// 그렇지 않은 경우에는 계속 frame 처리
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

// Frame 함수: application에 대한 모든 처리가 이루어지는 곳
// 사용자의 입력을 통해서 그만둘지 여부를 판단함
bool SystemClass::Frame()
{
	bool result;

	// 사용자가 esc키를 눌렀는지 판단
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

// 입력 받은 key의 값을 처리하는 MessageHandler 함수
// CALLBACK은 윈도우 시스템이 함수를 직접 호출하기 위한 규악을 따른다는 의미
// 운영체제가 필요할 때 이 함수를 부를 수 있게 됨
LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// keydown 처리
		case WM_KEYDOWN:
		{
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}
		// keyup 처리
		case WM_KEYUP:
		{
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}
		// 그외에는 사용하지 않기에 default message handler에 보내줌
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

	// 이 object의 외부 pointer를 가져옴
	// 이를 통해서 외부함수인 WinProc에서 클래스와 소통할 수 있는 주소를 제공해 줌
	ApplicationHandle = this;
	// application에 instance 할당
	m_hinstance = GetModuleHandle(NULL);
	// application에 이름 부여
	m_applicationName = L"Engine";

	// 기본적인 윈도우 세팅
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
	
	// 윈도우 클래스를 등록하기
	RegisterClassEx(&wc);
	// 클라이언트 데스크탑 화면 크기
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 전체화면일 때와 아닐 때의 스크린 설정 결정
	if (FULL_SCREEN) {
		// 전체화면이면 스크린 크기를 데스크탑 화면 크기로 설정하고 32bit로 설정
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// display 설정을 전체 화면으로 변경
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우 위치를 왼쪽 위로 설정
		posX = posY = 0;
	}
	else
	{
		// 전체화면이 아닌경우 윈도우를 화면 중앙에 800x600크기로 생성
		screenWidth = 800;
		screenHeight = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 스크린 설정을 가진 윈도우 식별자(handle) 생성하기
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
							WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면 중앙에 가져오고 포커스 맞추기
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// 마우스 커서 숨기기
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// 마우스 커서 보이기
	ShowCursor(true);

	// 전체화면 모드일 경우 디스플레이 설정 고치기
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 윈도우 제거
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// application 인스턴스 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// class를 향하는 pointer 초기화
	ApplicationHandle = NULL;

	return;
}

// 윈도우 메세지를 처리하는 함수를 SystemClass 파일 내부에 작성해서 코드를 깔끔하게 유지함
LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// 윈도우 파괴시
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// 윈도우가 닫힐때
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		// 다른 메세지는 SystemClass의 message handler로 보내짐
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}