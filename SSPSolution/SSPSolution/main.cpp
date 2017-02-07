#include "System.h"
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string testpath = "//DESKTOP-BOKNO6D/server/Assets/bbf files/Levels/Sewerslvl1.level";

	System* mySystem = new System();
	mySystem->Initialize(testpath);
	mySystem->Run();
	mySystem->Shutdown();
	delete mySystem;
	return 0;
}