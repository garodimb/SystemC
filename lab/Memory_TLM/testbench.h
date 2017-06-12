#ifndef TESTBENCH_H
#define TESTBENCH_H
//#define DEBUG_T 1
#include <systemc>
#include <tlm.h>
#include <tlm_utils/simple_initiator_socket.h>

#define BUFF_SIZE 1048576 // 1MB
using namespace std;
using namespace tlm;
using namespace tlm_utils;
using namespace sc_dt;
using namespace sc_core;

struct Testbench : sc_module
{
	private:
		unsigned char* read_buff, *write_buff;
		sc_time r_dly; // Read delay
 		sc_time w_dly; // Write delay
		void read(); // Read from memory
		void write(); // Write to memory
		void read_write(); // Do read and write operation
		size_t read_file(char *fname, void *buf, size_t len);
		bool compare(const unsigned char* read_buff, const unsigned char* write_buff, size_t len);

	public:
		simple_initiator_socket<Testbench> socket; // Initiator socket
		Testbench(sc_module_name _name);
		virtual ~Testbench();

		SC_HAS_PROCESS(Testbench);

};

#endif