#pragma once
#include <conio.h>
#include <Windows.h>
#include <cstdint>

class HexKbd {

public:

	std::uint8_t GetKeypress() {
		for (;;) {
			int key = _getch();
			switch (key) {
			case '1': return 0x1;
			case '2': return 0x2;
			case '3': return 0x3;
			case '4': return 0xC;
			case 'q': return 0x4;
			case 'w': return 0x5;
			case 'e': return 0x6;
			case 'r': return 0xD;
			case 'a': return 0x7;
			case 's': return 0x8;
			case 'd': return 0x9;
			case 'f': return 0xE;
			case 'z': return 0xA;
			case 'x': return 0x0;
			case 'c': return 0xB;
			case 'v': return 0xF;
			}
		}
	}

	bool KeyIsPressed(std::uint8_t key) {
		switch (key) {
		case 0x1: return GetKeyState(0x31) & 0x8000; // 1
		case 0x2: return GetKeyState(0x32) & 0x8000; // 2
		case 0x3: return GetKeyState(0x33) & 0x8000; // 3
		case 0xC: return GetKeyState(0x34) & 0x8000; // 4
		case 0x4: return GetKeyState(0x51) & 0x8000; // q
		case 0x5: return GetKeyState(0x57) & 0x8000; // w
		case 0x6: return GetKeyState(0x45) & 0x8000; // e
		case 0xD: return GetKeyState(0x52) & 0x8000; // r
		case 0x7: return GetKeyState(0x41) & 0x8000; // a
		case 0x8: return GetKeyState(0x53) & 0x8000; // s
		case 0x9: return GetKeyState(0x44) & 0x8000; // d
		case 0xE: return GetKeyState(0x46) & 0x8000; // f
		case 0xA: return GetKeyState(0x5A) & 0x8000; // z
		case 0x0: return GetKeyState(0x58) & 0x8000; // x
		case 0xB: return GetKeyState(0x43) & 0x8000; // c
		case 0xF: return GetKeyState(0x56) & 0x8000; // v
		}
		return false;
	}
};