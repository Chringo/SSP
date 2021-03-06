#include "D3DRenderWidget.h"

D3DRenderWidget::~D3DRenderWidget()
{
	Resources::Status status;
	status = this->m_Communicator->Release();
	if (status == Resources::ST_OK)
	{
		delete this->m_Communicator;
	}
}

void D3DRenderWidget::paintEvent(QPaintEvent * evt)
{
	GraphicsHandler*	GraphicsHptr = nullptr;
	EditorInputHandler*	EditorInputHptr = nullptr;
	bool isPreview = false;
	
	//get the desired values from EditorCommunicator
	GraphicsHptr    = this->m_Communicator->GetGraphicsHandler();
	EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	isPreview		= this->m_Communicator->GetIsPreview();

	QPoint local = this->mapFromGlobal(QCursor::pos());
	EditorInputHptr->SetMousePos(local);
	GraphicsHptr->clearEditor();

	this->m_frameCount++;
	if (getTime() > 1.0f)
	{
		this->m_fps = this->m_frameCount;
		this->m_frameCount = 0;
		startTimer();
	}
	this->m_frameTime = getFrameTime();

	static OBB* axisOBBs;
	static DirectX::XMVECTOR* axisOBBpositions;
	static OBB* selectedObjectOBB;
	static DirectX::XMVECTOR ** axisColors;
	static DirectX::XMVECTOR * OBBColor;
	
	if (!isPreview)
	{
		EditorInputHptr->KeyboardMovement(this->m_frameTime);
		EditorInputHptr->UpdateMouse();
		//this->m_Communicator->m_EditorInputHandler->MoveObject();
		//this->m_Communicator->m_EditorInputHandler->MousePicking();
	}
 	std::unordered_map<unsigned int, std::vector<Container>> *m_ModelMap = m_Communicator->GetCurrentLevel()->GetModelEntities();
	if (!m_ModelMap->empty())
	{
	Resources::Status st;
	std::vector<Container>* InstancePtr = nullptr;
	std::vector<Resources::Model*>* modelPtr = DataHandler::GetInstance()->GetModels();
	

	for (size_t i = 0; i < modelPtr->size(); i++)
		{
			std::unordered_map<unsigned int, std::vector<Container>>::iterator got = m_ModelMap->find(modelPtr->at(i)->GetId());
			
			if (got == m_ModelMap->end()) { // if does not exists in memory
				continue;
			}
			else {
				InstancePtr = &got->second;
				for (size_t j = 0; j < InstancePtr->size(); j++)
				{
					if (InstancePtr->at(j).isDirty)
					{
						this->m_Communicator->UpdateModel(modelPtr->at(i)->GetId(), InstancePtr->at(j).internalID, InstancePtr->at(j).position, InstancePtr->at(j).rotation);
						
						Container* ptr = &InstancePtr->at(j);
						SelectionHandler::GetInstance()->SetSelectedContainer(ptr);
						if (SelectionHandler::GetInstance()->HasSelection())
						{
							SelectionHandler::GetInstance()->GetSelectionRenderComponents(axisOBBs, axisOBBpositions, axisColors, selectedObjectOBB, OBBColor);
							SelectionHandler::GetInstance()->Update();
						}
					}

					GraphicsHptr->RenderFromEditor(
						modelPtr->at(i),
						&InstancePtr->at(j).component
					);
 				}
			}
		}
	}
	for (size_t i = 0; i < ContainerType::NUM_PUZZLE_ELEMENTS; i++)
	{
		const std::vector<Container*>* cont = m_Communicator->GetCurrentLevel()->GetPuzzleElements(ContainerType(i));
		for (size_t j = 0; j < cont->size(); j++)
		{
		
			GraphicsHptr->RenderFromEditor(
				cont->at(j)->component.modelPtr,
				&cont->at(j)->component);

			if (cont->at(j)->isDirty)
			{
				this->m_Communicator->UpdateModel(cont->at(j)->component.modelID, cont->at(j)->internalID, cont->at(j)->position, cont->at(j)->rotation);
				Container* ptr = cont->at(j);
				SelectionHandler::GetInstance()->SetSelectedContainer(ptr);
				if (SelectionHandler::GetInstance()->HasSelection())
				{
					SelectionHandler::GetInstance()->GetSelectionRenderComponents(axisOBBs, axisOBBpositions, axisColors, selectedObjectOBB, OBBColor);
					SelectionHandler::GetInstance()->Update();
				}
			}

		}

	}
	for (size_t i = 0; i < 2; i++)
	{
		Container* spawn =  m_Communicator->GetCurrentLevel()->GetSpawnPoint(i);
		this->m_Communicator->GetGraphicsHandler()->RenderFromEditor(
			spawn->component.modelPtr,
			&spawn->component);

		if (spawn->isDirty)
		{
			this->m_Communicator->UpdateSpawnPoint(i, spawn->position, spawn->rotation);
			if (SelectionHandler::GetInstance()->HasSelection())
			{

				SelectionHandler::GetInstance()->GetSelectionRenderComponents(axisOBBs, axisOBBpositions, axisColors, selectedObjectOBB, OBBColor);
				SelectionHandler::GetInstance()->Update();
			}
		}

	}


	if (SelectionHandler::GetInstance()->HasSelection())
	{
		if (SelectionHandler::GetInstance()->NeedsUpdate())
			SelectionHandler::GetInstance()->GetSelectionRenderComponents(axisOBBs, axisOBBpositions, axisColors, selectedObjectOBB, OBBColor);

		GraphicsHptr->RenderBoundingVolume(
			//SelectionHandler::GetInstance()->GetSelected()->position,
			*SelectionHandler::GetInstance()->GetOBBCenterPosition(),
			*selectedObjectOBB,
			*OBBColor
		);

		for (int i = 0; i < TransformWidget::NUM_AXIS; i++)
		{
			GraphicsHptr->RenderBoundingVolume(
				axisOBBpositions[i],
				axisOBBs[i],
				*axisColors[i]
			);
		}
	}	
	std::vector<AiContainer*>* container = m_Communicator->GetCurrentLevel()->GetAiHandler()->GetAllPathComponents();
	for (size_t i = 0; i < container->size(); i++)
	{
		this->m_Communicator->GetGraphicsHandler()->RenderFromEditor(
			container->at(i)->component.modelPtr,
			&container->at(i)->component);

		GraphicsHptr->RenderBoundingVolume(
			container->at(i)->aiComponent.AC_waypoints, 
			container->at(i)->aiComponent.AC_nrOfWaypoint);

		if (container->at(i)->isDirty)
		{
			Container* ptr = (Container*)container->at(i);
			SelectionHandler::GetInstance()->SetSelectedContainer(ptr);
			this->m_Communicator->UpdateModel(container->at(i)->component.modelID, container->at(i)->internalID, container->at(i)->position, container->at(i)->rotation);



			if (SelectionHandler::GetInstance()->HasSelection())
			{

				SelectionHandler::GetInstance()->GetSelectionRenderComponents(axisOBBs, axisOBBpositions, axisColors, selectedObjectOBB, OBBColor);
				SelectionHandler::GetInstance()->Update();
			}
		}
	}

	for each (Point* light in *m_Communicator->GetCurrentLevel()->GetLights())
	{
		if (light->isDirty)
		{
			light->Update();
			LIGHTING::LightHandler::GetInstance()->UpdateStructuredBuffer();
		
			if (SelectionHandler::GetInstance()->HasSelection())
				SelectionHandler::GetInstance()->Update();
		}
		GraphicsHptr->RenderBoundingVolume(light->position, light->pickSphere, { 1.0,1.0,1.0 });
		if(!LightController::GetInstance()->DisplayLightRadius())
			GraphicsHptr->RenderBoundingVolume(light->position, light->rangeSphere, { light->data->color.r, light->data->color.g, light->data->color.b, });
	}

	for each(CheckpointContainer * checkpoint in *m_Communicator->GetCurrentLevel()->GetCheckpoints())
	{
		if (checkpoint->isDirty)
		{
			checkpoint->Update();
			SelectionHandler::GetInstance()->Update();
		}
		GraphicsHptr->RenderBoundingVolume(checkpoint->position, checkpoint->obb, { 0.5, 0.5,0.0 });
	}


	GraphicsHptr->renderFinalEditor();
	this->update();
	

}

