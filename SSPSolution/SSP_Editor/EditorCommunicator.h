#pragma once
#include "EditorInputHandler.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"

struct Container
{
	unsigned int internalId;
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
	unsigned int m_InternalID;
	std::vector<Container> m_ModelContainer;


public:
	bool m_IsPreview;
	GraphicsHandler* m_GraphicsHandler;
	EditorInputHandler* m_EditorInputHandler;

	Communicator();
	~Communicator();
	Resources::Status Initialize(HWND hwnd, HINSTANCE hinstance, int w, int h, bool isPreview);
	Resources::Status Release();

public:
	Container GetModel(unsigned int id);
	void AddModel(Resources::Model model, DirectX::XMVECTOR position, float rotation);
	void UpdateModel(unsigned int id, DirectX::XMVECTOR position, float rotation);
	void RemoveModel(unsigned int id);
};