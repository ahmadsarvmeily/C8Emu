#pragma once
#include <fstream>
#include <string>
#include <random>
#include <array>
#include <cstdint>
#include "HexKbd.h"

class Chip8 {

public:
	Chip8(HexKbd& kbd);

	void LoadGame(std::string gamePath);
	void Update(float dt);
	void EmulateCycle();
	void SetKeys();
	bool DrawFlag() const;
	std::uint8_t GetPixel(int x, int y);
	bool drawFlag;
private:
	std::uniform_int_distribution<int> dist;
	std::mt19937 rng;

	HexKbd& kbd;

	float frameTime = 0.0f;

	std::uint16_t opcode;
	std::uint16_t indexRegister;
	std::uint16_t pc;

	std::array<std::uint8_t,4096> memory;
	std::array<std::uint8_t,16> V;

	std::array<std::uint8_t,64*32> gfx;
	
	std::uint8_t delayTimer, soundTimer;

	std::array<std::uint16_t,16> stack;
	std::uint16_t sp;
	std::array<std::uint8_t,16> key;

	static constexpr std::array<std::uint8_t,80> fontset =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
};