#ifndef GRAPHICSDLL_SHADERLIB
#define GRAPHICSDLL_SHADERLIB

#include <d3d11.h>
#include <DirectXMath.h>

namespace ShaderLib
{
	enum ShaderVariations
	{
		Normal			  = 0,
		Instanced		  = 1,
		Animated		  = 2,
		InstancedAnimated = 3,
		Wireframe = 4
	};


	enum RasteriserState
	{
		RASTER_NORMAL,
		RASTER_WIREFRAME
	};


}

#endif