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
  
void bma250_read_xyz(int16_t* x, int16_t* y, int16_t* z){
  //get the LSBs first
  uint8_t x_lsb;
  uint8_t y_lsb;
  uint8_t z_lsb;
  i2c_transaction(0x02 , 1 , &x_lsb , 1);
  i2c_transaction(0x04 , 1 , &y_lsb, 1);
  i2c_transaction(0x06 , 1 , &z_lsb , 1);
  
  //get the MSBs second
  uint8_t x_msb;
  uint8_t y_msb;
  uint8_t z_msb;
  i2c_transaction(0x03 , 1 , &x_msb , 1);
  i2c_transaction(0x05 , 1 , &y_msb , 1);
  i2c_transaction(0x07 , 1 , &z_msb , 1);

  *x = x_msb ;
  *y = y_msb ;
  *z = z_msb ;
  *x = (*x << 2) | (x_lsb >> 6);
  *y = (*y << 2) | (y_lsb >> 6);
  *z = (*z << 2) | (z_lsb >> 6);
  }

  
