#include <sam.h>
#include <math.h>
#include "i2c.h"
#include "ledcircle.h"
#include "timer.h"

extern "C" void __libc_init_array(void);

extern "C" int _write(int fd, const void *buf, size_t count) {
  //STDOUT
  if(fd == 1)
    SerialUSB.write((char*)buf, count);
  return 0;
}
  volatile bool FALL = false;
  volatile bool BLINKER = false;
  volatile int COUNTER = 0;
  volatile int8_t MINUTES = 0;
  static uint8_t i = 0 ;

  
  void TC3_Handler() {

  
 
  if(TC3->COUNT16.INTFLAG.bit.MC0 == 1){
        //at every interrupt, check the low_int flag in the accelerometer
        uint8_t low_int ;
        
        i2c_transaction(0x09 , 1 , &low_int , 1);
        //printf("%x\r\n", low_int);
        if((low_int << 7)>>7){FALL = true;}
        
    //if we fell, start counting minutes, otherwise just keep counting
    if(FALL){
      if(COUNTER == 1200){
        COUNTER = 0;
        MINUTES++;
        }
      else {
        ++COUNTER;
      }
    }
    //if we actually fell, change the state of blinker to on/off every second
    if(FALL && COUNTER%20 == 0)
      BLINKER = !BLINKER;
  }
  TC3->COUNT16.INTFLAG.bit.MC0 = 1;
  timer3_reset();
  }

void my_init( void )
{
  // Set Systick to 1ms interval, common to all Cortex-M variants
  if ( SysTick_Config( SystemCoreClock / 1000 ) )
  {
    // Capture error
    while ( 1 ) ;
  }
  NVIC_SetPriority (SysTick_IRQn,  (1 << __NVIC_PRIO_BITS) - 2);  /* set Priority for Systick Interrupt (2nd lowest) */



// Defining VERY_LOW_POWER breaks Arduino APIs since all pins are considered INPUT at startup
// However, it really lowers the power consumption by a factor of 20 in low power mode (0.03mA vs 0.6mA)
#ifndef VERY_LOW_POWER
  // Setup all pins (digital and analog) in INPUT mode (default is nothing)
  for (uint32_t ul = 0 ; ul < NUM_DIGITAL_PINS ; ul++ )
  {
    pinMode( ul, INPUT ) ;
  }
#endif


}

  int main(void){
    /*=USB CONFIGURATION=*/

    my_init();
    __libc_init_array();
    USBDevice.init();
    USBDevice.attach();
    /*==================*/

   
    GCLK->GENDIV.bit.ID = 0x00;                            // select GCLK_GEN[0]
    GCLK->GENDIV.bit.DIV = 40;                              // no prescaler

    GCLK->GENCTRL.bit.ID = 0x00;                           // select GCLK_GEN[0]
    GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_OSC32K;
    GCLK->GENCTRL.bit.GENEN = 1;                           // enable generator
  
    
    /* === Init Drivers === */
    timer3_init();
    i2c_init();
    bma250_init();
    PM->SLEEP.bit.IDLE = 0x01;
    /* ==================== */

    /* ===== MAIN LOOP ===== */
    while(1) {

        while(BLINKER){
          //display out ID (8 bits) which is 0xf1 where LED 1 is the MSB for the ID and LED 8 is the LSB for the ID
          //also display the amount of minutes passed (8 bits) passed represented in binary. MSB is led 9 and LSB 16
          ledcircle_select(1);
          ledcircle_select(2);
          ledcircle_select(3);
          ledcircle_select(4);
          ledcircle_select(8);
          show_minutes();
          ledcircle_select(0);
        }
        __WFI();
      }
    return 0;
    }



    //helper function to convert minutes passed to binary in their respective LEDs
    void show_minutes(){
        int8_t curr_min = MINUTES;
        int pos = 0;
        while(pos < 8){
          if(curr_min & 0x1)
            ledcircle_select(16-pos);
          pos++;
          curr_min = curr_min>>1;
        }
      }
    //helper function to determine if the device has fallen
