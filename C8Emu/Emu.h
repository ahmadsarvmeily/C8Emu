#pragma once
#include "HexKbd.h"
#include "Chip8.h"
#include "FrameTimer.h"
#include <SFML\Graphics.hpp>
#include <string>
#include <memory>

class Emu {

public:

	Emu(int frameWidth, int frameHeight);

	void Render();
	void ProcessChip8Pixels();
	void Run(std::string gamePath);

private:
	int frameWidth, frameHeight;
	float pixelWidth, pixelHeight;
	HexKbd kbd;
	Chip8 chip8;
	FrameTimer ft;
	std::unique_ptr<sf::RenderWindow> window = nullptr;
	sf::Thread renderThread;
	sf::RectangleShape pixel = sf::RectangleShape(sf::Vector2f(pixelWidth, pixelHeight));
	volatile bool isRunning;
	volatile bool windowReady;
};