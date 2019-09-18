# SystemC/TLM2.0 Cheatsheet

## SystemC

* ### IP Module Declaration
    ```cpp
    class ModuleName : public sc_core::sc_module
    {
    public: 
        ModuleName(sc_core::sc_module_name ModuleName);
        SC_HAS_PROCESS(ModuleName);
    };
    ```

* ### Port Declaration
    ```cpp
    sc_core::sc_in<dataType> dataIn_{ "dataIn_" };
    sc_core::sc_out<dataType> dataOut_{ "dataOut_" };
    sc_core::sc_inout<dataType> dataInout_{"dataInout_"};
    ```
    > ***Recommendation: Use direction prefix after name of port***

* ### Event Declaration
    ```cpp
    sc_core::sc_event triggerEvent_{ "triggerEvent_" };
    ```
    > ***Recommendation: Use 'Event_' after name of event***
    
* ### Process Declaration and Specifying Sentivity
    ```cpp
    // Declare member function in module class
    void runHandlerMethod();
    void transferThread();
    
    // Register member function as METHOD/THREAD in constructor
    SC_METHOD(runHandlerMethod);
    sensitive << triggerEvent_;
    dont_initialize();
    
    SC_THREAD(transferThread);
    sensitive << someEvent_;
    dont_initialize();
    ```
    > ***Recommendation: Use 'Method/Thread' as prefix for method/thread name***

* ### Signal Declaration
    ```cpp
    sc_core::sc_signal<dataType> dataSig_{ "dataSig_" };
    ```
    > ***Recommendation: Use 'Sig_' as a prefix for signal name***

* ### Port Bindings
    ```
    moduleName.dataIn_.bind(dataSig_);
    moduleName.dataOut_.bind(dataSig_);
    ```
    
## TLM-2.0
* ### Simple Socket Declaration
    ```cpp
    tlm_utils::simple_initiator_socket<ModuleName, BUSWIDTH> initiatorSocket_{ "initiatorSocket_" };
    tlm_utils::simple_initiator_socket<ModuleName, BUSWIDTH> targetSocket_{ "targetSocket_" };
    ```
* ### Forward Interface methods
    > ***Implement in Target Module***
    
    ```cpp
    void b_transport(tlm::tlm_generic_payload& trans, sc_time& delay);
    unsigned int transport_dbg(tlm::tlm_generic_payload& r);
    tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay);
    bool get_direct_mem_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi_data)
    ```
* ### Backward Interface methods
    > ***Implement in Initiator Module***

    ```cpp
    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& delay);
    void invalidate_direct_mem_ptr(sc_dt::uint64 start_range, sc_dt::uint64 end_range);
    ```
* ### Registering member function as callback to simple socket
    ```cpp
    targetSocket_.register_b_transport(this, &ModuleName::memberFunctionName);
    ```
* ### Binding Sockets
    ```cpp
    initiatorModule.initiatorSocket_.bind(targetModule.targetSocket_);
    ```