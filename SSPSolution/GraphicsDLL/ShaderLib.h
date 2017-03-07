#ifndef GRAPHICSDLL_SHADERS_SHADERLIB_H
#define GRAPHICSDLL_SHADERS_SHADERLIB_H

#include <d3d11_1.h>
#include <DirectXMath.h>

namespace ShaderLib
{
	enum ShaderVariations
	{
		Normal			  = 0,
		Instanced		  = 1,
		Animated		  = 2,
		InstancedAnimated = 3,
		Shadow			  = 4,
		InstancedShadow   = 5,
		AnimatedShadow	  = 6,
		Wireframe		  = 7,
		CubeMap_Generation = 8
	};


	enum RasteriserState
	{
		RASTER_NORMAL,
		RASTER_WIREFRAME
	};


}

#endif