#include <shift_reg.h>


template
<int T>
ShiftRegister<T>::ShiftRegister(sc_core::sc_module_name _name) : sc_core::sc_module(_name)
{
	shift_delay = sc_core::sc_time(2, sc_core::SC_NS);
	
	SC_METHOD(shift);
	sensitive << clk.pos();

	SC_METHOD(r_out);
	dont_initialize();
	sensitive << r_event;
}

template
<int T>
void ShiftRegister<T>::shift()
{
	t_r = r;
	for(int i = T-1 ; i > 0 ; i--){
		t_r[i] = r[i-1];
	}
	t_r[0] = din;
	r_event.notify(shift_delay);
}

template
<int T>
void ShiftRegister<T>::r_out()
{
	r.write(t_r);
}