# SystemC

## Install
* After successfull installation export LD_LIBRARY_PATH to $SYSTEMC_HOME/lib-linux64

## Compile

```bash
g++ -I. -I$SYSTEMC_HOME/include -L. -L$SYSTEMC_HOME/lib-linux64 -o out input.cpp -lm -lsystemc
```
