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

void SystemClass::Shutdown()
{
	if (m_Application)
	{

	}
}