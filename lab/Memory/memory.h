#ifndef MEMORY_H
#define MEMORY_H
#include <systemc>
using namespace sc_core;
using namespace sc_dt;
using namespace std;
#define ADDR_WIDTH 32 // Address width in bits
#define DATA_WIDTH 32 // Data bus width in bits
#define SIZE 256 // Total memory size DATA_WIDTH * SIZE bytes

//#define DEBUG_M 1

struct Memory : sc_module
{
	
	private:
		sc_uint<DATA_WIDTH> *mem;
		void read(); // Read from memory
		void write(); // Write to memory
		void do_reset(); // Do reset

	public:
		sc_in<bool> read_en, write_en, reset_n;
		sc_in< sc_uint<ADDR_WIDTH> > addr_bus;
		sc_inout< sc_uint<DATA_WIDTH> > data_bus;
		
		Memory(sc_module_name _name);
		virtual ~Memory();

	SC_HAS_PROCESS(Memory);
};
#endif