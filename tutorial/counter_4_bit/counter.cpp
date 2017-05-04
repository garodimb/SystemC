#include <systemc.h>

SC_MODULE(counter){

	sc_in_clk clock;
	sc_in<bool> enable;
	sc_in<bool> reset;
	sc_out< sc_uint<4> > counter_out;

	sc_uint<4> count;

	void incr_counter(){
		if(reset.read() == 1){
			count = 0;
			counter_out.write(count);
		}
		else if(enable.read() == 1){
			count++;
			counter_out.write(count);
			cout << "[" << sc_time_stamp() <<"]"<< "Increamented to: "
				<< counter_out.read() << endl;
		}
	}

	SC_CTOR(counter){
		SC_METHOD(incr_counter);
		sensitive << reset;
		sensitive << clock.pos();
	}
};
