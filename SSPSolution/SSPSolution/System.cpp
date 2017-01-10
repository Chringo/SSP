#include "System.h"
System::System()
{
	this->m_inputHandler = NULL;
	this->m_window = NULL;
}


System::~System()
{
}

int System::Shutdown()
{
	int result = 0;
	//Destroy the display window
	SDL_DestroyWindow(m_window);
	//Quit SDL subsystems
	SDL_Quit();
	this->m_gsh.ShutDown();
	this->m_graphicsHandler->Shutdown();
	delete this->m_graphicsHandler;
	this->m_graphicsHandler = nullptr;
	delete this->m_camera;
	this->m_camera = nullptr;
	this->m_inputHandler->Shutdown();
	delete this->m_inputHandler;
	this->m_inputHandler = nullptr;
	this->m_physicsHandler.ShutDown();
	DebugHandler::instance().Shutdown();

	/*Delete animation class ptr here.*/
	//delete this->m_Anim;

	return result;
	

}

int System::Initialize()
{
	int result = 1;
	this->m_fullscreen = false;
	this->m_running = true;
	this->m_window = NULL;
	//Get the instance if this application
	this->m_hinstance = GetModuleHandle(NULL);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL failed in initializing the window! SDL_Error: %hS\n", SDL_GetError());
	}
	else
	{
		printf("SDL succeeded in initializing the window!\n");
	}

	m_window = SDL_CreateWindow("SSD Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (m_window == NULL)
	{
		printf("Window creation failed! SDL_ERROR: %hS\n", SDL_GetError());
	}
	else
	{
		printf("Window creation succeeded!\n");

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_window, &wmInfo);
		m_hwnd = wmInfo.info.win.window;
	}

	this->m_graphicsHandler = new GraphicsHandler();
	if (this->m_graphicsHandler->Initialize(&this->m_hwnd, DirectX::XMINT2(SCREEN_WIDTH, SCREEN_HEIGHT)))
	{
		printf("GraphicsHandler did not work. RIP!\n");
	}
	this->m_camera = new Camera();
	this->m_camera->Initialize();
	//this->m_camera->SetRotationAroundPosOffset(0.0f, 1.0f, 1.0f);
	Camera* oldCam = this->m_graphicsHandler->SetCamera(this->m_camera);
	delete oldCam;
	oldCam = nullptr;
	//Initialize the PhysicsHandler

	this->m_physicsHandler.Initialize();
	this->CreateDummyObjects();

	//Initialize the InputHandler
	this->m_inputHandler = new InputHandler();
	this->m_inputHandler->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	//Initialize the ComponentHandler. This must happen before the initialization of the gamestatehandler
	this->m_componentHandler.Initialize(this->m_graphicsHandler, &this->m_physicsHandler);
	//Initialize the GameStateHandler
	this->m_gsh.Initialize(&this->m_componentHandler);
	//Initialize the network module
	this->m_networkModule.Initialize();

	this->m_Anim = new Animation();

	DebugHandler::instance().CreateCustomLabel("Frame counter", 0);

	return result;
}

//Do not place things here without talking to the system designers. Place any update method in the System::Update(float dt) method
int System::Run()
{
	int result = 0;
	LARGE_INTEGER frequency, currTime, prevTime, elapsedTime;
	QueryPerformanceFrequency(&frequency);
	//QueryPerformanceCounter(&prevTime);
	QueryPerformanceCounter(&currTime);
	while (this->m_running)
	{
		DebugHandler::instance().StartProgram();
		prevTime = currTime;
		QueryPerformanceCounter(&currTime);
		elapsedTime.QuadPart = currTime.QuadPart - prevTime.QuadPart;
		elapsedTime.QuadPart *= 1000000;
		elapsedTime.QuadPart /= frequency.QuadPart;

		//Prepare the InputHandler
		this->m_inputHandler->Update();
		//Handle events and update inputhandler through said events
		result = this->HandleEvents();
		SDL_PumpEvents();
		//Update game
		if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			this->m_running = false;
		}
		if (!this->Update((float)elapsedTime.QuadPart))
		{
			this->m_running = false;
		}
		if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_F))
		{
			this->FullscreenToggle();
		}
		if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_C))
		{
			DebugHandler::instance().ResetMinMax();
			printf("Reseted min max on timers\n");
		}
		
		DebugHandler::instance().EndProgram();
		DebugHandler::instance().Display((float)elapsedTime.QuadPart);
	}
	if (this->m_fullscreen)
		this->FullscreenToggle();

	return result;
}

