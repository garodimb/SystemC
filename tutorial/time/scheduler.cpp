#include <systemc.h>

SC_MODULE(some_block)
{
	sc_in<bool> clock;
	sc_in<bool> data;
	sc_in<bool> reset;
	sc_in<bool> inv;
	sc_out<bool> out;

	void body()
	{
		if(reset.read() == 1){
			out = bool(0);
		}
		else if(inv.read() == 1){
			out = ~out.read();
		}
		else{
			out.write(data.read());
		}
	}
	
	SC_CTOR(some_block)
	{
		SC_METHOD(body);
		sensitive << clock.pos();
	}
};


SC_MODULE(signal_bind)
{
	sc_in<bool> clock;
	
	sc_signal<bool> data;
	sc_signal<bool> reset;
	sc_signal<bool> inv;
	sc_signal<bool> out;
	some_block *block;

	void do_test()
	{
		while(true){
			wait();
			cout << "[ " << sc_time_stamp() << " ] Starting simulation." << endl;
			wait();
			wait();
			inv = false;
			data = false;;
			cout << "[ " << sc_time_stamp() << " ] Driving all inputes to 0" << endl;
			
			reset = true;
			cout << "[ " << sc_time_stamp() << " ] Asserting reset." << endl;
			
			wait();
			wait();
			reset = false;
			cout << "[ " << sc_time_stamp() << " ] De-Asserting reset" << endl;
				
			wait();
			wait();
			cout << "[ " << sc_time_stamp() << " ] Asserting data" << endl;
			data = true;
			
			wait();
			wait();	
			cout << "[ " << sc_time_stamp() << " ] Asserting inv" << endl;
			inv = true;
			
			wait();
			wait();	
			cout << "[ " << sc_time_stamp() << " ] De-Asserting inv" << endl;
			inv = false;;
			
			wait();
			wait();	
			cout << "[ " << sc_time_stamp() << " ] Stopping simulation" << endl;
			sc_stop();
		}
	}

	void monitor()
	{
		cout << "[ " << sc_time_stamp() << " ] Data: " << data.read() <<", Out: " << out.read()
			<< ", Reset: " << reset.read() << ", Inv: " << inv.read() << endl;
	}
	
	SC_CTOR(signal_bind)
	{
		block = new some_block("First_Block");
		block->clock(clock);
		block->data(data);
		block->out(out);
		block->reset(reset);
		block->inv(inv);

		SC_CTHREAD(do_test, clock.pos());
		SC_METHOD(monitor);
		sensitive << data << reset << inv << out;
	}
	
};


int sc_main(int argc, char *argv[])
{
	sc_clock clock("my_clock",1,0.5);
	signal_bind bind("Signal_Bind");
	
	bind.clock(clock);

	sc_trace_file *wf = sc_create_vcd_trace_file("sc_start");
		sc_trace(wf, bind.clock, "clk");
		sc_trace(wf, bind.data, "data");
		sc_trace(wf, bind.reset, "reset");
		sc_trace(wf, bind.out, "out");
		sc_trace(wf, bind.inv, "inv");
		
	sc_start(0,SC_PS);
	sc_start(1,SC_PS);
	sc_start(1,SC_SEC);

	cout << "Closing VCD file" << endl;
	sc_close_vcd_trace_file(wf);
	return 0;
}
