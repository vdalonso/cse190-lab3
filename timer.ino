#include <sam.h>
#include "ledcircle.h"
#include "timer.h"
void timer3_set(uint16_t period_ms) {
  uint16_t cycles = (48 * 1024) / 1000;
  TC3->COUNT16.CC[0].reg = period_ms * cycles; //Establish period
}
void timer3_init(){
  GCLK->GENDIV.reg = GCLK_GENDIV_ID (0) | GCLK_GENDIV_DIV (0); //
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  GCLK->GENCTRL.reg |= GCLK_GENCTRL_ID (0) | GCLK_GENCTRL_GENEN; //Use Generator 0 and Enable it
  while(GCLK->STATUS.bit.SYNCBUSY);//Synchronize
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID_TCC2_TC3 | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_CLKEN; //Use TC3 Peripheral
  while(GCLK->STATUS.bit.SYNCBUSY); //Synchronize
  PM->APBCSEL.bit.APBCDIV = 0; //No prescaler
  PM->APBCMASK.bit.TC3_ = 1; //Enable TC3 Interface

  //divide by 1024 which will give you your new frequency
  TC3->COUNT16.CTRLA.reg = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1024 | TC_CTRLA_WAVEGEN_MFRQ;
  //TC3->COUNT16.CC[0].reg = 37500; //25,000 cycles @ 500,000 cycles/s = 50ms
  timer3_set(50); //Every 50ms
  TC3->COUNT16.INTENSET.reg = TC_INTENSET_MC0; //Match interrupts on channel 0
  TC3->COUNT16.CTRLA.bit.ENABLE = 1; //Enable TC3 Peripheral
  
  while(TC3->COUNT16.STATUS.bit.SYNCBUSY){}
  NVIC_SetPriority(TC3_IRQn, 0); 
  NVIC_EnableIRQ(TC3_IRQn); //Enable TC3 NVIC Line
}

void timer3_reset() {
  TC3->COUNT16.CTRLA.bit.ENABLE = 0;
  TC3->COUNT16.COUNT.bit.COUNT = 0x00; //Resets counter to 0
  TC3->COUNT16.CTRLA.bit.ENABLE = 1;
}
