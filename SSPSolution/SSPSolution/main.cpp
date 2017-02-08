#include "System.h"
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string testpath;
	//"//DESKTOP-BOKNO6D/server/Assets/bbf files/Levels/Sewerslvl1.level";
	System* mySystem = new System();
	if (argc > 1)
	{
		std::string testpath(argv[1]);
		mySystem->Initialize(testpath);
		printf("%s", testpath);
	}
	else {
		mySystem->Initialize(std::string(""));
	}

	mySystem->Run();
	mySystem->Shutdown();
	delete mySystem;
	return 0;
}