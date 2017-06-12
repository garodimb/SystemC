#include "memory.h"

Memory::Memory(sc_module_name _name) : sc_module(_name), socket("mem_socket")
{
	// Allocate memory and register blocking transport method
	mem = new unsigned char[MEM_SIZE];
	socket.register_b_transport(this, &Memory::b_transport);
}

Memory::~Memory()
{
	// Free memmory
	delete[] mem;
}


void Memory::b_transport(tlm_generic_payload& trans, sc_time& delay)
{
	// Get attributes of transaction
	tlm_command 	cmd = trans.get_command();	
	uint64 			addr = trans.get_address();
	unsigned char* 	data = trans.get_data_ptr();
	unsigned int 	len = trans.get_data_length();
	unsigned char* 	byte_en = trans.get_byte_enable_ptr();
	unsigned int 	s_width = trans.get_streaming_width();


	// Check for any error and generate error
	if((addr+len) > uint64(MEM_SIZE)){
		// Address out of range error
		trans.set_response_status(TLM_ADDRESS_ERROR_RESPONSE);
		return ;
	}

	if( byte_en != 0){
		// Byte enable not supported
		trans.set_response_status(TLM_BYTE_ENABLE_ERROR_RESPONSE);
	}

	if( len > 4 || s_width < len ){
		// Streaming data transfer not supported
		trans.set_response_status(TLM_BURST_ERROR_RESPONSE);
	}
	
	if( cmd == TLM_READ_COMMAND ){
		// Read data from memory
		#ifdef DEBUG_M
			cout << "[READ_M][ " << sc_time_stamp() << " ] MEM[ " << addr
				 << " ] : " << (unsigned int)mem[addr] << endl;
		#endif
		memcpy(data, &mem[addr], len);
	}
	else if( cmd == TLM_WRITE_COMMAND){
		// Write data to memory
		memcpy(&mem[addr], data, len);
		#ifdef DEBUG_M
			cout << "[WRITE_M][ " << sc_time_stamp() << " ] MEM[ " << addr
				 << " ] : " << (unsigned int)mem[addr] << endl;
	#endif
	}

	// Set response status
	trans.set_response_status(TLM_OK_RESPONSE);

}