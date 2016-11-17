//#include <SFML\Window.hpp>
#include "System.h"
int main(int argc, char** argv)
{
	System* mySystem = new System();
	mySystem->Initialize();
	mySystem->Run();
	delete mySystem;
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