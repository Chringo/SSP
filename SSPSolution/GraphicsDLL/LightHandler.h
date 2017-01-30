#pragma once
#ifndef GRAPHICSDLL_LIGHTHSTRUCTS_H
#define GRAPHICSDLL_LIGHTHSTRUCTS_H
#include <DirectXMath.h>

#ifdef GRAPHICSDLL_EXPORTS
#define GRAPHICSDLL_API __declspec(dllexport)
#else
#define GRAPHICSDLL_API __declspec(dllimport)
#endif
#include <d3d11.h>
#include "LightStructs.h"
namespace LIGHT
{
	class GRAPHICSDLL_API LightHandler
	{

	private:
	public:

	};
}
#endif