#pragma once
#include "HexKbd.h"
#include "Chip8.h"
#include "FrameTimer.h"
#include <SFML\Graphics.hpp>
#include <string>
#include <memory>
#include "FPSTimer.h"

class Emu {

public:

	Emu(int frameWidth, int frameHeight, int maxFrameRate, bool showFPS);

	void Render();
	void ProcessChip8Pixels();
	void Run(std::string gamePath);
	void Update(float dt);

private:
	const int maxFrameRate;
	const float frameInterval;
	bool showFPS;
	int frameWidth, frameHeight;
	float pixelWidth, pixelHeight;
	HexKbd kbd;
	Chip8 chip8;
	FrameTimer ft;
	FPSTimer fpsTimer;
	std::string gameTitle;
	std::unique_ptr<sf::RenderWindow> window = nullptr;
	sf::Thread renderThread;
	sf::RectangleShape pixel = sf::RectangleShape(sf::Vector2f(pixelWidth, pixelHeight));
	volatile bool isRunning;
	volatile bool windowReady;
};