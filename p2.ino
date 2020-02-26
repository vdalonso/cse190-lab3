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
  volatile bool FREE_FALL = false;
  volatile bool BLINKER = false;
  volatile int COUNTER = 0;
  volatile int8_t MINUTES = 0;
  volatile int FALL_SUM = 2000;
  
  void TC3_Handler() {
  if(TC3->COUNT16.INTFLAG.bit.MC0 == 1){
        //at every interrupt, check the low_int flag in the accelerometer
        uint8_t low_int = 0x11;
        i2c_transaction(0x09 , 1 , &low_int , 1);
        printf("%x\r\n", low_int);
        if((low_int << 7)>>7){FALL = true;}
        /*
        uint8_t test = 0x88;
        i2c_transaction(0x22 , 0 , &test , 1);
        uint8_t result;
        i2c_transaction(0x22 , 1 , &result , 1);
        printf("%x\r\n", result);
        */
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

  int main(void){
    /*=USB CONFIGURATION=*/
    init();
    __libc_init_array();
    USBDevice.init();
    USBDevice.attach();
    /*==================*/

    /* === Init Drivers === */
    i2c_init();
    //bma250_init();
    timer3_init();
    //while(true){printf("before init");}
    bma250_init();
    /* ==================== */

    /* ===== MAIN LOOP ===== */
    while(1) {
        if(BLINKER){
          //display out ID (8 bits) which is 0xf1 where LED 1 is the MSB for the ID and LED 8 is the LSB for the ID
          //also display the amount of minutes passed (8 bits) passed represented in binary. MSB is led 9 and LSB 16
          ledcircle_select(1);
          ledcircle_select(2);
          ledcircle_select(3);
          ledcircle_select(4);
          ledcircle_select(8);
          ledcircle_select(0);
          show_minutes();
        }
        else
          ledcircle_select(0);
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
