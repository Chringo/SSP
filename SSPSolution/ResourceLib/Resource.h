#ifndef RESOURCELIB_RESOURCE_RESOURCE_H
#define RESOURCELIB_RESOURCE_RESOURCE_H
#include "DefineHeader.h"
#include <memory>
#include <unordered_map>
#include <deque>
#include "Enumerations.h"
#include <string>

namespace Resources
{

/*This is the base class of all resources. Each resource class i a descendant of this class*/
	class Resource
	{
	public:
		struct RawResourceData
		{
			//char m_name[256]; 
			unsigned int m_id;
			ResourceType m_resType = RES_UNKOWN;
		};
	
	protected:
		Resource(); 
		std::unordered_map<std::string, Resource*> m_connections;
		RawResourceData m_resourceData;

	public:
		DLL_OPERATION Resource(RawResourceData data);
		DLL_OPERATION virtual ~Resource();

		DLL_OPERATION const bool IsType(ResourceType type) const;
		DLL_OPERATION const ResourceType GetResourceType() const { return m_resourceData.m_resType; };

		DLL_OPERATION const unsigned int GetId() const;

		DLL_OPERATION virtual std::shared_ptr<char> GetDataAsBinary(size_t* size, bool* result = nullptr) = 0;

	};

	/*
	A resource container is used to keep track of references to a specific resource.
	When a level is loaded, all the assets that the level uses are incremented by 1 (or loaded if missing)
	When a level is unloaded all the assets used by it are decremented by 1.

	If the refCount of a rescontainer hits 0 it will be unloaded from memory.
	*/
	struct ResourceContainer
	{
		int refCount = 0; // when this hits 0 unload from memory
		Resource* resource;

		ResourceContainer() {};
		ResourceContainer(Resource* res, int ref = 1)
		{
			refCount = ref;
			resource = res;
		}
	};

	static void OutputErrorString(Resource* object, std::string info = "")
	{
		std::cout << "Error in object :" << object->GetId() << "| Additional info : " << info << std::endl;
	}
}

#endif

