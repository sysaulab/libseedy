1. Install

make
sudo make install



2. Use in your code

#include "libicm.h"
icm_state_t* icm_state;
uint64_t buffer[len];
icm( &icm_state, buffer, len );



3. Link the library to the binary 

cc -licm main.c