//Place all the update functions within the System::Update(float deltaTime) function.
int System::Update(float deltaTime)
{
	if (deltaTime < 0.000001f)
		deltaTime = 0.000001f;
	DebugHandler::instance().StartTimer("Update");
	int result = 1;

	//Update the network module
	this->m_networkModule.Update();

	PhysicsComponent* tempPlayer = nullptr;
	tempPlayer = this->m_physicsHandler.getDynamicComponentAt(1);
	DirectX::XMFLOAT3 playerPos;
	DirectX::XMFLOAT3 cameraPos = this->m_camera->GetCameraPos();
	
	//uncomment this to move player with camera
	//DirectX::XMStoreFloat3(&playerPos, tempPlayer->PC_pos);
	
	int translateCameraX = 0,translateCameraY = 0, translateCameraZ = 0;

	//translateCameraX = playerPos.x - cameraPos.x;
	//translateCameraY = playerPos.y - cameraPos.y;
	//translateCameraZ = playerPos.z - cameraPos.z;
	//tempPlayer->PC_pos = DirectX::XMLoadFloat3(&cameraPos);

	int rotateCameraY = 0;
	std::list<CameraPacket> cList;

	//Check for camera updates from the network
	cList = this->m_networkModule.PacketBuffer_GetCameraPackets();

	if (!cList.empty())
	{
		std::list<CameraPacket>::iterator iter;

		for (iter = cList.begin(); iter != cList.end();)
		{
			this->m_camera->SetCameraPos((iter)->pos);
			this->m_camera->Update();
			iter++;	
		}

		cList.empty();	//When we have read all the packets, empty the list

	}

	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_W))
	{
		translateCameraZ++;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_S))
	{
		translateCameraZ--;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_SPACE))
	{
		translateCameraY++;
		if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_LSHIFT))
		{
			translateCameraY *= -1;
		}
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_D))
	{
		translateCameraX++;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_A))
	{
		translateCameraX--;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_E))
	{
		rotateCameraY += 3;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_Q))
	{
		rotateCameraY -= 3;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_B))
	{
		
	}
	if (translateCameraY || translateCameraX || translateCameraZ || rotateCameraY)
	{
		DirectX::XMFLOAT3 posTranslation = DirectX::XMFLOAT3(float(translateCameraX) * (deltaTime / 1000000.0f), float(translateCameraY) * (deltaTime / 1000000.0f), float(translateCameraZ) * (deltaTime / 1000000.0f));
		this->m_camera->ApplyLocalTranslation(posTranslation);
		//this->m_camera->AddToLookAt(posTranslation);
		float rotationAmount = DirectX::XM_PI / 6;
		rotationAmount *= deltaTime / 1000000.0f;
		DirectX::XMFLOAT4 newRotation = DirectX::XMFLOAT4(0.0f, rotateCameraY * DirectX::XMScalarSin(rotationAmount / 2.0f), 0.0f, DirectX::XMScalarCos(rotationAmount / 2.0f));
		this->m_camera->SetRotation(newRotation);
		//this->m_camera->Update();

		//Send updates over the network
		if (this->m_networkModule.GetNrOfConnectedClients() != 0)
		{
			DirectX::XMFLOAT4 updatePos;
			this->m_camera->GetCameraPos(updatePos);
			this->m_networkModule.SendCameraPacket(updatePos);
		}

	}
	this->m_camera->Update();
	//Network
	if(this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_J))
	{
		if (this->m_networkModule.GetNrOfConnectedClients() <= 0)	//If the network module is NOT connected to other clients
		{
			if (this->m_networkModule.Join(this->m_ip))				//If we succsefully connected
			{
				printf("Joined client with the ip %s\n", this->m_ip);
			}
			else
			{
				printf("Failed to connect to the client %s\n", this->m_ip);
			}
			
		}
		else
		{
			printf("Join failed since this module is already connected to other clients\n");
		}
	}
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_K))
	{
		this->m_networkModule.SendFlagPacket(DISCONNECT_REQUEST);
	}

	/*Testing to play different animations here based on the input. Temp place right now*/
	int animState = 0;

	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_1))
	{
		animState = 0;
		m_Anim->Push(animState, true);
	}

	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_2))
	{
		animState = 1;
		m_Anim->Push(animState, true);
	}

	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_3))
	{
		animState = 2;;
		m_Anim->Push(animState, true);
	}

	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_4))
	{
		animState = 3;
		m_Anim->Push(animState, true);
	}

	//Update animations here. Temp place right now.
	m_Anim->Update(deltaTime);
	m_graphicsHandler->SetTempAnimComponent((void*)m_Anim->GetAnimationComponentTEMP());

	//Update the logic and transfer the data from physicscomponents to the graphicscomponents
	this->m_gsh.Update(deltaTime, this->m_inputHandler);
	//Update the network module
	this->m_networkModule.Update();

