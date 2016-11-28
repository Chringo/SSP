#pragma once
#define DLLEXPORT


#ifdef DLLEXPORT
#define DLL_OPERATION __declspec(dllexport)
#endif


#include <iostream>

