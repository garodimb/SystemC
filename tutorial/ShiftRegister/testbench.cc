#include <iostream>
#include <testbench.h>

template
<int T>
Testbench<T>::Testbench(sc_core::sc_module_name _name) : sc_core::sc_module(_name)
{
	SC_THREAD(test);

	SC_METHOD(monitor);
	dont_initialize();
	sensitive << r;
}

template
<int T>
void Testbench<T>::test()
{
	wait(15, sc_core::SC_NS);

	for(int i = 0; i < 1000; i++){
		din.write(rand()%2);
		wait(10, sc_core::SC_NS);
	}

	sc_core::sc_stop();
}

template
<int T>
void Testbench<T>::monitor()
{
	std::cout << "[ " << sc_core::sc_time_stamp() << " ] " << r.read().to_string(sc_dt::SC_BIN) << std::endl;
}