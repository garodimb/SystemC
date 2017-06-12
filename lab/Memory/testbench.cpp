#include <testbench.h>

Testbench::Testbench(sc_module_name _name) : sc_module(_name)
{
	// Read and write delay
	r_dly = sc_time(2, SC_NS);
	w_dly = sc_time(3, SC_NS);

	// Read and write buffers
	read_buff = new sc_uint<DATA_WIDTH>[BUFF_SIZE];
	write_buff = new sc_uint<DATA_WIDTH>[BUFF_SIZE];
	
	// Read file to buffer
	read_file("1mb.bin",write_buff,BUFF_SIZE);

	SC_THREAD(read_write);
}

Testbench::~Testbench()
{
	// Delete allocated memory
	delete[] read_buff;
	delete[] write_buff;
}

void Testbench::read()
{
	// Deassert read_en
	read_en.write(0);
	wait(1, SC_NS);

	for(int i = 0 ; i < BUFF_SIZE; i++){
		
		// Read from memory
		addr_bus.write(i);
		read_en.write(1);
		wait(r_dly); // Realize read delay
		read_buff[i] = data_bus.read();
		
		#ifdef DEBUG_T
			cout << "[READ_T][ " << sc_time_stamp() << " ] MEM[ " << i << " ] : " << read_buff[i] << endl;
		#endif

		// Deassert read_en
		read_en.write(0);
		wait(1, SC_NS);
	}
}

void Testbench::write()
{
	// Deassert write_en
	write_en.write(0);
	wait(1, SC_NS);

	for(int i = 0; i < BUFF_SIZE; i++){

		#ifdef DEBUG_T
		cout << "[WRITE_T][ " << sc_time_stamp() << " ] MEM[ " << i << " ] : " << write_buff[i] << endl;
		#endif
		// Write to memory
		addr_bus.write(i);
		data_bus.write(write_buff[i]);
		write_en.write(1);
		wait(w_dly); // Realize write delay

		// Deassert write_en
		write_en.write(0);
		wait(1, SC_NS);
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

bool Testbench::compare(const sc_uint<32> *read_buff, const sc_uint<32> *write_buff, size_t len)
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