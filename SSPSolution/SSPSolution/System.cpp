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
	this->m_graphicsHandler->Shutdown();
	delete this->m_graphicsHandler;
	delete this->m_camera;
	this->m_inputHandler->Shutdown();
	delete this->m_inputHandler;
	this->m_physicsHandler.ShutDown();
	return result;
	
	//Shutdown Network module
	this->m_networkModule.Shutdown();
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
	Camera* oldCam = this->m_graphicsHandler->SetCamera(this->m_camera);
	delete oldCam;
	oldCam = nullptr;
	//Initialize the PhysicsHandler
	this->m_physicsHandler.Initialize();

	//Initialize the InputHandler
	this->m_inputHandler = new InputHandler();
	this->m_inputHandler->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//Init the network module
	this->m_networkModule.Initialize();

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
		prevTime = currTime;
		QueryPerformanceCounter(&currTime);
		elapsedTime.QuadPart = currTime.QuadPart - prevTime.QuadPart;
		elapsedTime.QuadPart *= 1000000;
		elapsedTime.QuadPart /= frequency.QuadPart;

		//Update the network module
		this->m_networkModule.Update();

		this->m_physicsHandler.Update();
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
		//std::cout << int(totalTime) << "\n";
		//Render
		this->m_graphicsHandler->Render();

	}
	if (this->m_fullscreen)
		this->FullscreenToggle();

	return result;
}

int System::Update(float deltaTime)
{
	int result = 1;
	int translateCameraX = 0, translateCameraY = 0, translateCameraZ = 0;
	int rotateCameraY = 0;
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
		rotateCameraY++;
	}
	if (this->m_inputHandler->IsKeyDown(SDL_SCANCODE_Q))
	{
		rotateCameraY--;
	}
	if (translateCameraY || translateCameraX || translateCameraZ || rotateCameraY)
	{
		DirectX::XMFLOAT3 posTranslation = DirectX::XMFLOAT3(float(translateCameraX) * (deltaTime / 1000000.0f), float(translateCameraY) * (deltaTime / 1000000.0f), float(translateCameraZ) * (deltaTime / 1000000.0f));
		this->m_camera->AddToCameraPos(posTranslation);
		this->m_camera->AddToLookAt(posTranslation);
		float rotationAmount = DirectX::XM_PI / 8;
		rotationAmount *= deltaTime / 1000000.0f;
		DirectX::XMFLOAT4 newRotation = DirectX::XMFLOAT4(0.0f, rotateCameraY * DirectX::XMScalarSin(rotationAmount / 2.0f), 0.0f, DirectX::XMScalarCos(rotationAmount / 2.0f));
		this->m_camera->SetRotation(newRotation);
		this->m_camera->Update();
	}
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
				printf("Failed to connect to the client\n", this->m_ip);
			}
			
		}
		else
		{
			printf("Join failed since this module is already connected to other clients\n");
		}
	}
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
