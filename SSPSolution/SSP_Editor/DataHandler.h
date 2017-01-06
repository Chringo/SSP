#ifndef SSPEDITOR_DATAHANDLER_DATAHANDLER_H
#define SSPEDITOR_DATAHANDLER_DATAHANDLER_H
#include "../ResourceLib/FileLoader.h"
#include "../ResourceLib/Model.h"
#include "../ResourceLib/FileHeaders.h"
#include "../ResourceLib/TextureHandler.h"
#include <unordered_map>

/*
	Author : Martin Clementson

	This class holds all data that is loaded from the server.
	It manages things in a way that avoids duplication of data.
	For example, it has one texture handler and only one, and no other texturehandler at all.
*/


class DataHandler
{
private:
	std::vector<Resources::Model*> m_models;
	std::vector<Resources::Mesh*>  m_meshes;
	std::vector<Resources::Material*> m_materials;
	Resources::TextureHandler* m_textureHandler;
	std::unordered_map<std::string, Resources::Texture*> m_textures;
	std::unordered_map<unsigned int, bool> loadedIds; //keeps track of all id's loaded.
	DataHandler();
public:
	Resources::Status AddModel(Resources::Model* model);
	Resources::Status AddMesh(Resources::Mesh* mesh);
	Resources::Status AddMaterial(Resources::Material* material);
	Resources::Status AddTexture(Resources::Texture* texture);
	bool IDExists(unsigned int& id); //function to check if the data is already loaded
	static DataHandler* GetInstance();
	~DataHandler();

	std::vector<Resources::Model*>* GetModels() { return &this->m_models; };
	std::vector<Resources::Mesh*>*  GetMeshes() { return &this->m_meshes; };
	std::vector<Resources::Material*>* GetMaterials() { return &this->m_materials; };
	std::unordered_map<std::string, Resources::Texture*>* GetTextures(){ return &this->m_textures; };

	Resources::Status GetTexture(std::string id, Resources::Texture*& texture);

	/*Temp*/Resources::TextureHandler* GetTextureHandler() { return this->m_textureHandler; };
};

#endif
