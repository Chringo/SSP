#include "MenuState.h"
#include "GameStateHandler.h"


MenuState::MenuState()
{
	this->timeoutTime = 0;
	this->isHosting = false;
	this->isJoining = false;
	this->sentSyncPacket = false;
}

MenuState::~MenuState()
{
}

int MenuState::ShutDown()
{
	int result = 1;
	return result;
}

int MenuState::Initialize(GameStateHandler * gsh, ComponentHandler* cHandler, Camera* cameraRef)
{
	int result = 0;

	result = GameState::InitializeBase(gsh, cHandler, cameraRef, false);

	this->m_cameraRef = cameraRef;
	
	//Workaround for camera trying to access nullptr
	this->m_lockTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR targetOffset = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	float distance = 4.0f;
	this->m_cameraRef->SetCameraPivot(&this->m_lockTarget, targetOffset, distance);

	this->inFullscreen = false;

	this->m_menuBG = cHandler->GetUIComponent();
	this->m_menuBG->active = 1;
	this->m_menuBG->position = DirectX::XMFLOAT2(0.0f, 0.0f);
	this->m_menuBG->size = DirectX::XMFLOAT2(1280.f, 720.f);
	this->m_menuBG->spriteID = 4;

	for (size_t i = 0; i < 3; i++) //Create the main menu buttons
	{
		UIComponent* tempUIComp = cHandler->GetUIComponent();
		tempUIComp->active = 1;
		tempUIComp->position = DirectX::XMFLOAT2(100.f, 200.f + (i * 150.f));
		tempUIComp->size = DirectX::XMFLOAT2(300.f, 100.f);
		tempUIComp->spriteID = 5;
		TextComponent* tempTextComp = cHandler->GetTextComponent();
		tempTextComp->active = 1;
		tempTextComp->position = DirectX::XMFLOAT2(100.f, 220.f + (i * 150.f));
		MenuButton button;
		button.m_uiComp = tempUIComp;
		button.m_textComp = tempTextComp;
		this->m_mainMenuButtons.push_back(button);
	}
	this->m_keymaps = cHandler->GetUIComponent();
	this->m_keymaps->active = 0;
	this->m_keymaps->position = DirectX::XMFLOAT2(200.f, 400.f);
	this->m_keymaps->size = DirectX::XMFLOAT2(800.f, 600.f);
	this->m_keymaps->spriteID = 3;
	this->m_keymaps->scale = 0.5f;
	for (size_t i = 0; i < 2; i++) //Create the options menu buttons
	{
		UIComponent* tempUIComp = cHandler->GetUIComponent();
		tempUIComp->active = 0;
		tempUIComp->position = DirectX::XMFLOAT2(50.f, 50.f + (i * 150.f));
		tempUIComp->size = DirectX::XMFLOAT2(300.f, 100.f);
		tempUIComp->spriteID = 5;
		TextComponent* tempTextComp = cHandler->GetTextComponent();
		tempTextComp->active = 0;
		tempTextComp->position = DirectX::XMFLOAT2(75.f, 70.f + (i * 150.f));
		MenuButton button;
		button.m_uiComp = tempUIComp;
		button.m_textComp = tempTextComp;
		this->m_optionsMenuButtons.push_back(button);
	}
	for (size_t i = 0; i < 3; i++) //Create the start game menu buttons
	{
		UIComponent* tempUIComp = cHandler->GetUIComponent();
		tempUIComp->active = 0;
		tempUIComp->position = DirectX::XMFLOAT2(100.f, 200.f + (i * 150.f));
		tempUIComp->size = DirectX::XMFLOAT2(300.f, 100.f);
		tempUIComp->spriteID = 5;
		TextComponent* tempTextComp = cHandler->GetTextComponent();
		tempTextComp->active = 0;
		tempTextComp->position = DirectX::XMFLOAT2(100.f, 220.f + (i * 150.f));
		MenuButton button;
		button.m_uiComp = tempUIComp;
		button.m_textComp = tempTextComp;
		this->m_startMenuButtons.push_back(button);
	}

	//Init the ip text box
	this->m_ipTextBox.m_uiComp = cHandler->GetUIComponent();
	this->m_ipTextBox.m_uiComp->active = 0;
	this->m_ipTextBox.m_uiComp->position = DirectX::XMFLOAT2(550.f, 200.f + (150.f));
	this->m_ipTextBox.m_uiComp->size = DirectX::XMFLOAT2(300.f, 100.f);
	this->m_ipTextBox.m_uiComp->scale = 0.6;
	this->m_ipTextBox.m_uiComp->spriteID = 5;
	this->m_ipTextBox.m_textComp = cHandler->GetTextComponent();
	this->m_ipTextBox.m_textComp->active = 0;
	this->m_ipTextBox.m_textComp->position = DirectX::XMFLOAT2(575.f, 220.f + (150.f));
	this->m_ipTextBox.m_textComp->scale = DirectX::XMFLOAT2(0.6f, 0.6f);
	this->m_ipTextBox.m_textComp->text = L"Enter IP...";

	this->m_mainMenuButtons[0].m_textComp->text = L"Start Game";
	this->m_mainMenuButtons[2].m_textComp->text = L"Quit Game";
	this->m_mainMenuButtons[1].m_textComp->text = L"Options";

	this->m_optionsMenuButtons[0].m_textComp->text = L"Fullscreen";
	this->m_optionsMenuButtons[1].m_textComp->text = L"Go Back";

	this->m_startMenuButtons[0].m_textComp->text = L"Host Game";
	this->m_startMenuButtons[1].m_textComp->text = L"Join Game";
	this->m_startMenuButtons[2].m_textComp->text = L"Go Back";

	this->m_markedItem = 0;
	this->m_mainMenuButtons[0].SetHovered(true);

	return result;
}

