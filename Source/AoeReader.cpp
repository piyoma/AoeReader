// AoeReader.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "AoeReader.h"
#include "Reader.h"
#pragma comment(lib, "shlwapi.lib")

//thread 
//1===db
//2===doc
//3==ui

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
			//_CrtSetBreakAlloc(114518);

	
			ReaderClient client;
				
				client.Run();
				
				 return 0;
}



