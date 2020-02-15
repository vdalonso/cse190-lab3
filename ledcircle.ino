#include <sam.h>
#include "ledcircle.h"


void ledcircle_select(uint8_t led) {
  PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA20 | PORT_PA21 | PORT_PA06 | PORT_PA07;
  PORT->Group[0].OUTCLR.reg = PORT_PA15 | PORT_PA20 | PORT_PA21 | PORT_PA06 | PORT_PA07;
  switch(led){
      //LED 1
      case 0:
          PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA20 | PORT_PA21 | PORT_PA06 | PORT_PA07;
          PORT->Group[0].OUTCLR.reg = PORT_PA15 | PORT_PA20 | PORT_PA21 | PORT_PA06 | PORT_PA07;
          break;
      case 1:
        PORT->Group[0].DIRCLR.reg = PORT_PA21 | PORT_PA07 | PORT_PA06;
        PORT->Group[0].DIRSET.reg = PORT_PA15 | PORT_PA20;
        PORT->Group[0].OUTSET.reg = PORT_PA15;
        PORT->Group[0].OUTCLR.reg = PORT_PA20;
        break;
      //LED 2
      case 2:
        PORT->Group[0].DIRCLR.reg = PORT_PA21 | PORT_PA07 | PORT_PA06;
        PORT->Group[0].DIRSET.reg = PORT_PA15 | PORT_PA20;
        PORT->Group[0].OUTSET.reg = PORT_PA20;
        PORT->Group[0].OUTCLR.reg = PORT_PA15;
        break;
      //LED 3
      case 3:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA07 | PORT_PA06;
        PORT->Group[0].DIRSET.reg = PORT_PA15 | PORT_PA21;
        PORT->Group[0].OUTSET.reg = PORT_PA15;
        PORT->Group[0].OUTCLR.reg = PORT_PA21;
        break;
      //LED 4
      case 4:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA06 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA21 | PORT_PA15;
        PORT->Group[0].OUTSET.reg = PORT_PA21;
        PORT->Group[0].OUTCLR.reg = PORT_PA15;
        break;
      //LED 5
      case 5:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA06 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA20 | PORT_PA21;
        PORT->Group[0].OUTSET.reg = PORT_PA20;
        PORT->Group[0].OUTCLR.reg = PORT_PA21;
        break;
      //LED 6
      case 6:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA07 | PORT_PA06;
        PORT->Group[0].DIRSET.reg = PORT_PA21 | PORT_PA20;
        PORT->Group[0].OUTSET.reg = PORT_PA21;
        PORT->Group[0].OUTCLR.reg = PORT_PA20;
        break;
      //LED 7
      case 7:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA07 | PORT_PA21;
        PORT->Group[0].DIRSET.reg = PORT_PA20 | PORT_PA06;
        PORT->Group[0].OUTSET.reg = PORT_PA20;
        PORT->Group[0].OUTCLR.reg = PORT_PA06;
        break;
      //LED 8 (PARITY BIT)
      case 8:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA21 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA06 | PORT_PA20;
        PORT->Group[0].OUTSET.reg = PORT_PA06;
        PORT->Group[0].OUTCLR.reg = PORT_PA20;
        break;
      case 9:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA21 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA15 | PORT_PA06;
        PORT->Group[0].OUTSET.reg = PORT_PA15;
        PORT->Group[0].OUTCLR.reg = PORT_PA06;
        break;
      case 10:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA21 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA06 | PORT_PA15;
        PORT->Group[0].OUTSET.reg = PORT_PA06;
        PORT->Group[0].OUTCLR.reg = PORT_PA15;
        break;
      case 11:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA15 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA06 | PORT_PA21;
        PORT->Group[0].OUTSET.reg = PORT_PA06;
        PORT->Group[0].OUTCLR.reg = PORT_PA21;
        break;
      case 12:
        PORT->Group[0].DIRCLR.reg = PORT_PA20 | PORT_PA15 | PORT_PA07;
        PORT->Group[0].DIRSET.reg = PORT_PA21 | PORT_PA06;
        PORT->Group[0].OUTSET.reg = PORT_PA21;
        PORT->Group[0].OUTCLR.reg = PORT_PA06;
        break;
      case 13:
        PORT->Group[0].DIRCLR.reg = PORT_PA06 | PORT_PA15 | PORT_PA20;
        PORT->Group[0].DIRSET.reg = PORT_PA21 | PORT_PA07;
        PORT->Group[0].OUTSET.reg = PORT_PA07;
        PORT->Group[0].OUTCLR.reg = PORT_PA21;
        break;
      case 14:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA06 | PORT_PA20;
        PORT->Group[0].DIRSET.reg = PORT_PA21 | PORT_PA07;
        PORT->Group[0].OUTSET.reg = PORT_PA21;
        PORT->Group[0].OUTCLR.reg = PORT_PA07;
        break;
      case 15:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA21 | PORT_PA20;
        PORT->Group[0].DIRSET.reg = PORT_PA06 | PORT_PA07;
        PORT->Group[0].OUTSET.reg = PORT_PA07;
        PORT->Group[0].OUTCLR.reg = PORT_PA06;
        break;
      case 16:
        PORT->Group[0].DIRCLR.reg = PORT_PA15 | PORT_PA21 | PORT_PA20;
        PORT->Group[0].DIRSET.reg = PORT_PA06 | PORT_PA07;
        PORT->Group[0].OUTSET.reg = PORT_PA06;
        PORT->Group[0].OUTCLR.reg = PORT_PA07;
        break;
    }
}
