#include "memory.h"

Memory::Memory(sc_module_name _name) : sc_module(_name), socket("mem_socket")
{
	mem = new unsigned char[MEM_SIZE];
	socket.register_b_transport(this, &Memory::b_transport);
}

Memory::~Memory()
{
	delete[] mem;
}


void Memory::b_transport(tlm_generic_payload& trans, sc_time& delay)
{
	tlm_command 	cmd = trans.get_command();	
	uint64 			addr = trans.get_address();
	unsigned char* 	data = trans.get_data_ptr();
	unsigned int 	len = trans.get_data_length();
	unsigned char* 	byte_en = trans.get_byte_enable_ptr();
	unsigned int 	s_width = trans.get_streaming_width();


	if((addr+len) >= uint64(MEM_SIZE)){
		trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
		return ;
	}

	if( byte_en != 0){
		trans.set_response_status(TLM_BYTE_ENABLE_ERROR_RESPONSE);
	}

	if( len > 4 || s_width < len ){
		trans.set_response_status(TLM_BURST_ERROR_RESPONSE);
	}
	
	if( cmd == TLM_READ_COMMAND ){
		memcpy(data, &mem[addr], len);
	}
	else if( cmd == TLM_WRITE_COMMAND){
		memcpy(&mem[addr], data, len);
	}

	trans.set_response_status(TLM_OK_RESPONSE);

}