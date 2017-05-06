#include <systemc.h>

int sc_main(int argc, char *argv[])
{
	sc_uint<4> u_num;
	sc_int<4> num;

	u_num = 15;
	num = -1;
	u_num = num;
	cout << "u_num: " << u_num << endl;
	cout << "num: " << num << endl;
	return 0;
}
