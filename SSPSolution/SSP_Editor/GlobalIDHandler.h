#ifndef SSPEDITOR_GlobalIDHandler_H
#define SSPEDITOR_GlobalIDHandler_H
#include <vector>
#include <unordered_map>

/* Author: Martin Clementson
	This is a simple class with the sole purpose of handling the unique ids in the level.
	 When GetNewId is called, the currenID is incremented with 1,
*/
class GlobalIDHandler
{
private:
	GlobalIDHandler();
	unsigned int currentID = 1; // 0 and 1 is reserved for Player Ids 
	std::vector<unsigned int> ids;
public:
	~GlobalIDHandler();
	static GlobalIDHandler* GetInstance();
	bool ReturnRemovedIndex(int index);
	const unsigned int GetNewId();
	const unsigned int AddExistingID(unsigned int id); //Used when loading file. If the id exists, it generates a new id. (This fixes old level files)
	void ResetIDs();

};

#endif