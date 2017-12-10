#include "Emu.h"
#include <mutex>
#include <iostream>
#include <algorithm>

Emu::Emu(int frameWidth, int frameHeight, int maxFrameRate, bool showFPS) :
	frameWidth(frameWidth),
	frameHeight(frameHeight),
	pixelWidth(frameWidth/64.0f),
	pixelHeight(frameHeight/32.0f),
	chip8(kbd),
	renderThread(&Emu::Render, this),
	isRunning(false),
	windowReady(false),
	maxFrameRate(maxFrameRate),
	frameInterval(1000.0f/maxFrameRate),
	showFPS(showFPS)
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
	window->display();
	chip8.drawFlag = false;
}

void Emu::Run(std::string gamePath)
{
	chip8.LoadGame(gamePath);

	while (!windowReady);

	size_t pos = gamePath.find_last_of("/") + 1;
	gameTitle = gamePath.substr(pos, gamePath.size());
	window->setTitle(gameTitle + " | CHIP-8 Emulator");

	isRunning = true;
	while (window->isOpen()) {

		Update(ft.Mark());
		chip8.EmulateCycle();
		chip8.SetKeys();
	}
	isRunning = false;
}

void Emu::Update(float dt)
{
	if (showFPS) {
		float frameTime = max(frameInterval, dt);
		fpsTimer.Push(1000.0f / frameTime);

		double fpsAvg2dp = floor(fpsTimer.Average() * 100.0 + 0.5) / 100.0;

		size_t length = std::to_string(maxFrameRate).length();
		std::string fpsStr = " | " + std::to_string(fpsAvg2dp).substr(0, length + 3) + " FPS";
		window->setTitle(gameTitle + " | CHIP-8 Emulator"+fpsStr);
	}
	
	if (dt < frameInterval) {
		std::this_thread::sleep_for(std::chrono::milliseconds((long long)(frameInterval-dt)));
	}
}

