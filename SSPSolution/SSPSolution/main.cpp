//#include <SFML\Window.hpp>
#include <SDL.h>

int main(int argc, char** argv)
{
	bool fullscreen = false;
	SDL_Surface* displayWindow = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		int i = 0;
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