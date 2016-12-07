#pragma once
#include "EditorInputHandler.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"

struct Container
{
	unsigned int internalID;
	DirectX::XMVECTOR position; // Total värde. 
	float rotation; //Total värde. 
	GraphicsComponent component;
};

class Communicator
{
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Camera* m_Camera;

	int m_Width;
	int m_Height;

	std::unordered_map<unsigned int, std::vector<Container>> m_Map;

public:
	bool m_IsPreview;
	GraphicsHandler* m_GraphicsHandler;
	EditorInputHandler* m_EditorInputHandler;

	Communicator();
	~Communicator();
	Resources::Status Initialize(HWND hwnd, HINSTANCE hinstance, int w, int h, bool isPreview);
	Resources::Status Release();

public:
	Resources::Status GetModel(unsigned int modelID, unsigned int InstanceID, Container& container);
	Resources::Status AddModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, float rotation);
	Resources::Status UpdateModel(unsigned int modelID, unsigned int InstanceID, DirectX::XMVECTOR position, float rotation);
	Resources::Status RemoveModel(unsigned int modelID, unsigned int InstanceID);
};