int MenuState::Update(float dt, InputHandler * inputHandler)
{
	int result = 1;

	inputHandler->SetMouseLocked(false);
	DirectX::XMFLOAT2 mousePos = inputHandler->GetMousePos();
	size_t nrOfMainMenuItems = this->m_mainMenuButtons.size();
	size_t nrOfOptionMenuitems = this->m_optionsMenuButtons.size();
	size_t nrOfStartMenuitems = this->m_startMenuButtons.size();
	switch (this->m_menuState)
	{
	case 0: /*/ Main menu /*/
		for (size_t i = 0; i < nrOfMainMenuItems; i++) //Mouse hover
		{
			this->m_mainMenuButtons[i].m_uiComp->UpdateHover(mousePos);
			if (this->m_mainMenuButtons[i].m_uiComp->isHovered)
			{
				this->m_mainMenuButtons[i].SetHovered(true);
				this->m_markedItem = (unsigned int)i;
			}
			else if (i != this->m_markedItem)
			{
				this->m_mainMenuButtons[i].SetHovered(false);
			}
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT)) //Mouse click
		{
			for (size_t i = 0; i < nrOfMainMenuItems; i++)
			{
				this->m_mainMenuButtons[i].m_uiComp->UpdateClicked(mousePos);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_RETURN)) //Select item with keyboard
		{
			this->m_mainMenuButtons[m_markedItem].m_uiComp->wasClicked = true;
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_DOWN)) //Hover down
		{
			if (this->m_markedItem < nrOfMainMenuItems - 1)
			{
				this->m_mainMenuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem++;
				this->m_mainMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_UP)) //Hover up
		{
			if (this->m_markedItem > 0)
			{
				this->m_mainMenuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem--;
				this->m_mainMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}

		if (this->m_mainMenuButtons[0].m_uiComp->CheckClicked())
		{
			//Start game was clicked
			//Switch visable buttons
			this->m_mainMenuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = 0;
			this->m_startMenuButtons[0].SetHovered(true);
			this->m_menuState = 2;
			for (size_t i = 0; i < nrOfMainMenuItems; i++)
			{
				this->m_mainMenuButtons[i].SetActive(false);
			}
			for (size_t i = 0; i < nrOfStartMenuitems; i++)
			{
				this->m_startMenuButtons[i].SetActive(true);
			}
			this->m_ipTextBox.SetActive(true); 
			//Get the saved IP
			Progression::instance().ReadFromFile("Save1");
			std::wstring ipString = Progression::instance().GetIPString();
			if (ipString.length() > 0)
			{
				this->m_ipTextBox.m_textComp->text = ipString;
			}
		}
		else if (this->m_mainMenuButtons[1].m_uiComp->CheckClicked())
		{
			//Options was clicked
			//Switch visable buttons
			this->m_mainMenuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = 0;
			this->m_optionsMenuButtons[0].SetHovered(true);
			this->m_menuState = 1;
			for (size_t i = 0; i < nrOfMainMenuItems; i++)
			{
				this->m_mainMenuButtons[i].SetActive(false);
			}
			for (size_t i = 0; i < nrOfOptionMenuitems; i++)
			{
				this->m_optionsMenuButtons[i].SetActive(true);
			}
			this->m_keymaps->active = 1;
		}
		else if (this->m_mainMenuButtons[2].m_uiComp->CheckClicked())
		{
			//Quit game was clicked
			result = 0;
		}
		break;

	case 1: /*/ Options menu /*/
		for (size_t i = 0; i < nrOfOptionMenuitems; i++) //Mouse hover
		{
			this->m_optionsMenuButtons[i].m_uiComp->UpdateHover(mousePos);
			if (this->m_optionsMenuButtons[i].m_uiComp->isHovered)
			{
				this->m_optionsMenuButtons[i].SetHovered(true);
				this->m_markedItem = (unsigned int)i;
			}
			else if (i != this->m_markedItem)
			{
				this->m_optionsMenuButtons[i].SetHovered(false);
			}
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT))
		{
			for (size_t i = 0; i < nrOfOptionMenuitems; i++)
			{
				this->m_optionsMenuButtons[i].m_uiComp->UpdateClicked(mousePos);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_RETURN) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_ENTER))
		{
				this->m_optionsMenuButtons[m_markedItem].m_uiComp->wasClicked = true;
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			if (this->m_markedItem < nrOfOptionMenuitems - 1)
			{
				this->m_optionsMenuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem++;
				this->m_optionsMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_UP))
		{
			if (this->m_markedItem > 0)
			{
				this->m_optionsMenuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem--;
				this->m_optionsMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}

		if (this->m_optionsMenuButtons[0].m_uiComp->CheckClicked())
		{
			//Toggle fullscreen was clicked
			////Cheating by telling the system the user pressed F
			//inputHandler->SetKeyState(SDL_SCANCODE_F, true); //Seems this does not reset, ever
			result = 511;
			if (!this->inFullscreen)
			{
				this->m_optionsMenuButtons.at(0).m_textComp->text = L"Windowed";
			}
			else
			{
				this->m_optionsMenuButtons.at(0).m_textComp->text = L"Fullscreen";
			}
			this->inFullscreen = !this->inFullscreen;
		}
		else if (this->m_optionsMenuButtons[1].m_uiComp->CheckClicked())
		{
			//Return to main menu was clicked
			//Switch visable buttons
			this->m_optionsMenuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = 0;
			this->m_mainMenuButtons[0].SetHovered(true);
			this->m_menuState = 0;
			for (size_t i = 0; i < nrOfMainMenuItems; i++)
			{
				this->m_mainMenuButtons[i].SetActive(true);
			}
			for (size_t i = 0; i < nrOfOptionMenuitems; i++)
			{
				this->m_optionsMenuButtons[i].SetActive(false);
			}
			this->m_keymaps->active = 0;
		}
		break;

	case 2: /*/ Start game menu /*/
		for (size_t i = 0; i < nrOfStartMenuitems; i++) //Mouse hover
		{
			this->m_startMenuButtons[i].m_uiComp->UpdateHover(mousePos);
			if (this->m_startMenuButtons[i].m_uiComp->isHovered)
			{
				this->m_startMenuButtons[i].SetHovered(true);
				this->m_markedItem = (unsigned int)i;
				this->m_ipTextBox.SetFocused(false);
			}
			else if (i != this->m_markedItem)
			{
				this->m_startMenuButtons[i].SetHovered(false);
			}
		}

		if (inputHandler->IsMouseKeyReleased(SDL_BUTTON_LEFT))
		{
			for (size_t i = 0; i < nrOfStartMenuitems; i++)
			{
				this->m_startMenuButtons[i].m_uiComp->UpdateClicked(mousePos);
			}
			this->m_ipTextBox.m_uiComp->UpdateClicked(mousePos);
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_RETURN) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_ENTER))
		{
			if (!this->m_ipTextBox.m_focused)
			{
				this->m_startMenuButtons[this->m_markedItem].m_uiComp->wasClicked = true;
			}
			else
			{
				this->m_ipTextBox.SetFocused(false);
				this->m_markedItem = 1;
				this->m_startMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			if (this->m_markedItem < nrOfStartMenuitems)
			{
				this->m_startMenuButtons[this->m_markedItem].SetHovered(false);
				this->m_markedItem++;
				if (this->m_markedItem != nrOfStartMenuitems)
				{
					this->m_startMenuButtons[this->m_markedItem].SetHovered(true);
				}
				else
				{
					this->m_ipTextBox.SetFocused(true);
				}
			}
		}
		if (inputHandler->IsKeyPressed(SDL_SCANCODE_UP))
		{
			if (this->m_markedItem > 0)
			{
				if (this->m_markedItem < nrOfStartMenuitems)
				{
					this->m_startMenuButtons[this->m_markedItem].SetHovered(false);
				}
				else
				{
					this->m_ipTextBox.SetFocused(false);
				}
				this->m_markedItem--;
				this->m_startMenuButtons[this->m_markedItem].SetHovered(true);
			}
		}

		if (this->m_startMenuButtons[0].m_uiComp->CheckClicked())
		{
			//Host Game was clicked
		
			if (isHosting == false)
			{
				inputHandler->SetMouseLocked(true);
				printf("Hosting..\n");
				#pragma region
				//Hide buttons
				for (size_t i = 0; i < nrOfStartMenuitems; i++)
				{
					this->m_startMenuButtons[i].SetActive(false);
				}
				this->m_ipTextBox.SetActive(false);
				this->m_menuBG->active = 0;

				#pragma endregion Hide Menu
				
				#pragma region

				if (this->m_networkModule == nullptr)	//If the networkModule isa not initialized
				{
					this->m_networkModule = new NetworkModule();	//Create a new networkModule
					int result = this->m_networkModule->Initialize();	// Try to init the networkModule

					if (result != 1)	//If failed
					{
						this->m_networkModule = nullptr;	//Set the module pointer to nullptr
					}
				}

				#pragma endregion Network_INIT


				this->isHosting = true;
			}

		}
		else if (this->m_startMenuButtons[1].m_uiComp->CheckClicked())
		{
			//Join Game was clicked

			//Hide buttons
			for (size_t i = 0; i < nrOfStartMenuitems; i++)
			{
				this->m_startMenuButtons[i].SetActive(false);
			}
			this->m_ipTextBox.SetActive(false);
			this->m_menuBG->active = 0;

			//Update the IP stored in Progression
			if (!this->m_ipTextBox.firstChar)
			{
				Progression::instance().SetIPString(this->m_ipTextBox.m_textComp->text);
				Progression::instance().WriteToFile("Save1");
			}

			if (this->isJoining == false)
			{
				int result;
				#pragma region

				if (this->m_networkModule == nullptr)	//If the networkModule isa not initialized
				{
					this->m_networkModule = new NetworkModule();	//Create a new networkModule
					 result = this->m_networkModule->Initialize();	// Try to init the networkModule

					if (result != 1)	//If failed
					{
						this->m_networkModule = nullptr;	//Set the module pointer to nullptr
					}
				}

				#pragma endregion Network_INIT
				
				#pragma region
				if (this->m_networkModule != nullptr)
				{
					char* ip = new char[255];
					sprintf(ip, "%ls", Progression::instance().GetIPString().c_str());

					int result = this->m_networkModule->Join(ip);

					if (result <= 0)	//If failed to connect
					{
						//Shut down the networkModule
						this->m_networkModule->Shutdown();
						delete this->m_networkModule;
						this->m_networkModule = nullptr;

						this->isJoining = false;

						//Show buttons
						for (size_t i = 0; i < this->m_startMenuButtons.size(); i++)
						{
							this->m_startMenuButtons[i].SetActive(true);
						}
						this->m_ipTextBox.SetActive(true);
					}
					else //If succeded to connect
					{
						this->isJoining = true;
					}
				}

				#pragma endregion Try to connect
			}

		}
		else if (this->m_startMenuButtons[2].m_uiComp->CheckClicked())
		{
			//Go Back was clicked
			//Switch visable buttons
			this->m_startMenuButtons[this->m_markedItem].SetHovered(false);
			this->m_markedItem = 0;
			this->m_mainMenuButtons[0].SetHovered(true);
			this->m_menuState = 0;
			for (size_t i = 0; i < nrOfMainMenuItems; i++)
			{
				this->m_mainMenuButtons[i].SetActive(true);
			}
			for (size_t i = 0; i < nrOfStartMenuitems; i++)
			{
				this->m_startMenuButtons[i].SetActive(false);
			}
			this->m_ipTextBox.SetActive(false);
		}
		else if (this->m_ipTextBox.m_uiComp->CheckClicked())
		{
			//IP text box was clicked
			this->m_startMenuButtons[this->m_markedItem].SetHovered(false);
			this->m_ipTextBox.SetFocused(true);
		}

		if (this->m_ipTextBox.m_focused)
		{
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_BACKSPACE))
			{
				this->m_ipTextBox.RemoveChar();
				if (this->m_ipTextBox.firstChar)
				{
					this->m_ipTextBox.m_textComp->text = L"Enter IP...";
				}
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_1) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_1))
			{
				this->m_ipTextBox.AddChar(L"1");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_2) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_2))
			{
				this->m_ipTextBox.AddChar(L"2");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_3) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_3))
			{
				this->m_ipTextBox.AddChar(L"3");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_4) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_4))
			{
				this->m_ipTextBox.AddChar(L"4");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_5) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_5))
			{
				this->m_ipTextBox.AddChar(L"5");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_6) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_6))
			{
				this->m_ipTextBox.AddChar(L"6");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_7) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_7))
			{
				this->m_ipTextBox.AddChar(L"7");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_8) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_8))
			{
				this->m_ipTextBox.AddChar(L"8");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_9) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_9))
			{
				this->m_ipTextBox.AddChar(L"9");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_0) || inputHandler->IsKeyPressed(SDL_SCANCODE_KP_0))
			{
				this->m_ipTextBox.AddChar(L"0");
			}
			if (inputHandler->IsKeyPressed(SDL_SCANCODE_PERIOD))
			{
				this->m_ipTextBox.AddChar(L".");
			}
		}
		
		if (isHosting)	//If we have pressed the host button
		{
#ifndef HOST_DISABLE
			this->Hosting(dt, inputHandler);	//Do the Host update
#else
			//Create, Initialize and push a LevelSelectState
			LevelSelectState* levelSelect = new LevelSelectState();
			result = levelSelect->Initialize(this->m_gsh, this->m_cHandler, this->m_cameraRef);

			//If the initialization was successful
			if (result > 0)
			{
				//Push it to the gamestate stack/vector
				//this->PushStateToStack(levelSelect);

				levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));

				//Delete it. If it was successful it would have pushed a LevelState to the stack
				delete levelSelect;
				levelSelect = nullptr;
			}
			else
			{
				delete levelSelect;
				levelSelect = nullptr;
			}
			this->isHosting = false;
