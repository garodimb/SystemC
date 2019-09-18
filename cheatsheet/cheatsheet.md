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
