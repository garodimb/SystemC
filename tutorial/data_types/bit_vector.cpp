#include <systemc.h>

int sc_main(int argc, char *argv[])
{
	sc_bv<8> data_bus;
	sc_bv<16> addr_bus;

	//data_bus = "1001";
	cout << "Data: " << data_bus.range(7,0) << endl;
	cout << "XOR Reduce: " << data_bus.xor_reduce() << endl;
	cout << "AND Reduce: " << data_bus.and_reduce() << endl;
	cout << "OR Reduce: " << data_bus.or_reduce() << endl;
	return 0;
}
