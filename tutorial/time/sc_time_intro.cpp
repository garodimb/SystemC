#include <systemc.h>

int sc_main(int argc, char *argv[])
{
	// Set time resolution to Picoseconds
	sc_set_time_resolution(1,SC_PS);
	
	sc_time t1(1,SC_SEC); // Time object with 1 sec as value
	sc_time t2(2050,SC_US); // Time object with 2050 microseconds
	sc_time t3(1000,SC_PS); // Time object with 1000 picoseconds
	cout << "T3: " << t3.to_string() << endl;
	cout << "T1: " << t1 << ", T2: " << t2 << endl;

	sc_start(1000,SC_PS); // Invoke scheduler and do simulation for 1000 picoseconds
	cout << "Current Time " << sc_time_stamp() << endl; // Current time = 1000 picoseconds
	cout << "T1 + T2 + T3: " << t1 + t2 + t3 << endl;  // Addition of all time
	return 0;
}
