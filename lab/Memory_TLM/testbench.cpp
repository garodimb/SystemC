#include "testbench.h"

Testbench::Testbench(sc_module_name _name) : sc_module(_name), socket("tb_socket")
{
	// Read and write delay
	r_dly = sc_time(2, SC_NS);
	w_dly = sc_time(3, SC_NS);

	// Read and write buffers
	read_buff = new unsigned char[BUFF_SIZE];
	write_buff = new unsigned char[BUFF_SIZE];

	// Read file to buffer
	read_file("1mb.bin",write_buff,BUFF_SIZE);

	SC_THREAD(read_write);
}

Testbench::~Testbench()
{
	// Free read and write buffer
	delete[] read_buff;
	delete[] write_buff;
}


void Testbench::write()
{
	// Instantiate generic payload and set attributes
	tlm_generic_payload trans;
	trans.set_command(TLM_WRITE_COMMAND);
	trans.set_data_length(4);
	trans.set_streaming_width(4);
	trans.set_byte_enable_ptr(0);
	trans.set_dmi_allowed(false);
	trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
	
	for(int i = 0 ; i < BUFF_SIZE; i+=4){
		#ifdef DEBUG_T
		cout << "[WRITE_T][ " << sc_time_stamp() << " ] MEM[ " << i << " ] : " << (unsigned int)write_buff[i] << endl;
		#endif
		trans.set_address(uint64(i));
		trans.set_data_ptr(&write_buff[i]);
		socket->b_transport(trans, w_dly);
		
		wait(w_dly); // Realize wait delay
	}

}


void Testbench::read()
{
	// Instantiate generic payload and set attributes
	tlm_generic_payload trans;
	trans.set_command(TLM_READ_COMMAND);
	trans.set_data_length(4);
	trans.set_streaming_width(4);
	trans.set_byte_enable_ptr(0);
	trans.set_dmi_allowed(false);
	trans.set_response_status(TLM_INCOMPLETE_RESPONSE);
	
	for(int i = 0 ; i < BUFF_SIZE; i+=4){
		trans.set_address(uint64(i));
		trans.set_data_ptr(&read_buff[i]);
		socket->b_transport(trans, w_dly);
		wait(r_dly); // Realize read delay
		
		#ifdef DEBUG_T
			cout << "[READ_T][ " << sc_time_stamp() << " ] MEM[ " << i << " ] : " << (unsigned int)read_buff[i] << endl;
		#endif
		
		
	}
}


void Testbench::read_write()
{
		// Write to memory
	wait(1, SC_NS);
	write();

	// Read from memory
	wait(1, SC_NS);
	read();

	// Compare read and write buffer
	cout << "Test Result: " << (compare(read_buff, write_buff, BUFF_SIZE) ? "Passed" : "Failed") << endl;
}


bool Testbench::compare(const unsigned char* read_buff, const unsigned char* write_buff, size_t len)
{
	// Comapre buffers
	for(int i = 0 ; i < len; i++){
		if(read_buff[i] != write_buff[i]){
			return false;
		}
	}
	return true;
}


size_t Testbench::read_file(char *fname, void *buf, size_t len)
{
	size_t r_len;
	if (FILE *fp = fopen(fname, "r"))
	{
		r_len = fread(buf, 1, len , fp);
		fclose(fp);
	}

	return r_len;
}
