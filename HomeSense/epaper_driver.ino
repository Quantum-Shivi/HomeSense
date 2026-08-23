// WAVESHARE 1.9-INCH SEGMENT E-PAPER 

// Driver File

#include <Wire.h>
#include "epaper_driver.h"

// COMMAND SEQUENCES
uint8_t BOOST_SEQ[] = {0xA7, 0xE0};
uint8_t TEMP_SEQ[] = {0x7E, 0x81, 0xB4};
uint8_t FRAMETIME_SEQ[] = {0xE7, 0x0E};
uint8_t DISPLAYDATA_SEQ[] = {0xAC, 0x2B, 0x40, 0xA9, 0xA8};
uint8_t STARTREFRESH_SEQ[] = {0xAB, 0xAA, 0xAF};
uint8_t FINISH_SEQ[] = {0xAE, 0x28, 0xAD};

// SEGMENT CODES LUT for 7-segment display
uint8_t segment_code[10][3] = {
    {0xBF, 0x1F, 0x00}, 
    {0x00, 0x1F, 0x1F}, 
    {0xFD, 0x17, 0x00}, 
    {0xF5, 0x1F, 0x00}, 
    {0x47, 0x1F, 0x00}, 
    {0xF7, 0x1D, 0x00}, 
    {0xFF, 0x1D, 0x00}, 
    {0x21, 0x1F, 0x00}, 
    {0xFF, 0x1F, 0x00}, 
    {0xF7, 0x1F, 0x00}  
};


// Full Sequence to operate the epaper Display
//RST
 //↓
//INITIALIZATION
 //↓
//COMMANDS
 //↓
//15-byte DATA BUFFER + 0x00
 //↓
//0xAB → 0xAA → 0xAF
 //↓
//BUSY HIGH
 //↓
//0xAE → 0x28 → 0xAD

// Function Definitions

void reset() {
    digitalWrite(RST_PIN, HIGH);
    delay(200);
    digitalWrite(RST_PIN, LOW);
    delay(20);
    digitalWrite(RST_PIN, HIGH);
    delay(200);
}

void busy() {
    unsigned long start_time = millis();
    while (digitalRead(BUSY_PIN) == LOW) {
        if (millis() - start_time > 4000) {
            break;
        }
        delay(1);
    }
}

void send_cmd(uint8_t cmd) {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    Wire.write(cmd);
    Wire.endTransmission();
}

void send_data(uint8_t data) {
    Wire.beginTransmission(DATA_ADDRESS_EPAPER);
    Wire.write(data);
    Wire.endTransmission();
}

void POWER_ON() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    Wire.write(POWER_ON_CMD);
    Wire.endTransmission();
}

void BOOST() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(BOOST_SEQ); x++) {
        Wire.write(BOOST_SEQ[x]);
    }
    Wire.endTransmission();
}

void TEMP() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(TEMP_SEQ); x++) {
        Wire.write(TEMP_SEQ[x]);
    }
    Wire.endTransmission();
}

void FRAMETIME() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(FRAMETIME_SEQ); x++) {
        Wire.write(FRAMETIME_SEQ[x]);
    }
    Wire.endTransmission();
}

void DISPLAYDATA() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(DISPLAYDATA_SEQ); x++) {
        Wire.write(DISPLAYDATA_SEQ[x]);
    }
    Wire.endTransmission();
}

void STARTREFRESH() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(STARTREFRESH_SEQ); x++) {
        Wire.write(STARTREFRESH_SEQ[x]);
    }
    Wire.endTransmission();
}


void FINISH() {
    Wire.beginTransmission(CMD_ADDRESS_EPAPER);
    for (int x = 0; x < sizeof(FINISH_SEQ); x++) {
        Wire.write(FINISH_SEQ[x]);
    }
    Wire.endTransmission();
}

void display(int num = 0, int bottom_num = 0) {
    
    uint8_t array_buffer[15] = {0};
    uint8_t digit_array[3] = {0};

    for (int i = 0; i < 3; i++) {
        digit_array[i] = num % 10;
        num /= 10;
    }


    array_buffer[0] = segment_code[digit_array[2]][2];
    array_buffer[1] = segment_code[digit_array[1]][0];
    array_buffer[2] = segment_code[digit_array[1]][1];
    array_buffer[3] = segment_code[digit_array[0]][0];
    array_buffer[4] = segment_code[digit_array[0]][1];

    uint8_t bottom_digit_array[2] = {0};

    for (int i = 0; i < 2; i++) {
        bottom_digit_array[i] = bottom_num % 10;
        bottom_num /= 10;
    }

    array_buffer[5] = segment_code[bottom_digit_array[1]][0];
    array_buffer[6] = segment_code[bottom_digit_array[1]][1];
    array_buffer[7] = segment_code[bottom_digit_array[0]][0];
    array_buffer[8] = segment_code[bottom_digit_array[0]][1];

    reset();
    Serial.println("Reset done");
    
    POWER_ON();
    Serial.println("Power on done");
    delay(100);
    
    BOOST();
    Serial.println("Boost done");
    delay(10);
    
    TEMP();
    Serial.println("Temp done");
    delay(10);
    
    FRAMETIME();
    Serial.println("Frame time done");
    delay(10);
    
    DISPLAYDATA();
    Serial.println("Display data done");
    delay(10);

    delay(2);
    
    Wire.beginTransmission(DATA_ADDRESS_EPAPER);
    Wire.write(array_buffer, sizeof(array_buffer));
    Wire.write(0x00);
    Wire.endTransmission();
    
    delay(2);
    STARTREFRESH();
    Serial.println("Starting refresh");
    delay(10);
    
    busy();
    Serial.println("Busy done");
    
    FINISH();
    Serial.println("Finish done");
}

void setup_LCD() {
    pinMode(RST_PIN, OUTPUT);
    pinMode(BUSY_PIN, INPUT);
    
    Serial.begin(9600);
    Wire.begin();
    delay(2000);
    Serial.println("\n***STARTING***\n");
}

