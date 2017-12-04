#include "Emu.h"

int main() {

	Emu c8Emu(1280,640);
	c8Emu.Run("c8games/PONG2");

	return 0;
}