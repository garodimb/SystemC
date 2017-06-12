#include <testbench.h>
#include <memory.h>

int sc_main(int argc, char *argv[])
{
	// Instantiate memory and testbench
	Memory memory("Memory");
	Testbench tb("tb");

	// Bind memory and testbench using socket
	tb.socket.bind(memory.socket);

	sc_start();
	
	return 0;
}