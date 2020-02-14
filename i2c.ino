//Initialize I2C Peripheral
#include <sam.h>
#include "i2c.h"
#include <stdio.h>

void i2c_init(){
  /* ========== CONFIGURE CLOCK FOR SERCOM ============ */
  GCLK->GENDIV.reg = GCLK_GENDIV_ID (0) | GCLK_GENDIV_DIV (0); //
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  GCLK->GENCTRL.reg |= GCLK_GENCTRL_ID (0) | GCLK_GENCTRL_GENEN; //Use Generator 0 and Enable it
  while(GCLK->STATUS.bit.SYNCBUSY);//Synchronize
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM3_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN; //Use SERCOM3 Peripheral
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  PM->APBCSEL.bit.APBCDIV = 0; //No prescaler
  PM->APBCMASK.bit.SERCOM3_ = 1; //Enable SERCOM3 Interface
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE){}
  /* =================================================== */

  /* ========== SERCOM INITIALIZATION =========== */
  SERCOM3->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
  SERCOM3->I2CM.BAUD.bit.BAUD = 55;
  SERCOM3->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_MB;
  SERCOM3->I2CM.CTRLA.bit.ENABLE = 1;
  
  //intenset interrutp
  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;

  PORT->Group[0].PMUX[22/2].reg = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C;
  /* ============================================ */
  }

uint8_t i2c_transaction(uint8_t address, uint8_t dir, uint8_t* data, uint8_t len){
  //before any transaction occurs, force the bus state to be idle.
  printf("1\r\n");
  SERCOM3->I2CM.STATUS.bit.BUSSTATE = 0b01;
  while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP){}
  //dir == 1 : reading data
  if(dir){
     //send an initial write to with the address of the device.
     SERCOM3->I2CM.ADDR.bit.ADDR = 0x19 << 1 | 0;
     //wait for the slave to successfully recieve the address.
     while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
     //as for the data, send the address of the desired register
     SERCOM3->I2CM.DATA.bit.DATA = address;
     while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
     //repeated start
     SERCOM3->I2CM.CTRLB.bit.CMD = 1;
     //start the second read phase with the same address but a read bit
     SERCOM3->I2CM.ADDR.bit.ADDR = 0x19 << 1 | 1;
     //continuously read bytes from the DATA reg until we reach the desired length
     for(uint8_t i = 0; i < len ; i++){
        while(!SERCOM3->I2CM.INTFLAG.bit.SB){}
        while(1){printf("before reading data\r\n");}
        //read data - this is causing an error
        data[i] = SERCOM3->I2CM.DATA.bit.DATA;
        //NOTE: on last byte, send NACK
        if(i == len -1)
          SERCOM3->I2CM.CTRLB.bit.ACKACT = 1;
        else
          SERCOM3->I2CM.CTRLB.bit.ACKACT = 0;
        SERCOM3->I2CM.CTRLB.bit.CMD = 2;
        //printf("ran the for");
     }
     //stop condition
     SERCOM3->I2CM.CTRLB.bit.CMD = 3;
   }
   //dir == 0: writing data
   else{
    SERCOM3->I2CM.ADDR.bit.ADDR = 0x19 << 1 | 0;
    while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
    SERCOM3->I2CM.DATA.bit.DATA = address;
    while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
    for(uint8_t i = 1; i <= len ; i++){
        while(!SERCOM3->I2CM.INTFLAG.bit.SB){}
        //send data
        SERCOM3->I2CM.DATA.bit.DATA = data[len-i];
      }
      //stop condition
      SERCOM3->I2CM.CTRLB.bit.CMD = 3;
    }
    return 1;
  }