#pragma region tempAI
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_L))
	{
		this->director.Initialize();
		this->director.Update(0);
	}
#pragma endregion

	int nrOfComponents = this->m_physicsHandler.getNrOfComponents();
	//temp input for testing chain
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_P))
	{
		PhysicsComponent* ballPtr = this->m_physicsHandler.getDynamicComponentAt(0);
		DirectX::XMVECTOR dir;
		dir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&this->m_camera->GetLookAt()), DirectX::XMLoadFloat3(&this->m_camera->GetCameraPos()));
		dir = DirectX::XMVectorAdd(dir, DirectX::XMVectorSet(0, 1, 0, 0));
		//dir = DirectX::XMVectorSet(0.4, 1, 0, 0);
		dir = DirectX::XMVectorScale(dir, 500);


		this->m_physicsHandler.ApplyForceToComponent(ballPtr, dir, 1.0);
		//ballPtr->PC_velocity = DirectX::XMVectorSet(3, 2, 0, 0);
		//ballPtr->PC_velocity = DirectX::XMVectorSet(1, 1.5, 0, 0);
	}
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_I))
	{
		PhysicsComponent* ballPtr = this->m_physicsHandler.getDynamicComponentAt(0);
		ballPtr->PC_velocity = DirectX::XMVectorSet(-3, 2, 0, 0);
	}

	this->m_physicsHandler.Update(deltaTime);

	for (int i = 0; i < nrOfComponents; i++)
	{
		PhysicsComponent* temp = this->m_physicsHandler.getDynamicComponentAt(i);
		if (temp->PC_BVtype == BV_AABB)
		{
			AABB* AABB_holder = nullptr;
			this->m_physicsHandler.GetPhysicsComponentAABB(AABB_holder, i);
			this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *AABB_holder);
		}
		if (temp->PC_BVtype == BV_OBB)
		{
			OBB* OBB_holder = nullptr;
			this->m_physicsHandler.GetPhysicsComponentOBB(OBB_holder, i);
			this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *OBB_holder);
		}
		if (temp->PC_BVtype == BV_Plane)
		{
			Plane* planeHolder = nullptr;
			this->m_physicsHandler.GetPhysicsComponentPlane(planeHolder, i);
			this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *planeHolder);
		}
	}

	//locks camera to the player
	//this->LockCameraToPlayer((float)translateCameraX, (float)translateCameraY, (float)translateCameraZ);

	DebugHandler::instance().UpdateCustomLabelIncrease(0, 1.0f);
	DebugHandler::instance().EndTimer();
	//Render
	DebugHandler::instance().StartTimer("Render");
	this->m_graphicsHandler->Render(deltaTime);
	DebugHandler::instance().EndTimer();
	return result;
}

int System::HandleEvents()
{
	SDL_Event m_event;
	while (SDL_PollEvent(&m_event))
	{
		switch (m_event.type)
		{
#pragma region
		case SDL_WINDOWEVENT:
		{
			switch (m_event.window.event)
			{
			case SDL_WINDOWEVENT_ENTER:
			{
				//OnMouseFocus();
				break;
			}
			case SDL_WINDOWEVENT_LEAVE:
			{
				//OnMouseBlur();
				break;
			}
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			{
				//OnInputFocus();
				break;
			}
			case SDL_WINDOWEVENT_FOCUS_LOST:
			{
				//OnInputBlur();
				break;
			}
			case SDL_WINDOWEVENT_SHOWN:
			{
				break;
			}
			case SDL_WINDOWEVENT_HIDDEN:
			{
				break;
			}
			case SDL_WINDOWEVENT_EXPOSED:
			{
				break;
			}
			case SDL_WINDOWEVENT_MOVED:
			{
				break;
			}
			case SDL_WINDOWEVENT_RESIZED:
			{
				break;
			}
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			{
				break;
			}
			case SDL_WINDOWEVENT_MINIMIZED:
			{
				break;
			}
			case SDL_WINDOWEVENT_MAXIMIZED:
			{
				break;
			}
			case SDL_WINDOWEVENT_RESTORED:
			{
				break;
			}
			case SDL_WINDOWEVENT_CLOSE:
			{
				break;
			}
			}
			break;
		}
#pragma endregion window events
		case SDL_MOUSEMOTION:
		{
			break;
		}
		case SDL_QUIT:
		{
			//The big X in the corner
			this->m_running = false;
			break;
		}
#pragma region
		case SDL_KEYDOWN:
		{
			//OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode);
			
			this->m_inputHandler->SetKeyState(m_event.key.keysym.scancode, true);
			break;
		}
		case SDL_KEYUP:
		{
			//OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode);
			this->m_inputHandler->SetKeyState(m_event.key.keysym.scancode, false);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			this->m_inputHandler->SetMouseState(m_event.button.button, true);
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			this->m_inputHandler->SetMouseState(m_event.button.button, false);
			break;
		}
#pragma endregion Key / Button events
		case SDL_MOUSEWHEEL:
		{
			this->m_inputHandler->ApplyMouseWheel(m_event.wheel.x, m_event.wheel.y);
			break;
		}
		}
	}
	return 1;
}

