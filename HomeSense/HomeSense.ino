#include "epaper_driver.h"
#include "captivePortal.h"

void setup() {
    setup_LCD();
    captivePortal_setup();
}

void loop() {
    captivePortal_loop();
    for (int curnum = 0; curnum < 199; curnum++) {
        if (curnum <= 99) {
            display(curnum, curnum);
        }
        else {
            display(curnum, uint8_t(99));
        }
        delay(1000);
    }
}