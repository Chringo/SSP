#pragma once

#ifdef RESOURCELIB_EXPORTS
#define DLL_OPERATION __declspec(dllexport)
#else
#define DLL_OPERATION __declspec(dllimport)
#endif
#include <iostream>

#ifdef _DEBUG
	#define LOG(x) std::cout << x << std::endl;
#else
	#define LOG(x)
#endif

