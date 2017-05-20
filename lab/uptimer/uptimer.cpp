#include <systemc.h>
#define MAX_VALUE 		255

#define TIMER_CNTRL 		0x0
#define TIMER_VAL 		0x4
#define TIMER_CMP 		0x8
#define	TIMER_INTR_STATUS 	0xC

// Timer control register bits
#define TIMER_CNTRL_EN		0
#define TIMER_CNTRL_CMP		1
#define TIMER_CNTRL_OV		2

#define TIMER_INTR_STATUS_CMP 	0
#define TIMER_INTR_STATUS_OV	1

#define CHECK_BIT(bit, number) ( 1 << (bit) & (number) ) // LSB - 0th bit

SC_MODULE(uptimer)
{
	private:
		sc_bv<8> timer_cntrl; // Timer control register
		sc_uint<8> timer_val; // Timer value register
		sc_uint<8> timer_cmp; // Timer compare register
		sc_bv<8> timer_intr_status; // Timer interrupt status register

	public:
		sc_in<bool> clock; // Clock
		sc_in<bool> reset; // Reset
		sc_in< sc_uint<8> > addr; // In Address
		sc_in< sc_uint<8> > data_in; // In Data

		sc_out<bool> int0; // Overflow interrupt
		sc_out<bool> int1; // Comparatore interrupt
		sc_out< sc_uint<8> > data_out; // Data out

		sc_in<bool> read_en; // Read enable
		sc_in<bool> write_en; // Write enable

	void incr()
	{
		/**
		 * Increment timer if timer is enable and set corresponding
		 * interrupts on overflow and match
		 */
		if(!timer_cntrl[TIMER_CNTRL_EN])
			return;

		// Timer value is MAX_VALUE, set overflow interrupt
		if(timer_val==MAX_VALUE){
			timer_val = 0;
			if(timer_cntrl[TIMER_CNTRL_OV]){
				int0.write(1);
				timer_intr_status[TIMER_CNTRL_OV] = 1;
			}
		}
		else{
			timer_val = timer_val + 1;
			int0.write(0);
			timer_intr_status[TIMER_CNTRL_OV] = 0;
		}

		// Timer value matches to compare value, set comparator interrupt
		if(timer_cntrl[TIMER_CNTRL_CMP] && timer_val==timer_cmp){
			int1.write(1);
			timer_intr_status[TIMER_CNTRL_CMP] = 1;
		}
		else{
			int1.write(0);
			timer_intr_status[TIMER_CNTRL_CMP] = 0;
		}

	}

	void read()
	{
		/**
		 * Read TIMER_CNTRL, TIMER_VAL and TIMER_CMP registers
		 */
		if(read_en.read()){
			switch(addr.read()){
				case TIMER_CNTRL:
						data_out.write(timer_cntrl);
						break;

				case TIMER_VAL:
						data_out.write(timer_val);
						break;

				case TIMER_CMP:
						data_out.write(timer_cmp);
						break;
			}
		}
			
	}

	void write()
	{
		/**
		 * Write to TIMER_CNTRL, TIMER_CMP and TIMER_INTR_STATUS
		 */
		if(write_en.read()){
			switch(addr.read()){
				case TIMER_CNTRL:
						timer_cntrl.range(7, 0) = data_in.read();
						break;

				case TIMER_CMP:
						timer_cmp = data_in.read();
						break;
				
				// Reset interrupts by write 1 clear register
				case TIMER_INTR_STATUS:
							if(CHECK_BIT(TIMER_INTR_STATUS_OV, data_in.read())){
								timer_intr_status[TIMER_CNTRL_OV] = 0;
								int0.write(0);
							}
							if(CHECK_BIT(TIMER_INTR_STATUS_CMP, data_in.read())){
								timer_intr_status[TIMER_INTR_STATUS_CMP] = 0;
								int1.write(0);
							}
			}
		}
			
	}
	
	void reset_mod()
	{
		// Reset module
		if(reset.read()){
			timer_cntrl = 0;
			timer_val = 0;
			timer_cmp = 0;
			timer_intr_status = 0;
			int0 = 0;
			int1 = 0;
		}
	}

	SC_CTOR(uptimer)
	{
		timer_cntrl = 0x07;
		timer_val = 0x00;
		timer_cmp = 0x00;
		timer_intr_status = 0x00;

		// Increment sensitive to timer clock
		SC_METHOD(incr);
		sensitive << clock.pos();
		dont_initialize();
		// Read sensitive to read_en or addr
		SC_METHOD(read);
		sensitive << read_en << addr;

		// Write sensitive to write_en, addr or data_in
		SC_METHOD(write);
		sensitive << write_en << addr << data_in;

		// reset_mod sensitive to reset
		SC_METHOD(reset_mod);
		sensitive << reset;
	}

	void end_of_elaboration()
	{
		int0 = 0;
		int1 = 0;
	}

};



