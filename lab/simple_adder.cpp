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

	void add()
	{	
		/**
		 * Adder functionality to read value at port 'a' and 'b', and output
		 * the result after 2 NS on 'out' port.
		 */
		int result;
		while(true){
			wait();
			result = a.read() + b.read(); // Can also be written as a + b
			wait(2, SC_NS);
			out.write(result);	// Can alos be written as out = result
		}
		
	}

	SC_CTOR(adder)
	{
		SC_THREAD(add);
		sensitive << a << b; // Sensitive to a, and b
	}
};

SC_MODULE(testbench)
{
	sc_out<int> a;
	sc_out<int> b;
	sc_in<int> out;

	void monitor()
	{
		/**
		 * Monitor function prints the value of 'out' whenever it changes.
		 */
		cout << "[ " << sc_time_stamp() << " ] Out : " << out << endl;
	}
	
	void test()
	{
		a.write(10);
		b.write(0);

		wait(5, SC_NS);
		a.write(5);

		wait(2, SC_NS);
		b.write(10);

		wait(8, SC_NS);
		a.write(8);
		b.write(9);
	}

	SC_CTOR(testbench)
	{
		SC_METHOD(monitor);
		sensitive << out;
		SC_THREAD(test);
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
