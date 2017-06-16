#ifndef SHIFTREG_H
#define SHIFTREG_H

#include <systemc>

template
<int T>
struct ShiftRegister : sc_core::sc_module
{
	private:
		sc_dt::sc_int<T> reg;
		sc_dt::sc_int<T> t_r;
		sc_core::sc_time shift_delay;
		sc_core::sc_event r_event;

	public:
		SC_HAS_PROCESS(ShiftRegister);
		sc_core::sc_in<bool> clk;
		sc_core::sc_in<bool> din;
		sc_core::sc_out<sc_dt::sc_int<T> > r;
		ShiftRegister(sc_core::sc_module_name _name);
		void shift();
		void r_out();
};

#endif