#endif
		}

		if (isJoining)
		{
			this->Joining(inputHandler);
		}

		break;
	default:
		break;
	}
	
	if (this->m_networkModule)
	{
		this->m_networkModule->Update();
	}

	return result;
}

int MenuState::EnterState()
{
	//Show buttons
	size_t nrOfStartMenuitems = this->m_startMenuButtons.size();
	for (size_t i = 0; i < nrOfStartMenuitems; i++)
	{
		this->m_startMenuButtons[i].SetActive(true);
	}
	this->m_ipTextBox.SetActive(true);
	this->m_menuBG->active = 1;

	this->isHosting = false;
	this->isJoining = false;

	return 0;
}

int MenuState::LeaveState()
{
	return 0;
}

void MenuState::Hosting(float dt, InputHandler* inputHandler)
{
	#pragma region

	//Wait for someone to connect
	if (this->m_networkModule->GetNrOfConnectedClients() <= 0)	//If noone has connected
	{
		this->timeoutTime += dt / 1000000;	//Increase the timeout timer

		if (this->timeoutTime > TIMEOUT_TIME)	//If the timer is to high
		{
			//Shut down the networkModule
			this->m_networkModule->Shutdown();
			delete this->m_networkModule;
			this->m_networkModule = nullptr;

			this->timeoutTime = 0;
			this->isHosting = false;
			
			//Show buttons
			for (size_t i = 0; i < this->m_startMenuButtons.size(); i++)
			{
				this->m_startMenuButtons[i].SetActive(true);
			}
			this->m_ipTextBox.SetActive(true);

			printf("Timed-out becuase no one connected\n");
		}
	}
	else
	{
		/*
		Now when someone has connected we want to send a physics sync packet
		before we continue. When we recive the ready packet we will load our level.
		*/
		unsigned int	startIndex = 0;
		unsigned int	nrOfDynamics = 0;
		bool			isHost = false;
		unsigned int	levelID = 1;	//CHANGE LEVEL HERE NOW
		unsigned int	checkpointID = 0;

		if (this->sentSyncPacket == false)	//If we havent already sent the sync packet
		{
			this->m_networkModule->SendPhysicSyncPacket(startIndex, nrOfDynamics, isHost, levelID, checkpointID);
			this->sentSyncPacket = true;
			printf("Sent SyncPhysicsPacket to client\n");
		}

		if (this->m_networkModule->IsClientReady())	//When we recive the readyPacket from the client we start the level
		{
			printf("Client is Ready");
			#pragma region
			//Create, Initialize and push a LevelSelectState
			LevelSelectState* levelSelect = new LevelSelectState();
			int result = levelSelect->Initialize(this->m_gsh, this->m_cHandler, this->m_cameraRef);

			//If the initialization was successful
			if (result > 0)
			{
				//Push it to the gamestate stack/vector
				//this->m_gsh->PushStateToStack(levelSelect);

				#pragma region
				switch (levelID)
				{
				case 1:
					printf("LOAD LEVEL 1\n");
					levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));
					break;

				default:
					printf("LOAD DEFUALT\n");
					levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));
					break;

				}
				#pragma endregion Level_To_Load
			}
			//Delete it
			delete levelSelect;
			levelSelect = nullptr;
			#pragma endregion Load_Level

			inputHandler->SetMouseLocked(true);	//Lock the mouse again
		}
	}

