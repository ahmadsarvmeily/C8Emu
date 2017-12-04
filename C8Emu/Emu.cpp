#include "Emu.h"
#include <mutex>

Emu::Emu() :
	chip8(kbd),
	renderThread(&Emu::Render, this),
	isRunning(false),
	windowReady(false)
{
	renderThread.launch();
}

void Emu::Render()
{
	window = new sf::RenderWindow(sf::VideoMode(1280, 640, 1), "CHIP-8 Emulator");
	window->clear(sf::Color::Black);
	window->setFramerateLimit(60);

	windowReady = true;

	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
		}
		
		if (isRunning && chip8.drawFlag) {
			ProcessChip8Pixels();
		}
		
		window->display();
	}
}

void Emu::ProcessChip8Pixels()
{	
	window->clear(sf::Color::Black);
		
	for (int y = 0; y < 32; y++) {
		for (int x = 0; x < 64; x++) {
			if (chip8.GetPixel(x, y)) {
				pixel.setPosition(sf::Vector2f(x * 20.0f, y * 20.0f));
				window->draw(pixel);
			}
		}
	}
	chip8.drawFlag = false;
}

void Emu::Run(std::string gamePath)
{
	chip8.LoadGame(gamePath);

	isRunning = true;

	while (!windowReady);

	while (window->isOpen()) {
		chip8.Update(ft.Mark());
		chip8.SetKeys();
	}
	isRunning = false;
}
