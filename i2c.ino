//Initialize I2C Peripheral
#include <sam.h>
#include "i2c.h"
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
  SERCOM3->I2CM.BAUD.bit.BAUD = 59;

  PORT->Group[0].PINCFG[22].bit.PMUXEN = 1;
  PORT->Group[0].PINCFG[23].bit.PMUXEN = 1;

  //select for (22, 23)/2 for function C
  PORT->Group[0].PMUX[22/2].reg = PORT_PMUX_PMUXE_C | PORT_PMUX_PMUXO_C ;

  
  /* ============================================ */
  }
