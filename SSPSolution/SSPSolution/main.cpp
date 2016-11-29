#include "System.h"
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	System* mySystem = new System();
	mySystem->Initialize();
	mySystem->Run();
	mySystem->Shutdown();
	delete mySystem;
	return 0;
}