#include "epaper_driver.h"

void setup() {
    setup_LCD();
}

void loop() {
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