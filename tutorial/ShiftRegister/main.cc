#include <testbench.h>
#include <shift_reg.h>


int sc_main(int argc, char *argv[])
{

	ShiftRegister<4> reg("reg");
	Testbench<4> tb("tb");

	sc_core::sc_signal<bool> din;
	sc_core::sc_signal<sc_dt::sc_int<4> > r;

	sc_core::sc_clock clk("clk",10, sc_core::SC_NS, 0.5);
	
	reg.clk(clk);
	tb.clk(clk);

	reg.din(din);
	tb.din(din);

	reg.r(r);
	tb.r(r);

	sc_core::sc_start();
	return 0;
}