#pragma endregion Network_Sync
}

void MenuState::Joining(InputHandler* inputHandler)
{
	#pragma region
	//Listen for syncPacket
	std::list<SyncPhysicPacket> packets = this->m_networkModule->PacketBuffer_GetPhysicPacket();
		
	if (packets.size() != 0)	//We recive the syncpacket
	{
		printf("Recived SyncPhysicsPacket");
		std::list<SyncPhysicPacket>::iterator packet = packets.begin();

		//Send Ready packet
		this->m_networkModule->SendFlagPacket(PacketTypes::SYNC_READY);

		//Load level with correct values
		unsigned int	startIndex = packet->startIndex;
		unsigned int	nrOfDynamics = packet->nrOfDynamics;
		bool			isHost = packet->isHost;
		unsigned int	levelID = packet->levelID;
		unsigned int	checkpointID = packet->checkpointID;

		#pragma region
		//Create, Initialize and push a LevelSelectState
		LevelSelectState* levelSelect = new LevelSelectState();
		int result = levelSelect->Initialize(this->m_gsh, this->m_cHandler, this->m_cameraRef);

		//If the initialization was successful
		if (result > 0)
		{
			//Push it to the gamestate stack/vector
			//this->m_gsh->PushStateToStack(levelSelect);

			#pragma region
			switch (levelID)
			{
			case 1:
				printf("LOAD LEVEL 1\n");
				levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));
				break;

			default:
				printf("LOAD DEFUALT\n");
				levelSelect->LoadLevel(std::string("../ResourceLib/AssetFiles/L1P1.level"));
				break;

			}
			#pragma endregion Level_To_Load

			inputHandler->SetMouseLocked(true);	//Lock the mouse again
		}

		//Delete it
		delete levelSelect;
		levelSelect = nullptr;
		#pragma endregion Load_Level

		this->isJoining = false;
	}

#pragma endregion Network_Sync
}
