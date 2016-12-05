#pragma once
#include "Resource.h"

namespace Resources {

	class Animation :
		public Resource
	{
	public:
		Animation();
		virtual ~Animation();

		virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr);
	};
}

