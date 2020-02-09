#include <sam.h>
#include "i2c.h"

void i2c_init(){
  //ENABLE AND SELEC THE I2C MODE
  SERCOM.I2CM->CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE | SERCOM_I2CM_MODE_I2C_MASTER;
  }
