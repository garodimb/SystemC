#include <systemc.h>

SC_MODULE(event_module)
{
	sc_in<bool> clock;
	sc_event e1;
	sc_event e2;

	void do_test1()
	{
		while(true){
			wait(); // Wait for posedge of clock
			cout << "[ " << sc_time_stamp() << "] Starting Test" << endl;

			wait();
			cout << "[ " << sc_time_stamp() << "] Triggering event e1" << endl;
			e1.notify(5,SC_NS);
		
			wait();
			 cout << "[ " << sc_time_stamp() << "] Waiting for event e2" << endl;
			
			wait(e2);
			cout << "[ " << sc_time_stamp() << "] Recieved event e2" << endl;
			
			wait();
			cout << "[ " << sc_time_stamp() << "] Stopping Test" << endl;
			sc_stop();
		}
	}

	void do_test2()
	{
		while(true){
			wait();
			cout << "[ " << sc_time_stamp() << "] Waiting for event e1" << endl;
			
			wait(e1);
			cout << "[ " << sc_time_stamp() << "] Recieved event e1" << endl;

			wait(3);
			cout << "[ " << sc_time_stamp() << "] Triggering event e2" << endl;
			e2.notify();
		}
	}
	
	SC_CTOR(event_module)
	{
		SC_CTHREAD(do_test1,clock.pos());
		SC_CTHREAD(do_test2,clock.pos());
	}
};

int sc_main(int argc, char *argv[])
{
	event_module object("QUICK");
	sc_clock clock("Clock",1,0.5);
	object.clock(clock);
	sc_start(0,SC_NS);
	sc_start();
	return 0;
}