int System::FullscreenToggle()
{
	int result = 0;
	this->m_fullscreen = SDL_GetWindowFlags(this->m_window) & SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(this->m_window, this->m_fullscreen ? 0 : SDL_WINDOW_FULLSCREEN);
	this->m_fullscreen = SDL_GetWindowFlags(this->m_window) & SDL_WINDOW_FULLSCREEN;
	return result;
}

void System::LockCameraToPlayer(float translateCameraX, float translateCameraY, float translateCameraZ)
{
	DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&this->m_camera->GetCameraPos());
	DirectX::XMVECTOR camLookAt = DirectX::XMLoadFloat3(&this->m_camera->GetLookAt());
	PhysicsComponent* player= nullptr;

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(camLookAt, camPos);
	
	player = this->m_physicsHandler.getDynamicComponentAt(1);

	camPos = DirectX::XMVectorAdd(player->PC_pos, DirectX::XMVectorScale(diffVec, -3));
	camPos = DirectX::XMVectorAdd(camPos, DirectX::XMVectorSet(0, 3, 0, 0));
	camLookAt = DirectX::XMVectorAdd(camPos, diffVec);

	this->m_camera->SetCameraPos(camPos);
	this->m_camera->SetLookAt(camLookAt);

	this->m_physicsHandler.ApplyForceToComponent(player, DirectX::XMVectorSet(translateCameraX, translateCameraY, translateCameraZ, 0), 1.0f);
}

void System::CreateDummyObjects()
{
	//create random content
	PhysicsComponent* ptr = nullptr;
	DirectX::XMVECTOR tempPos;
	

	//ball
	tempPos = DirectX::XMVectorSet(0, 1.0, 60, 0);
	ptr = this->m_physicsHandler.CreatePhysicsComponent(tempPos, false);
	ptr->PC_mass = 200.f;
	ptr->PC_AABB.ext[0] = 0.3f;
	ptr->PC_AABB.ext[1] = 0.3f;
	ptr->PC_AABB.ext[2] = 0.3f;
	//AABB by default

	//play�r
	tempPos = DirectX::XMVectorSet(10.0, 1.0, 60, 0);
	ptr = this->m_physicsHandler.CreatePhysicsComponent(tempPos, false);
	ptr->PC_mass = 70.f;
	ptr->PC_AABB.ext[0] = 0.5f;
	ptr->PC_AABB.ext[1] = 0.5f;
	ptr->PC_AABB.ext[2] = 0.5f;
	//chain linku
	this->m_physicsHandler.CreateChainLink(0, 1, 10, 0.2f);


	//the g�lv
	tempPos = DirectX::XMVectorSet(0.0f, 0.0f, 60, 0);
	ptr = this->m_physicsHandler.CreatePhysicsComponent(tempPos, true);
	ptr->PC_BVtype = BoundingVolumeType::BV_Plane;
	ptr->PC_Plane.PC_normal = DirectX::XMVectorSet(0, 1, 0, 0);

	ptr->PC_friction = 0.2f;
	ptr->PC_elasticity = 0.5f;


	//the pl�tf�rm
	tempPos = DirectX::XMVectorSet(40.0, 5.0, 60, 0);
	ptr = this->m_physicsHandler.CreatePhysicsComponent(tempPos, true);
	ptr->PC_AABB.ext[0] = 20.f;
	ptr->PC_AABB.ext[1] = 3.0f;
	ptr->PC_AABB.ext[2] = 200.f;

	ptr->PC_friction = 0.2f;
	ptr->PC_elasticity = 0.5f;


}
