#include "Emu.h"
#include <mutex>

Emu::Emu(int frameWidth, int frameHeight) :
	frameWidth(frameWidth),
	frameHeight(frameHeight),
	pixelWidth(frameWidth/64.0f),
	pixelHeight(frameHeight/32.0f),
	chip8(kbd),
	renderThread(&Emu::Render, this),
	isRunning(false),
	windowReady(false)
{
	renderThread.launch();
}

void Emu::Render()
{
	window = std::make_unique<sf::RenderWindow>(sf::VideoMode(frameWidth, frameHeight, 1), "CHIP-8 Emulator");
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
				pixel.setPosition(sf::Vector2f(x * pixelWidth, y * pixelHeight));
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
