#include "System.h"
#include <crtdbg.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int result = 1;
	std::string path;
	System* mySystem = new System();
	if (argc > 1)
	{
		std::string path(argv[1]);
		result = mySystem->Initialize(path);
		printf("%s", path.c_str());
	}
	else 
	{
		result = mySystem->Initialize();
	}
	if (result == 0)
	{
		mySystem->Run();
		mySystem->Shutdown();// This function is not safe...
	}
	delete mySystem;

	return result;
}