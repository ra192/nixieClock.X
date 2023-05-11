#include "button.h"

void read_button(Button* btn, uint8_t pin_val)
{
    if(pin_val == 0)
    {
      switch(btn->state) {
        case RELEASED:
            btn->pressed_tick_count++;
            if(btn->pressed_tick_count > BTN_PRESS_COUNT) btn->state = PRESSED;
            break;
        case PRESSED:
            btn->pressed_tick_count++;
            btn->state = HOLD_PRESSED;
            break;
        case HOLD_PRESSED:
            btn->pressed_tick_count++;
            if(btn->pressed_tick_count > BTN_LONG_PRESS_COUNT) btn->state = LONG_PRESSED;
            break;
          case LONG_PRESSED:
              btn->state = HOLD_LONG_PRESSED;
              break;
          default:
              ;
        }  
    }
    else
    {
       btn->state = RELEASED;
       btn->pressed_tick_count = 0;
    }
}