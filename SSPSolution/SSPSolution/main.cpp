#include "System.h"
#include <crtdbg.h>
#include <omp.h>

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//std::string path;
	////"//DESKTOP-BOKNO6D/server/Assets/bbf files/Levels/Sewerslvl1.level";
	//System* mySystem = new System();
	//if (argc > 1)
	//{
	//	std::string path(argv[1]);
	//	mySystem->Initialize(path);
	//	printf("%s", path.c_str());
	//}
	//else {
	//	mySystem->Initialize();
	//}

	//mySystem->Run();
	//mySystem->Shutdown();
	//delete mySystem;

	int iCPU = omp_get_num_procs();
	// This statement should only print once
	printf("Starting Program!\n");

#pragma omp parallel
	{
		// This statement will run on each thread.
		// If there are 4 threads, this will execute 4 times in total
		printf("Running on multiple threads\n");
	}

	// We're out of the parallelized secion.
	// Therefor, this should execute only once
	printf("Finished!\n");

	return 0;
}