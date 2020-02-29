#include <sam.h>
#include <stdio.h>
#include "i2c.h"
#include "bma250.h"

void bma250_init(){
  //set low-g to axis-summing mode
  uint8_t data = 0x85;
  i2c_transaction(0x24 , 0 , &data , 1);
  //set interrupts to latch for 1 second.
  data = 0xff;
  i2c_transaction(0x21 , 0 , &data , 1);
  //set threshold
  data = 0x44;
  i2c_transaction(0x23 , 0 , &data , 1);
  //set to low power mode
  data = 0x04;
  i2c_transaction(0x12 , 0, &data , 1);
  i2c_transaction(0x11 , 0, &data , 1);
  //activate the interrupt
  data = 0x08;
  i2c_transaction(0x17 , 0 , &data , 1);
  }
  
