#include <systemc>
#include <iostream>
#include "memory.h"
#include "testbench.h"

int sc_main(int argc, char *argv[])
{
	// Instantiate memory and testbench
	Testbench tb("tb");
	Memory mem("mem");

	// Instantiate signals
	sc_signal<bool> read_en, write_en, reset_n;
	sc_signal< sc_uint<32> > addr_bus;
	sc_signal< sc_uint<32>, SC_MANY_WRITERS> data_bus;

	// Bins signals to corresponding ports
	mem.read_en(read_en);
	tb.read_en(read_en);

	mem.write_en(write_en);
	tb.write_en(write_en);

	mem.reset_n(reset_n);
	tb.reset_n(reset_n);

	mem.addr_bus(addr_bus);
	tb.addr_bus(addr_bus);

	mem.data_bus(data_bus);
	tb.data_bus(data_bus);

	sc_start();

	return 0;
}