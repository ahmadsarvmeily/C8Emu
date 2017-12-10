#include "Chip8.h"
#include <iostream>

Chip8::Chip8(HexKbd & kbd) :
	kbd(kbd),
	pc(0x200),
	memory({}),
	gfx({}),
	V({}),
	key({}),
	sp(0),
	indexRegister(0),
	drawFlag(true),
	soundFlag(false)
	{

	for (int i = 0; i < 80; ++i)
		memory[i] = fontset[i];

	std::random_device rand;
	rng = std::mt19937(rand());
	dist = std::uniform_int_distribution<>(0, 255);
}

void Chip8::LoadGame(std::string gamePath) {

	std::ifstream in(gamePath, std::ios_base::binary);

	for (int i = 0x200; in.good(); i++) {
		in.read(reinterpret_cast<char*>(&memory[i]), 1);
	}

	in.close();
}

void Chip8::EmulateCycle() {
	opcode = memory[pc] << 8 | memory[pc + 1];

	switch (opcode & 0xF000) {

	case 0x0000:
		switch (opcode & 0x0FFF) {
		case 0x00E0:
			gfx = {};
			drawFlag = true;
			pc += 2;
			break;

		case 0x00EE:
			sp--;
			pc = stack[sp];
			pc += 2;
			break;
		}
		break;

	case 0x1000:
		pc = opcode & 0x0FFF;
		break;

	case 0x2000:
		stack[sp] = pc;
		sp++;
		pc = opcode & 0x0FFF;
		break;

	case 0x3000:
		if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
			pc += 2;
		pc += 2;
		break;

	case 0x4000:
		if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
			pc += 2;
		pc += 2;
		break;

	case 0x5000:
		if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
			pc += 2;
		pc += 2;
		break;

	case 0x6000:
		V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		pc += 2;
		break;

	case 0x7000:
		V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		pc += 2;
		break;

	case 0x8000:
		switch (opcode & 0x000F) {
		case 0x0000:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0001:
			V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0002:
			V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0003:
			V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0004:
			if (V[(opcode & 0x0F00) >> 8] > 0xFF - V[(opcode & 0x00F0) >> 4])
				V[0xF] = 1;
			else
				V[0xF] = 0;

			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		case 0x0005: {
			std::uint8_t x = (opcode & 0x0F00) >> 8;
			std::uint8_t y = (opcode & 0x00F0) >> 4;
			if (V[y] > V[x])
				V[0xF] = 0;
			else
				V[0xF] = 1;

			V[x] -= V[y];

			pc += 2;
		}
			break;

		case 0x0006: {
			std::uint8_t y = (opcode & 0x00F0) >> 4;
			V[0xF] = V[y] & 0x1;
			V[(opcode & 0x0F00) >> 8] = V[y] >> 1;

			pc += 2;
		}
			break;

		case 0x0007: {
			std::uint8_t x = (opcode & 0x0F00) >> 8;
			std::uint8_t y = (opcode & 0x00F0) >> 4;
			if (V[x] > V[y])
				V[0xF] = 0;
			else
				V[0xF] = 1;

			V[x] = V[y] - V[x];

			pc += 2;
		}
			break;

		case 0x000E: {
			std::uint8_t y = (opcode & 0x00F0) >> 4;
			V[0xF] = V[y] >> 7;
			V[(opcode & 0x0F00) >> 8] = V[y] << 1;
			pc += 2;
		}
			break;
		}
		break;

	case 0x9000:
		if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
			pc += 2;
		pc += 2;
		break;

	case 0xA000:
		indexRegister = opcode & 0x0FFF;
		pc += 2;
		break;

	case 0xB000:
		pc = (opcode & 0x0FFF) + V[0];
		break;

	case 0xC000:
		V[(opcode & 0x0F00) >> 8] = dist(rng) & (opcode & 0x00FF);
		pc += 2;
		break;

	case 0xD000: {
		std::uint8_t x = (opcode & 0x0F00) >> 8;
		std::uint8_t y = (opcode & 0x00F0) >> 4;
		std::uint8_t height = (opcode & 0x000F);
		V[0xF] = 0;
		for (int yOff = 0; yOff < height; yOff++) {

			std::uint8_t pixelRow = memory[indexRegister + yOff];

			for (int xOff = 0; xOff < 8; xOff++) {

				if ((pixelRow & (0x80 >> xOff)) != 0) {

					if (gfx[V[x] + xOff + (V[y] + yOff) * 64] == 1)
						V[0xF] = 1;

					gfx[V[x] + xOff + (V[y] + yOff) * 64] ^= 1;
				}
			}
		}

		drawFlag = true;
		pc += 2;
	}
	break;

	case 0xE000:
		switch (opcode & 0x00FF) {
		case 0x009E:
			if (key[V[(opcode & 0x0F00) >> 8]] != 0)
				pc += 2;
			pc += 2;
			break;

		case 0x00A1:
			if (key[V[(opcode & 0x0F00) >> 8]] == 0)
				pc += 2;
			pc += 2;
			break;
		}
		break;

	case 0xF000:
		switch (opcode & 0x00FF) {

		case 0x0007:
			V[(opcode & 0x0F00) >> 8] = delayTimer;
			pc += 2;
			break;

		case 0x000A:
			V[(opcode & 0x0F00) >> 8] = kbd.GetKeypress();
			pc += 2;
			break;

		case 0x0015:
			delayTimer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x0018:
			soundTimer = V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x001E:
			if (indexRegister + V[(opcode & 0x0F00) >> 8] > 0x0FFF)
				V[0xF] = 1;
			else
				V[0xF] = 0;
			indexRegister += V[(opcode & 0x0F00) >> 8];
			pc += 2;
			break;

		case 0x0029:
			indexRegister = V[(opcode & 0x0F00) >> 8] * 0x5;
			pc += 2;
			break;

		case 0x0033: {
			std::uint8_t x = (opcode & 0x0F00) >> 8;
			memory[indexRegister] = V[x] / 100;
			memory[indexRegister + 1] = (V[x] / 10) % 10;
			memory[indexRegister + 2] = (V[x] % 100) % 10;
			pc += 2;
		}
		break;

		case 0x0055: {
			std::uint8_t x = (opcode & 0x0F00) >> 8;
			for (int i = 0; i <= x; i++)
				memory[indexRegister + i] = V[i];
			indexRegister += x + 1;
			pc += 2;
		}
			break;

		case 0x0065: {
			std::uint8_t x = (opcode & 0x0F00) >> 8;
			for (int i = 0; i <= x; i++)
				V[i] = memory[indexRegister + i];
			indexRegister += x + 1;
			pc += 2;
		}
			break;
		}
		break;
	}

	if (delayTimer > 0)
		delayTimer--;

	if (soundTimer > 0) {
		soundFlag = true;
		soundTimer--;
	}
}

void Chip8::SetKeys() {
	for (int i = 0; i < 16; i++)
		key[i] = kbd.KeyIsPressed(i);
}

bool Chip8::DrawFlag() const {
	return drawFlag;
}

std::uint8_t Chip8::GetPixel(int x, int y) {
	return gfx[x + y * 64];
}
