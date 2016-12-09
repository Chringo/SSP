#ifndef AIDLL_AICOMPONENT_H
#define AIDLL_AICOMPONENT_H

#ifdef AIDLL_EXPORTS
#define AIDLL_API __declspec(dllexport)
#else
#define AIDLL_API __declspec(dllimport)
#endif

class AIDLL_API AIHandler
{
public:
	AIHandler();
	~AIHandler();
};
#endif