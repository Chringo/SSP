#include "System.h"
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string path;
	//"//DESKTOP-BOKNO6D/server/Assets/bbf files/Levels/Sewerslvl1.level";
	System* mySystem = new System();
	if (argc > 1)
	{
		std::string path(argv[1]);
		mySystem->Initialize(path);
		printf("%s", path.c_str());
	}
	else {
		mySystem->Initialize();
	}

	mySystem->Run();
	mySystem->Shutdown();
	delete mySystem;


	return 0;
}