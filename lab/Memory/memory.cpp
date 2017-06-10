#include <memory.h>

Memory::Memory(sc_module_name _name): sc_module(_name)
{
	mem = new sc_uint<DATA_WIDTH>[SIZE];
	
	// Read method
	SC_METHOD(read);
	dont_initialize();
	sensitive << read_en.pos();

	// Write method
	SC_METHOD(write);
	dont_initialize();
	sensitive << write_en.pos();

	// Reset method
	SC_METHOD(do_reset);
	dont_initialize();
	sensitive << reset_n.pos();
}

Memory::~Memory()
{
	delete[] mem;
}

void Memory::read()
{	
	// Read to memory
	data_bus.write(mem[addr_bus.read()]);
	
	#ifdef DEBUG_M
			cout << "[READ_M][ " << sc_time_stamp() << " ] MEM[ " << addr_bus.read()
				 << " ] : " << mem[addr_bus.read()] << endl;
	#endif
	
	return;
}


void Memory::write()
{
	// Write to memory
	mem[addr_bus.read()] = data_bus.read();
	
	#ifdef DEBUG_M
			cout << "[WRITE_M][ " << sc_time_stamp() << " ] MEM[ " << addr_bus.read()
				 << " ] : " << mem[addr_bus.read()] << endl;
	#endif

	return;
}

void Memory::do_reset()
{
	// Reset memory model
	#ifdef DEBUG_M
		cout << "[RESET_M][ " << sc_time_stamp() << " ]" << endl;
	#endif

	data_bus.write(0);
	delete[] mem;
	mem = new sc_uint<DATA_WIDTH>[SIZE];
	return;
}