void D3DRenderWidget::resizeEvent(QResizeEvent * event)
{
	float aspect = 1.0f;
	float h = (float)parent->frameGeometry().height();
	float w = (float)parent->frameGeometry().width();

	this->frameGeometry().setWidth(w);
	this->frameGeometry().setHeight(h);
	if (h != 0)
		aspect = w / h;
	m_Communicator->ViewPortChanged(h, w);
}

void D3DRenderWidget::keyPressEvent(QKeyEvent * evt)
{
	EditorInputHandler*	EditorInputHptr = nullptr;
	
	//get the desired values from EditorCommunicator
	EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	EditorInputHptr->detectInput(this->m_frameTime, evt);
}

void D3DRenderWidget::keyReleaseEvent(QKeyEvent * evt)
{
	EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	//EditorInputHptr->detectInput(this->m_frameTime, evt);
	EditorInputHptr->keyReleased(evt);
}

void D3DRenderWidget::mousePressEvent(QMouseEvent * evt)
{
	EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	EditorInputHptr->mouseButtonDown(evt);
}

void D3DRenderWidget::mouseReleaseEvent(QMouseEvent * evt)
{
	EditorInputHandler*	EditorInputHptr = nullptr;

	//get the desired values from EditorCommunicator
	EditorInputHptr = this->m_Communicator->GetEditorInputHandler();
	EditorInputHptr->mouseButtonRelease(evt);
}

