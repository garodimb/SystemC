#include <iostream>
#include <systemc.h>
#include <counter.cpp>

int sc_main(int argc, char *argv[])
{
	sc_signal<bool> clock;
	sc_signal<bool> enable;
	sc_signal<bool> reset;
	sc_signal< sc_uint<4> > counter_out;
	int i = 0;

	counter inst_counter("Counter");
	inst_counter.clock(clock);
	inst_counter.enable(enable);
	inst_counter.reset(reset);
	inst_counter.counter_out(counter_out);

	sc_start(10,SC_NS);

	sc_trace_file *wf = sc_create_vcd_trace_file("counter");
	sc_trace(wf, clock, "clock");
	sc_trace(wf, enable, "enable");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, counter_out, "counter_out");

	reset = 0;
	enable = 0;
	for(i = 0; i < 5; i++){
		clock = 0;
		sc_start(10,SC_NS);
		clock = 1;
		sc_start(10,SC_NS);
	}

	reset = 1;
	cout << "[" << sc_time_stamp() << "]: " << "Assert reset" << endl;
	for( i = 0 ; i <10 ; i++){
		clock = 0;
		sc_start(10,SC_NS);
		clock = 1;
		sc_start(10,SC_NS);
	}
	
	reset = 0;
	cout << "[" << sc_time_stamp() << "]: " << "De-Assert reset" << endl;
	for( i = 0 ; i < 10 ; i++){
		clock = 0;
		sc_start(10,SC_NS);
		clock = 1;
		sc_start(10,SC_NS);
	}
	
	enable = 1;
	cout << "[" << sc_time_stamp() << "]: " << "Assert enable" << endl;
	for( i = 0 ; i < 20 ; i++){
		clock = 0;
		sc_start(10,SC_NS);
		clock = 1;
		sc_start(10,SC_NS);
	}
	
	enable = 0;
	cout << "[" << sc_time_stamp() << "]: " << "De-Assert enable" << endl;
	
	cout << "[" << sc_time_stamp() << "]: " << "Simulation terminated" << endl;	
	sc_close_vcd_trace_file(wf);
	return 0;
}
