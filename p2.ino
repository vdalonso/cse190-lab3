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
        //at every interrupt, check the readings for the accelerometer
        int16_t x, y, z;
        bma250_read_xyz(&x, &y, &z);
        //check if our device is falling
        check_fall(x, y, z);
        printf("%d %d %d \r\n",x , y , x);
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
    bma250_init();
    //ledcircle_select(0);
    timer3_init();
    //bma250_read_xyz(&x_prev, &y_prev, &z_prev);
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
    void check_fall(int16_t x, int16_t y, int16_t z){
      int vec_sum = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
      if(!FREE_FALL && vec_sum < 110){
        FREE_FALL = true;
        FALL_SUM = vec_sum;
        return;
      }
      else if(vec_sum < 110){FALL_SUM = vec_sum;}
      else if(vec_sum > 400 && FALL_SUM < 110){FALL = true;}
      else if(FALL){return;}
      else{
        FREE_FALL = false;
        FALL_SUM = vec_sum;
        return;
        }
    }
