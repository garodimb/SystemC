#ifndef TESTBENCH_H
#define TESTBENCH_H
#include <systemc>

#define BUFF_SIZE 256 // Total Size = BUFF_SIZE * DATA_WIDTH
#define DATA_WIDTH 32 // Data width in bits
#define ADDR_WIDTH 32 // In bits
using namespace std;
using namespace sc_dt;
using namespace sc_core;

//#define DEBUG_T 1

struct Testbench : sc_module
{
	private:
		sc_uint<DATA_WIDTH> *read_buff, *write_buff;
		sc_time r_dly;
		sc_time w_dly;
		void read(); // Read from memory
		void write(); // Write to memory
		void read_write(); // Do read and write operation
		bool compare(const sc_uint<32> *read_buff, const sc_uint<32> *write_buff, size_t len);

	public:
		sc_out<bool> read_en, write_en, reset_n;
		sc_out< sc_uint<ADDR_WIDTH> > addr_bus;
		sc_inout< sc_uint<DATA_WIDTH> > data_bus;

		Testbench(sc_module_name _name);
		virtual ~Testbench();
		
		SC_HAS_PROCESS(Testbench);
		
};

#endif