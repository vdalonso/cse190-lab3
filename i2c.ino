//Initialize I2C Peripheral
#include <sam.h>
#include "i2c.h"
//#include <stdio.h>

void i2c_init(){



  /* ========== CONFIGURE 8MHz Clock Source and enable connect to GCLK(0) ============ */
  SYSCTRL->OSC8M.bit.PRESC = 0;                          // no prescaler (is 8 on reset)
  SYSCTRL->OSC8M.reg |= 1 << SYSCTRL_OSC8M_ENABLE_Pos;   // enable source

  GCLK->GENDIV.bit.ID = 0x00;                            // select GCLK_GEN[1]
  GCLK->GENDIV.bit.DIV = 0;                              // no prescaler

  GCLK->GENCTRL.bit.ID = 0x00;                           // select GCLK_GEN[1]
  GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_OSC8M;           // OSC8M source
  GCLK->GENCTRL.bit.GENEN = 1;                           // enable generator
  

  /* ========== CONFIGURE CLOCK FOR SERCOM ============ */
  GCLK->GENDIV.reg = GCLK_GENDIV_ID (0) | GCLK_GENDIV_DIV (0); //
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  GCLK->GENCTRL.reg |= GCLK_GENCTRL_ID (0) | GCLK_GENCTRL_GENEN; //Use Generator 0 and Enable it
  while(GCLK->STATUS.bit.SYNCBUSY);//Synchronize
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_SERCOM3_CORE | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN; //Use SERCOM3 Peripheral
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  PM->APBCSEL.bit.APBCDIV = 0; //No prescaler
  PM->APBCMASK.bit.SERCOM3_ = 1; //Enable SERCOM3 Interface
  /* =================================================== */

  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;
  PORT->Group[0].PMUX[11].reg = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C;

  /* ========== SERCOM INITIALIZATION =========== */
  SERCOM3->I2CM.CTRLA.bit.ENABLE = 0;
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE){}
  SERCOM3->I2CM.CTRLA.reg |= SERCOM_I2CM_CTRLA_MODE_I2C_MASTER;
  SERCOM3->I2CM.CTRLB.bit.SMEN = 1;
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE){}
  SERCOM3->I2CM.BAUD.bit.BAUD = 55;
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE){}
  SERCOM3->I2CM.INTENSET.reg = SERCOM_I2CM_INTENSET_SB | SERCOM_I2CM_INTENSET_MB;
  SERCOM3->I2CM.CTRLA.bit.ENABLE = 1;
  while(SERCOM3->I2CM.SYNCBUSY.bit.ENABLE){}
  /* ========== Clock Gating =========== */
  //APBC
  PM->APBCMASK.bit.PAC2_ = PM->APBCMASK.bit.EVSYS_ = PM->APBCMASK.bit.SERCOM0_ = PM->APBCMASK.bit.SERCOM1_ = 
  PM->APBCMASK.bit.SERCOM2_ = PM->APBCMASK.bit.SERCOM4_ = PM->APBCMASK.bit.SERCOM5_ = PM->APBCMASK.bit.TCC0_ = 
  PM->APBCMASK.bit.TCC1_ = PM->APBCMASK.bit.TCC2_ = PM->APBCMASK.bit.TC4_  = PM->APBCMASK.bit.TC5_ = PM->APBCMASK.bit.TC6_ = 
  PM->APBCMASK.bit.TC7_= PM->APBCMASK.bit.ADC_ = PM->APBCMASK.bit.AC_ = PM->APBCMASK.bit.DAC_ = PM->APBCMASK.bit.PTC_ = PM->APBCMASK.bit.I2S_ = 0;
  //APBB
  PM->APBBMASK.bit.PAC1_ = PM->APBBMASK.bit.DSU_ = PM->APBBMASK.bit.NVMCTRL_ = PM->APBBMASK.bit.DMAC_ = PM->APBBMASK.bit.HMATRIX_ = 0;
  //APBA
  PM->APBAMASK.bit.PAC0_ = PM->APBAMASK.bit.PM_ = PM->APBAMASK.bit.SYSCTRL_ = PM->APBAMASK.bit.WDT_ = PM->APBAMASK.bit.RTC_ = PM->APBAMASK.bit.EIC_ =0;
  //AHB  TODO 
  
  }

uint8_t i2c_transaction(uint8_t address, uint8_t dir, uint8_t* data, uint8_t len){
  //before any transaction occurs, force the bus state to be idle.
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
     //SERCOM3->I2CM.CTRLB.bit.CMD = 1;
     //start the second read phase with the same address but a read bit
     SERCOM3->I2CM.ADDR.bit.ADDR = 0x19 << 1 | 1;
     //continuously read bytes from the DATA reg until we reach the desired length
     for(uint8_t i = 0; i < len ; i++){
        while(!SERCOM3->I2CM.INTFLAG.bit.SB){}
        //while(1){printf("before reading data\r\n");}
        //read data - this is causing an error
        //NOTE: on last byte, send NACK
        if(i == len -1)
          SERCOM3->I2CM.CTRLB.bit.ACKACT = 1;
        while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP){}
        data[i] = SERCOM3->I2CM.DATA.bit.DATA;
     }
     //stop condition
     SERCOM3->I2CM.CTRLB.bit.CMD = 3;
     while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP){}
   }
   //dir == 0: writing data
   else{
    SERCOM3->I2CM.ADDR.bit.ADDR = 0x19 << 1 | 0;
    while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
    SERCOM3->I2CM.DATA.bit.DATA = address;
    while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
    for(uint8_t i = 1; i <= len ; i++){
        while(!SERCOM3->I2CM.INTFLAG.bit.MB){}
        //send data
        SERCOM3->I2CM.DATA.bit.DATA = data[len-i];
        //if(i == len -1)
          //SERCOM3->I2CM.CTRLB.bit.ACKACT = 1;
        while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP){}
      }
     //stop condition
     //SERCOM3->I2CM.CTRLB.bit.CMD = 3;
     while(SERCOM3->I2CM.SYNCBUSY.bit.SYSOP){}
    }
    return 1;
  }
