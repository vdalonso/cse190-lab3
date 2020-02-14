#include <sam.h>
#include "i2c.h"
#include <stdio.h>

extern "C" void __libc_init_array(void);

extern "C" int _write(int fd, const void *buf, size_t count) {
  //STDOUT
  if(fd == 1)
    SerialUSB.write((char*)buf, count);
  return 0;
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
    /* ==================== */

    /* ===== MAIN LOOP ===== */
    while(1) {
        int16_t x, y, z;
        bma250_read_xyz(&x, &y, &z);
        while(1){
          //printf("%d\r\n",x);
          printf("run please\n\r");
        }
      }
    return 0;
    }
