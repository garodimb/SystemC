/**
 * Q. Develop the model of a adder, It should have two inputs A & B of integer type,
 * and one output OUT of integer type.
 * Whenever any of the input change, it should add both inputs and put the result on
 * output port after 2 ns.

 * Test bench: Applies the values at input port at following times:
 * 0 ns: A= 0, B= 0
 * 5 ns: A= 5, B= 0
 * 7 ns: A= 5, B= 10
 * 15 ns: A= 7, B= 8

 * Whenever the value changes at the output port of adder, the testbench should print the
 * value of this port and the simulation time
 */

#include <systemc.h>

SC_MODULE(adder)
{
	sc_in<int> a;
	sc_in<int> b;
	sc_out<int> out;

	int result; // Temporary to store result
	bool is_out; // To sync whether to write or read

	void add()
	{	
		/**
		 * Adder functionality to read value at port 'a' and 'b', and output
		 * the result after 2 NS on 'out' port.
		 */
	if(!is_out){
			result = a.read() + b.read();
			next_trigger(2, SC_NS);
			is_out = true;
			}
		else{
			out.write(result);
			is_out = false;
			}
	}
	
	SC_CTOR(adder)
	{
		is_out = true; // Start from true, to nullify value of is_out after default initialization
		result = 0;
		SC_METHOD(add);
		sensitive << a << b; // Sensitive to a, and b
	}
};

SC_MODULE(testbench)
{
	sc_out<int> a;
	sc_out<int> b;
	sc_in<int> out;

	int input[12];
	int indx;

	void monitor()
	{
		/**
		 * Monitor function prints the value of 'out' whenever it changes.
		 */
		cout << "[ " << sc_time_stamp() << " ] Out : " << out << endl;
	}
	
	void test()
	{
		a.write(input[indx]);
		b.write(input[indx+1]);

		if(input[indx+2] > 0){
			next_trigger(input[indx+2], SC_NS);
			indx += 3;
		}
	}

	SC_CTOR(testbench)
	{
		input[0] = 1, input[1] = 0, input[2] = 5;
		input[3] = 5, input[4] = 0, input[5] = 2;
		input[6] = 5, input[7] = 10, input[8] = 8;
		input[9] = 10, input[10] = 11, input[11] = -1;
		indx = 0;
		SC_METHOD(monitor);
		sensitive << out;
		SC_METHOD(test);
	}

};


int sc_main(int argc, char *argv[])
{
	// Signals to connect to adder module
	sc_signal<int> a;
	sc_signal<int> b;
	sc_signal<int> out;

	// Instance of 'adder' module
	adder adder_obj("Adder");
	testbench tb("tb");

	// Bind signals to ports by name
	adder_obj.a(a);
	adder_obj.b(b);
	adder_obj.out(out);
	
	tb.a(a);
	tb.b(b);
	tb.out(out);
	
	// Create trace file for tracing
	sc_trace_file *wf = sc_create_vcd_trace_file("Adder");
	wf->set_time_unit(1, SC_NS);
		sc_trace(wf, tb.a, "a");
		sc_trace(wf, tb.b, "b");
		sc_trace(wf, tb.out, "out");

	sc_start();

	// End of simulation, close tracing file
	sc_close_vcd_trace_file(wf);
	return 0;
}
