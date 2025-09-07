#include "systemclass.h";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// ���ο� system object ����
	System = new SystemClass;

	// object�� �ʱ�ȭ�ϰ� �۵���Ŵ
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// system�� shutdown�ϰ� release��
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}