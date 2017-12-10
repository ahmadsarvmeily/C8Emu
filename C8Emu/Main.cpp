#include "Emu.h"

int main() {

	Emu c8Emu(640,320,60,true);
	c8Emu.Run("c8games/PONG2");

	return 0;
}