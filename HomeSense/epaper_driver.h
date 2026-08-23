// WAVESHARE 1.9-INCH SEGMENT E-PAPER Driver

// PIN DEFINITIONS
#define RST_PIN 8
#define BUSY_PIN 7

// ADDRESS DEFINITIONS
#define CMD_ADDRESS_EPAPER 0x3C
#define DATA_ADDRESS_EPAPER 0x3D

// COMMAND SEQUENCE for POWER ON
#define POWER_ON_CMD 0x2B

// FUNCTIONS 
void reset(); // e-paper controller goes back to known starting state
void busy(); // Waits if LCD is updating/refreshing
void send_cmd(uint8_t cmd);
void send_data(uint8_t data);


void POWER_ON(); // Command Sequence to power on the e-paper display
void BOOST(); // Command Sequence to turn on High Voltage for the e-paper display
void TEMP(); // Command Sequence to configure temperature for the e-paper display
void FRAMETIME(); // Command Sequence to set frame time for the e-paper display
void DISPLAYDATA(); // Command Sequence to configure display data on the e-paper display
void STARTREFRESH(); // Command Sequence to start refreshing the e-paper display (Updating e-paper particles)
void FINISH(); // Command Sequence to Power off and turn High Voltage off the e-paper display.


void display(int num = 0, int bottom_num = 0);
void setup_LCD();