void D3DRenderWidget::Initialize(QWidget* parent, bool isPreview, FileImporter* fileImporter)
{
	Resources::Status st;
	InitDosConsole();
	
	this->m_Communicator = new Communicator();
	this->m_hwnd		 = (HWND)parent->winId();
	this->m_hInstance    = (HINSTANCE)::GetModuleHandle(NULL);

	float h = (float)parent->frameGeometry().height();
	float w = (float)parent->frameGeometry().width();

	st = this->m_Communicator->Initialize(
		this->m_hwnd,
		this->m_hInstance,
		w, 
		h, 
		isPreview,
		DataHandler::GetInstance()->GetModels()
	);
	
	

	this->frameGeometry().setWidth(w);
	this->frameGeometry().setHeight(h);
	
	this->m_Device = this->m_Communicator->GetDevice();
	this->m_fileImporter = fileImporter;
	this->m_fileImporter->setDevice(this->m_Device);
	DataHandler::GetInstance()->GetTextureHandler()->SetDevice(m_Device);
	//this->resizeEvent(nullptr); // Update the camera projection matrix to fit the widget window
}

D3DRenderWidget::D3DRenderWidget(QWidget* parent, FileImporter* fileImporter)
	: QWidget(parent) {
	this->parent = parent;
	setAttribute(Qt::WA_DontShowOnScreen, true);
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
	Initialize(parent, false, fileImporter);
	setFocusPolicy(Qt::StrongFocus);
}

void D3DRenderWidget::startTimer()
{
	LARGE_INTEGER frequencycount;

	QueryPerformanceFrequency(&frequencycount);
	m_countsPerSecond = double(frequencycount.QuadPart);

	QueryPerformanceCounter(&frequencycount);
	m_counterStart = frequencycount.QuadPart;
}

double D3DRenderWidget::getTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	return double(currentTime.QuadPart - m_counterStart) / m_countsPerSecond;
}

double D3DRenderWidget::getFrameTime()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - this->m_frameTimeOld;
	this->m_frameTimeOld = currentTime.QuadPart;

	if (tickCount < 0.0f)
	{
		tickCount = 0.0f;
	}

	return float(tickCount) / this->m_countsPerSecond;
}