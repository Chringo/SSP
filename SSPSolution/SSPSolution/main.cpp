//#include <SFML\Window.hpp>
#include <SDL.h>
#include <iostream>
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
int main(int argc, char** argv)
{
	bool fullscreen = false;

	//This is the window we render to
	SDL_Window* displayWindow = NULL;
	//This is the surface contained by the window
	SDL_Surface* displaySurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL failed in initializing the window! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		printf("SDL succeeded in initializing the window!\n");
	}
	
	displayWindow = SDL_CreateWindow("SSD Application", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (displayWindow == NULL)
	{
		printf("Window creation failed! SDL_ERROR: %S\n", SDL_GetError());
	}
	else
	{
		printf("Window creation succeeded!\n");

		displaySurface = SDL_GetWindowSurface(displayWindow);
		SDL_FillRect(displaySurface, NULL, SDL_MapRGB(displaySurface->format, 0xFF, 0xFF, 0xFF));
	}

	bool running = true;
	SDL_Event m_event;
	while (running)
	{
		while (SDL_PollEvent(&m_event))
		{
			
		}
	}

	SDL_Quit();
	delete displayWindow;
	return 0;
}


//sf::Window window(sf::VideoMode(800, 600), "SFML works!");

/*while (window.isOpen())
{
sf::Event event;
while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();

if (event.type == sf::Event::KeyPressed) {
if (event.key.code == sf::Keyboard::F) {
window.close();
if (fullscreen) {
window.create(sf::VideoMode(800, 600), "SFML works!");
fullscreen = false;
}
else {
window.create(sf::VideoMode(800, 600), "SFML works!", sf::Style::Fullscreen);
fullscreen = true;
}
}
else if (event.key.code == sf::Keyboard::Escape) {
window.close();
}
}
}

window.display();
}*/