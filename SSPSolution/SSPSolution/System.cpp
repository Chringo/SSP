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
	this->m_AIHandler.Shutdown();
	SoundHandler::instance().Shutdown();
	this->m_AnimationHandler->ShutDown();
	delete this->m_AnimationHandler;

	delete &Progression::instance();
	DebugHandler::instance()->Shutdown();
	
	return result;
}

int System::Initialize(std::string path)
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
	//Initialize the AIHandler
	this->m_AIHandler = AIHandler();
	this->m_AIHandler.Initialize(4);

	//Initialize the InputHandler
	this->m_inputHandler = new InputHandler();
	this->m_inputHandler->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, m_window);

	//Initialize the animation handler. 
	this->m_AnimationHandler = new AnimationHandler();
	this->m_AnimationHandler->Initialize(m_graphicsHandler->GetGraphicsAnimationComponents(), m_graphicsHandler->GetAmountOfGraphicAnimationComponents());

	//Initialize the ComponentHandler. This must happen before the initialization of the gamestatehandler
	this->m_componentHandler.Initialize(this->m_graphicsHandler, &this->m_physicsHandler, &this->m_AIHandler, this->m_AnimationHandler);
	//Initialize the GameStateHandler
	this->m_gsh.Initialize(&this->m_componentHandler, this->m_camera, path);

	//this->m_Anim = new Animation();

	DebugHandler::instance()->SetComponentHandler(&this->m_componentHandler);
	DebugHandler::instance()->CreateTimer(L"GS Update");
	DebugHandler::instance()->CreateTimer(L"Physics");
	DebugHandler::instance()->CreateTimer(L"Render");
	DebugHandler::instance()->CreateTimer(L"Frustum Cull");
	DebugHandler::instance()->CreateCustomLabel(L"Frame counter", 0);
	DebugHandler::instance()->CreateCustomLabel(L"Components in frustum", 0.0f);


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
	//Enable nesting of threading so that things can thread inside the Physics and Graphics parallel sections
	omp_set_nested(1);
	while (this->m_running)
	{
		DebugHandler::instance()->StartProgram();

		prevTime = currTime;
		QueryPerformanceCounter(&currTime);
		elapsedTime.QuadPart = currTime.QuadPart - prevTime.QuadPart;
		elapsedTime.QuadPart *= 1000000;
		elapsedTime.QuadPart /= frequency.QuadPart;

		//Prepare the InputHandler
		this->m_inputHandler->Update();
		//Handle events and update inputhandler through said events
		result = this->HandleEvents();
		this->m_inputHandler->mouseMovement(m_window);
		SDL_PumpEvents();
		//Update game
		/*if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			this->m_running = false;
		}*/
		if (!this->Update((float)elapsedTime.QuadPart))
		{
			this->m_running = false;
		}
		if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_GRAVE))
		{
			DebugHandler::instance()->ToggleDebugInfo();
		}
		if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_C))
		{
			DebugHandler::instance()->ResetMinMax();
			printf("Reset min max on timers\n");
		}

		DebugHandler::instance()->EndProgram();
		DebugHandler::instance()->DisplayOnScreen((float)elapsedTime.QuadPart);
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

	int result = 1;
