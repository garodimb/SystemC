#ifndef MEMORY_H
#define MEMORY_H
//#define DEBUG_M
#define MEM_SIZE 1048576 //1MB
#include <systemc>
#include <tlm.h>
#include <tlm_utils/simple_target_socket.h>

using namespace std;
using namespace sc_core;
using namespace sc_dt;
using namespace tlm_utils;
using namespace tlm;


struct Memory : sc_module
{
	private:
		unsigned char *mem; // Pointer to memory
	public:
		simple_target_socket<Memory> socket; // Target socket for communication
		Memory(sc_module_name _name);
		virtual ~Memory();
		virtual void b_transport(tlm_generic_payload& trans, sc_time& delay); // Blocking transport method
};
#endif