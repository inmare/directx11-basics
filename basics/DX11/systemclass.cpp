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

void SystemClass::Shutdown()
{
	if (m_Application)
	{

	}
}