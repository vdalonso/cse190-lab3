#include <sam.h>
#include "i2c.h"

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
      
      }
    return 0;
    }
