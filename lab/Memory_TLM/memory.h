#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 1024
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
		unsigned char *mem;


	public:
		simple_target_socket<Memory> socket;
		Memory(sc_module_name _name);
		virtual ~Memory();
		virtual void b_transport(tlm_generic_payload& trans, sc_time& delay);
};
#endif