#pragma omp parallel num_threads(3)
	{
		int myThreadID = omp_get_thread_num();
		//int amountOfThreads = omp_get_num_threads();
		//printf("My ID: %d Out of: %d\n", myThreadID, amountOfThreads);
		if (myThreadID == 0)
		{
			//Do the physics
#pragma region
	DebugHandler::instance()->StartTimer(1);

	this->m_physicsHandler.Update(deltaTime);

	DebugHandler::instance()->EndTimer(1);
#pragma endregion Physics
		}
		else if (myThreadID == 1)
		{
			//Render the objects from last frame
#pragma region
			//Render
			//Frustrum cull

			DebugHandler::instance()->StartTimer(3);
			int renderedItems = this->m_graphicsHandler->FrustrumCullOctreeNodeThreaded(1);
			//int renderedItems = this->m_graphicsHandler->FrustrumCullOctreeNode();
			
			DebugHandler::instance()->UpdateCustomLabel(1, float(renderedItems));
			DebugHandler::instance()->EndTimer(3);

			int nrOfComponents = this->m_physicsHandler.GetNrOfComponents();
#ifdef _DEBUG
			//for (int i = 0; i < nrOfComponents; i++)
			//{
			//	PhysicsComponent* temp = this->m_physicsHandler.GetComponentAt(i);
			//	if (temp->PC_BVtype == BV_AABB)
			//	{
			//		AABB* AABB_holder = nullptr;
			//		this->m_physicsHandler.GetPhysicsComponentAABB(AABB_holder, i);
			//		this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *AABB_holder);
			//	}
			//	if (temp->PC_BVtype == BV_OBB)
			//	{
			//		OBB* OBB_holder = nullptr;
			//		this->m_physicsHandler.GetPhysicsComponentOBB(OBB_holder, i);

			//		DirectX::XMVECTOR tempOBBpos = DirectX::XMVectorAdd(temp->PC_pos, OBB_holder->ort.r[3]);

			//		this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *OBB_holder);
			//	}
			//	if (temp->PC_BVtype == BV_Plane)
			//	{
			//		Plane* planeHolder = nullptr;
			//		this->m_physicsHandler.GetPhysicsComponentPlane(planeHolder, i);
			//		this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *planeHolder);
			//	}
			//	if (temp->PC_BVtype == BV_Sphere)
			//	{
			//		Sphere* sphereHolder = nullptr;
			//		this->m_physicsHandler.GetPhysicsComponentSphere(sphereHolder, i);
			//		this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *sphereHolder, DirectX::XMVectorSet(1, 1, 0, 0)); 
			//	}
			//}
			int nrOfBodyParts = this->m_physicsHandler.GetNrOfBodyComponents();
			PhysicsComponent* temp = nullptr;
			OBB* OBB_holder = nullptr;
			temp = this->m_physicsHandler.GetPlayer1Ragdoll()->playerPC;
			OBB_holder = &temp->PC_OBB;
			if (temp != nullptr)
			{
					this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *OBB_holder);
				}
			//for (int i = 0; i < nrOfBodyParts; i++)
			//{
			//	temp = this->m_physicsHandler.GetBodyComponentAt(i);
			//	Sphere* sphereHolder = nullptr;
			//	sphereHolder = &temp->PC_Sphere;
			//	this->m_graphicsHandler->RenderBoundingVolume(temp->PC_pos, *sphereHolder, DirectX::XMVectorSet(1, 1, 0, 0)); 
			//}
#endif  _DEBUG

			this->m_graphicsHandler->Update(deltaTime);

			DebugHandler::instance()->StartTimer(2);
			int objCntForRay = this->m_graphicsHandler->Render(deltaTime);
			DebugHandler::instance()->UpdateCustomLabel(2, float(objCntForRay));

			DebugHandler::instance()->EndTimer(2);
#pragma endregion Graphics and rendering
		}
			//Do AI and Animation
		else if (myThreadID == 2)
		{
#pragma region
	//AI
	this->m_AIHandler.Update(deltaTime);

			this->m_AnimationHandler->Update(deltaTime);
#pragma endregion AI And Animation
		}

	}
	//Part of this will sync physics data with graphics data. Future improvement would be to have another step doing this and placing logic in the parallel part
	//Sync data between the physics and graphics
#pragma region

#ifdef DEVELOPMENTFUNCTIONS
	//Save progress
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_F9))
	{
		bool result = Progression::instance().WriteToFile("Save1");

		if (result == false)
		{
			printf("Error with saving to file\n");
		}
		else
		{
			printf("Saved to file\n");
		}
	}
	//Load
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_F10))
	{
		bool result = Progression::instance().ReadFromFile("Save1");

		if (result == false)
		{
			printf("Error with loading from file\n");
		}
		else
		{
			printf("Loaded from file\n");
		}
	}

	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_KP_5))
	{
		SoundHandler::instance().ReInitSoundEngine();
	}
	if (this->m_inputHandler->IsKeyPressed(SDL_SCANCODE_F7))
	{
		this->m_graphicsHandler->ToggleOverviewCamera();
	}
#endif // DEVELOPMENTFUNCTIONS


	//this->m_AIHandler.Update(deltaTime);
	//this->m_AnimationHandler->Update(deltaTime);
	
	DebugHandler::instance()->StartTimer(0);

	//Update the logic and transfer the data from physicscomponents to the graphicscomponents
	enum { TOGGLE_FULLSCREEN = 511, EXIT_GAME = -2 };
	result = this->m_gsh.Update(deltaTime, this->m_inputHandler);
	if (result == TOGGLE_FULLSCREEN)
	{
		this->FullscreenToggle();
	}
	else if (result == EXIT_GAME)
	{
		result = 0;
	}
	DebugHandler::instance()->EndTimer(0);

	DebugHandler::instance()->UpdateCustomLabelIncrease(0, 1.0f);
#pragma endregion Logic

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
				this->m_inputHandler->captureMouse(SDL_TRUE);
				//SDL_CaptureMouse(SDL_TRUE);
				//OnInputFocus();
				break;
			}
			case SDL_WINDOWEVENT_FOCUS_LOST:
			{
				this->m_inputHandler->captureMouse(SDL_FALSE);
				//SDL_CaptureMouse(SDL_FALSE);
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
	PhysicsComponent* player = nullptr;

	DirectX::XMVECTOR diffVec = DirectX::XMVectorSubtract(camLookAt, camPos);
	
	player = this->m_physicsHandler.GetDynamicComponentAt(0);

	camPos = DirectX::XMVectorAdd(player->PC_pos, DirectX::XMVectorScale(diffVec, -3));
	camPos = DirectX::XMVectorAdd(camPos, DirectX::XMVectorSet(0, 3, 0, 0));
	camLookAt = DirectX::XMVectorAdd(camPos, diffVec);

	this->m_camera->SetCameraPos(camPos);
	this->m_camera->SetLookAt(camLookAt);

	this->m_physicsHandler.ApplyForceToComponent(player, DirectX::XMVectorSet(translateCameraX, translateCameraY, translateCameraZ, 0), 1.0f);
}