SC_MODULE(testbench)
{
	sc_in<bool> clock; // Clock
	sc_out<bool> reset; // Reset
	sc_out< sc_uint<8> > addr; // In Address
	sc_out< sc_uint<8> > data_in; // In Data

	sc_in<bool> int0; // Overflow interrupt
	sc_in<bool> int1; // Comparator interrupt
	sc_in< sc_uint<8> > data_out; // Data out

	sc_out<bool> read_en; // Read enable
	sc_out<bool> write_en; // Write enable

	bool intr_msg; // Flag to control interrupt asserted messages

	void test()
	{
		intr_msg = true;
		addr.write(TIMER_CNTRL);
		data_in.write(0x07);
		write_en.write(1);
		intr_msg = true;

		wait(0, SC_NS);
		write_en.write(0);

		while(true){
			// At 0 ns: Write the value to timer_cmp = 135
			addr.write(TIMER_CMP);
			data_in.write(135);
			write_en.write(1);
			wait(1, SC_NS);
			write_en.write(0);

			// At 18470 ns Read the value of register timer_val and print the value
			wait(18469, SC_NS);
			addr.write(TIMER_VAL);
			read_en.write(1);
			wait(1, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << data_out.read() << endl;
			read_en.write(0);

			// At 19990 ns Read the value of register timer_val and print the value
			wait(1519, SC_NS);
			addr.write(TIMER_VAL);
			read_en.write(1);
			wait(1, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << data_out.read() << endl;
			read_en.write(0);

			// At 20000 ns (20 uS), disable the timer
			wait(9, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] Disable timer" << endl;
			addr.write(TIMER_CNTRL);
			data_in.write(0x06);
			write_en.write(1);
			wait(1, SC_NS);
			write_en.write(0);

			// At 20500 ns Read the value of register timer_val and print the value
			wait(499, SC_NS);
			addr.write(TIMER_VAL);
			read_en.write(1);
			wait(1, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << data_out.read() << endl;
			read_en.write(0);
			
			// At 21000 ns. (21uS) enable the timer
			wait(499, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] Enable timer" << endl;
			addr.write(TIMER_CNTRL);
			data_in.write(0x07);
			write_en.write(1);
			wait(1, SC_NS);
			write_en.write(0);

			// At 29990 Read the value of register timer_val and print the value
			wait(8989, SC_NS);
			addr.write(TIMER_VAL);
			read_en.write(1);
			wait(1, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << data_out.read() << endl;
			read_en.write(0);

			// At 30000 ns (30 uS) change the value of timer_cmp
			wait(9, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] Changing timer_cmp value to 100" << endl;
			addr.write(TIMER_CMP);
			data_in.write(100);
			write_en.write(1);
			wait(1, SC_NS);
			write_en.write(0);

			// At 40000 ns (40 uS) , Turn off the interrupt asserted messages
			wait(9999, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << "Turn off interrupt asserted messages" << endl;
			intr_msg = false;

			// At 1000000 ns (1ms), Turn on the interrupt asserted messages
			wait(960000, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << "Turn on interrupt asserted messages" << endl;
			intr_msg = true;

			wait(750, SC_NS);
			addr.write(TIMER_VAL);
			read_en.write(1);
			wait(1, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << data_out.read() << endl;
			read_en.write(0);

			// At 1010000 ns, Turn off the interrupt asserted messages
			wait(9249, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] " << "Turn off interrupt asserted messages" << endl;
			intr_msg = false;

			wait(98990000, SC_NS);
			cout << "[ " << sc_time_stamp() << " ] Stopping simulation" << endl;
			sc_stop();
		}
	}

	void monitor_int0(){
		if(int0.read() && intr_msg){
			cout << "Interrupt asserted: Intr0 ( " << sc_time_stamp() << " )" << endl; 
		}
	}

	void monitor_int1(){
		if(int1.read() && intr_msg){
			cout << "Interrupt asserted: Intr1 ( " << sc_time_stamp() << " )" << endl; 
		}
	}

	SC_CTOR(testbench)
	{	
		intr_msg = false;
		SC_THREAD(test);

		SC_METHOD(monitor_int0);
		sensitive << int0;

		SC_METHOD(monitor_int1);
		sensitive << int1;
	}
};


int sc_main(int argc, char *argv[])
{
	sc_clock clock("my_clock", 20, SC_NS, 0.5);

	uptimer timer("timer");
	testbench tb("tb");

	sc_signal<bool> reset; // Reset
	sc_signal< sc_uint<8> > addr; // In Address
	sc_signal< sc_uint<8> > data_in; // In Data

	sc_signal<bool> int0;  // Overflow interrupt
	sc_signal<bool> int1; // Copmarator interrupt
	sc_signal< sc_uint<8> > data_out; // Data out

	sc_signal<bool> read_en; // Read enable
	sc_signal<bool> write_en; // Write enable

	// Connect signals to timer instance
	timer.clock(clock);
	timer.reset(reset);
	timer.addr(addr);
	timer.data_in(data_in);
	timer.int0(int0);
	timer.int1(int1);
	timer.data_out(data_out);
	timer.read_en(read_en);
	timer.write_en(write_en);

	// Connect signals to testbench instance
	tb.clock(clock);
	tb.reset(reset);
	tb.addr(addr);
	tb.data_in(data_in);
	tb.int0(int0);
	tb.int1(int1);
	tb.data_out(data_out);
	tb.read_en(read_en);
	tb.write_en(write_en);

	// Start simulation
	sc_start();
	return 0;
}

/**
Output:

        SystemC 2.3.1-Accellera --- May  8 2017 10:53:14
        Copyright (c) 1996-2014 by all Contributors,
        ALL RIGHTS RESERVED
Interrupt asserted: Intr1 ( 2680 ns )
Interrupt asserted: Intr0 ( 5100 ns )
Interrupt asserted: Intr1 ( 7800 ns )
Interrupt asserted: Intr0 ( 10220 ns )
Interrupt asserted: Intr1 ( 12920 ns )
Interrupt asserted: Intr0 ( 15340 ns )
Interrupt asserted: Intr1 ( 18040 ns )
[ 18471 ns ] 156
[ 19991 ns ] 232
[ 20 us ] Disable timer
[ 20501 ns ] 233
[ 21 us ] Enable timer
Interrupt asserted: Intr0 ( 21460 ns )
Interrupt asserted: Intr1 ( 24160 ns )
Interrupt asserted: Intr0 ( 26580 ns )
Interrupt asserted: Intr1 ( 29280 ns )
[ 29991 ns ] 170
[ 30 us ] Changing timer_cmp value to 100
Interrupt asserted: Intr0 ( 31700 ns )
Interrupt asserted: Intr1 ( 33700 ns )
Interrupt asserted: Intr0 ( 36820 ns )
Interrupt asserted: Intr1 ( 38820 ns )
[ 40 us ] Turn off interrupt asserted messages
[ 1 ms ] Turn on interrupt asserted messages
[ 1000751 ns ] 68
Interrupt asserted: Intr1 ( 1001380 ns )
Interrupt asserted: Intr0 ( 1004500 ns )
Interrupt asserted: Intr1 ( 1006500 ns )
Interrupt asserted: Intr0 ( 1009620 ns )
[ 1010 us ] Turn off interrupt asserted messages
[ 100 ms ] Stopping simulation

Info: /OSCI/SystemC: Simulation stopped by user.

real	0m2.875s
user	0m2.864s
sys	0m0.004s
**/
