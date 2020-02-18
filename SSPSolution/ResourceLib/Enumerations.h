#ifndef RESOURCELIB_ENUM_ENUM_H
#define RESOURCELIB_ENUM_ENUM_H
#pragma warning( disable: 4996 )
#include <d3d11_1.h>

namespace Resources
{
	enum Memory
	{
		MEM_LEVEL,
		MEM_RES,
		MEM_CHUNKS
	};
	enum ResourceType
	{
		RES_UNKOWN	    = -1,
		RES_TEXTURE		= 0,
		RES_MATERIAL    = 1,
		RES_MESH		= 2,
		RES_MODEL		= 3,
		RES_SKELETON	= 4,
		RES_SOUND		= 5,
		RES_LIGHT		= 6,
		RES_ANIMATION	= 7,
		RES_UI			= 8,
		RES_NUMTYPES	= 9
	};

	enum TextureType
	{
		TEXTURE_UNKNOWN		= -1,
		TEXTURE_ALBEDO		= 0,
		TEXTURE_SPECULAR	= 1,
		TEXTURE_ROUGHNESS   = 2,
		TEXTURE_NORMAL		= 3,
		TEXTURE_AO			= 4
	};

	enum Status
	{
		ST_OK,
		ST_WRONG_RESTYPE,
		ST_RES_MISSING,
		ST_MEM_FULL,
		ST_BUFFER_ERROR,
		ST_RES_ALREADY_LOADED,
		ST_DEVICE_MISSING,
		ST_ERROR_OPENING_FILE,
		ST_FILE_CLOSED,
		ST_USING_PLACEHOLDER

	};

	static bool SAFE_RELEASE(IUnknown* dxBuffer)
	{

 		if (dxBuffer != nullptr || dxBuffer != NULL)
		{
			try {
				while (dxBuffer->Release()>0);
			}
			catch (...)
			{
				return false;
			}
			
		}
		return true;

	}





}
#endif