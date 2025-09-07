#include "systemclass.h";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// 새로운 system object 생성
	System = new SystemClass;

	// object를 초기화하고 작동시킴
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// system을 shutdown하고 release함
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}