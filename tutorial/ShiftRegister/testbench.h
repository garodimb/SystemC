#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <systemc>
template
<int T>
struct Testbench : sc_core::sc_module
{

	public:
		sc_core::sc_out<bool> din;
		sc_core::sc_in<bool> clk;
		sc_core::sc_in<sc_dt::sc_int<T> > r;
		SC_HAS_PROCESS(Testbench);
		Testbench(sc_core::sc_module_name _name);
		void test();
		void monitor();
};

#endif