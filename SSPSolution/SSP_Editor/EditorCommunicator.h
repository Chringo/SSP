#pragma once
#include "EditorInputHandler.h"
#include "Header.h"
#include "../GraphicsDLL/GraphicsHandler.h"
#include "../GraphicsDLL/Camera.h"
#include "LevelHandler.h"
#include "SelectionHandler.h"


//struct Container
//{
//	unsigned int internalID;
//	DirectX::XMVECTOR position; // Total v�rde. 
//	float rotation; //Total v�rde. 
//	GraphicsComponent component;
//};

class Communicator
{
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	Camera* m_Camera;

	int m_Width;
	int m_Height;
	bool m_IsPreview;

	Level* m_currentLevel;
	
	GraphicsHandler* m_GraphicsHandler;
	EditorInputHandler* m_EditorInputHandler;
public:
	Communicator();
	~Communicator();
	Resources::Status Initialize(HWND hwnd, HINSTANCE hinstance, int w, int h, bool isPreview, std::vector<Resources::Model*>* modelPtr);
	Resources::Status Release();

	SelectionHandler * GetSelectionHandler() { return SelectionHandler::GetInstance(); };

public:
	//getters and setters
	ID3D11Device*			GetDevice() { return this->m_GraphicsHandler->GetDevice(); };
	Camera*					GetCamera() {return this->m_Camera;};
	GraphicsHandler*		GetGraphicsHandler()	const;
	EditorInputHandler*		GetEditorInputHandler() const;
	bool					GetIsPreview()			const;

	void SetMousePos(QPoint point) { this->m_EditorInputHandler->SetMousePos(point); };
	
	Resources::Status FindModel(int modelID, std::vector<Container>* modelPtr);
	Resources::Status GetComponent(unsigned int modelID, unsigned int instanceID, Container& container);
	Resources::Status AddModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateModel(unsigned int modelID, unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);
	Resources::Status UpdateSpawnPoint(unsigned int instanceID, DirectX::XMVECTOR position, DirectX::XMVECTOR rotation);

	Resources::Status RemoveModel(unsigned int modelID, unsigned int instanceID);
	void ViewPortChanged(float height, float width);
	Level* GetCurrentLevel() { return m_currentLevel; };
};