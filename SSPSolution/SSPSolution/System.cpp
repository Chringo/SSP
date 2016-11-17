#include "System.h"



System::System()
{
}


System::~System()
{
}

int System::Cleanup()
{
	int result = 0;
	//Destroy the display window
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	SDL_Quit();
	return result;
}

int System::Initialize()
{
	int result = 1;
	this->m_fullscreen = false;
	this->m_running = true;
	this->window = NULL;


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL failed in initializing the window! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		printf("SDL succeeded in initializing the window!\n");
	}

	window = SDL_CreateWindow("SSD Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window creation failed! SDL_ERROR: %S\n", SDL_GetError());
	}
	else
	{
		printf("Window creation succeeded!\n");

		SDL_SysWMinfo wmInfo;
		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);
		hwnd = wmInfo.info.win.window;
	}

	return result;
}

int System::Run()
{
	int result = 0;

	while (this->m_running)
	{
		result = this->HandleEvents();
		int tempResult = 5;
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
		case SDL_QUIT:
		{
			this->m_running = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			//OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode);
			break;
		}
		case SDL_KEYUP:
		{
			//OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod, Event->key.keysym.scancode);
			break;
		}
		case SDL_MOUSEMOTION:
		{
			//OnMouseMove(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			/*switch (Event->button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			OnLButtonDown(Event->button.x, Event->button.y);
			break;
			}
			case SDL_BUTTON_RIGHT:
			{
			OnRButtonDown(Event->button.x, Event->button.y);
			break;
			}
			case SDL_BUTTON_MIDDLE:
			{
			OnMButtonDown(Event->button.x, Event->button.y);
			break;
			}
			}*/
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			/*switch (Event->button.button)
			{
			case SDL_BUTTON_LEFT:
			{
			OnLButtonUp(Event->button.x, Event->button.y);
			break;
			}
			case SDL_BUTTON_RIGHT:
			{
			OnRButtonUp(Event->button.x, Event->button.y);
			break;
			}
			case SDL_BUTTON_MIDDLE:
			{
			OnMButtonUp(Event->button.x, Event->button.y);
			break;
			}
			}*/
			break;
		}
		}
	}
	return 1;
}

int System::FullscreenToggle()
{
	int result = 0;
	return result;
}
