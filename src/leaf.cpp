
#include <mcp_can.h>           // Download code from https://github.com/coryjfowler/MCP_CAN_lib add to Ardunio libraries folder
 
#define VERSION "LeafSOC v58"
#define DATE    "6 Jul 2021"
#define AUTHOR  "Paul Kennett"

#define KM_PER_KWH 6.4F        // km per kWh (from my car) used to calculate Range estimate (6.4 km.kWh is my average for the coldest month of 2019)
#define MAX_GIDS 1             // From my car. LeafSpy showed 225 Gids at 94.8% SOC on 12 Oct 2019.
#define GIDS_TURTLE 8          // the number of Gids at which Turtle mode kicks in (you might achieve 5-7 sometimes).
#define WH_PER_GID 75.0F       // Wh per Gid.  74.73 comes from one of the early Leaf hackinG pioneers. (Some people prefer 80.)
#define CAN0_INT 2             // [Why is this set using a define statement? Why bother?]
#define LINE1 16
#define LINE2 38
#define LINE3 60
#define LINE4 64

long unsigned int rxId;        // CAN message ID
unsigned char len = 0;         // data length
unsigned char rxBuf[8];        // a buffer for CAN data
uint16_t rawGids = 0;          // raw Gids
uint16_t rawGids2 = 0;         // compare with rawGids to see if value has changed
float rawGidsLow;              // raw Gids - 8 (the number of Gids to reach Turtle mode)
float GidsPct;                 // Gids in percentage, This is the main battery SOC % number that I display
uint16_t rawSoc;               // State of charge value directly from the EV-CAN
float SocPct;                  // Leaf SOC% from EV-Can. This corresponds to the LeafSpy app SOC%, but without rounding up
float SocPctSkewed;            // Skewing the top to show "100%" when battery is actually 95% and 0% when battery is 5%.
float kWh;                     // Energy left in main baterys
float km_per_kWh;
byte km_per_dWh = KM_PER_KWH * 10; // using "deca"-Watthour because it's more efficient in EEPROM
byte MaxGids = MAX_GIDS;       // Default to MAX_GIDS value before reading the EEPROM
byte InitialGids = 0;          // Will be set to the first value of Gids read from the Can bus on startup
// byte GidTest = 2;              // temporary value used for comparing Gids in EEPROM to find highest value Gid
int range;                     // My esitmated range
char buffer[4];
int i;
int Page = 1;                  // Screen page; default to Page 1 before reading the EEPROM
byte BootCount = 5;

byte EEPROMaddr0 = 0;          // Address 0 in EEPROM used for the page number
byte EEPROMaddr1 = 1;          // Address 1 in EEPROM used for km_per_kWh
byte EEPROMaddr2 = 2;          // Address 2 in EEPROM used for MaxGids [I should upgrade this to a 2 byte INT at some point]
byte EEPROMaddr4 = 4;          // Boot counter
//                             // EEPROMaddr 5 to EEPROMaddr 95 used for Gids values from start-up (for 60 boots, then cyles around)

byte rawCCStatus1;             // Climate Control, if ON
byte rawCCVentIntake;          // Climate Control, and Intake is RECIRC
byte rawCCVentTarget;          // Climate Control, and Vent is FEET then enter "change settings" modes
byte rawCCFanSpeed;            // used to change page in EEPROM
byte rawCCButtonPress;         // alternates after every button press on the CC
byte rawCCButtonPress2;        // to compare if CCButtonPress has changed
byte rawCCSetpoint;            // used to set default setting in EEPROM
byte rawGearPos = 1;           // used to select which settings option to edit in EEPROM
byte rawGearPos2 = 1;          // preset to Neutral (1)

MCP_CAN CAN0(10);              // Set MCP CS to pin 10



void(* resetFunc) (void) = 0;


void setup() {


  // Run this line once first time with a new micro
  //  Clear_EEPROM();   // clean out Gid space in EEPROM


  CAN0.setMode(MCP_NORMAL);                 // Set to normal mode so the MCP2515 sends acks to received data.
  pinMode(CAN0_INT, INPUT);
  Serial.begin(115200);


} //end setup loop


void loop() {

  // talking to the EV-CAN bus and calculating stuff
  if (!digitalRead(CAN0_INT)) {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);        // Read data: len = data length, buf = data byte(s)

    if ((rxId & 0x40000000) == 0x40000000) {    // Determine if message is a remote request frame.
      // sprintf(msgString, " REMOTE REQUEST FRAME");
    } else {
      // Pick out the CAN messages we're interested in
      if (rxId == 0x5bc) {            // read GIDS from EV-CAN (500ms)
        rawGids = (rxBuf[0] << 2) | (rxBuf[1] >> 6);
        GidsPct = ((float)(rawGids - GIDS_TURTLE) / (MaxGids - GIDS_TURTLE)) * 100.0F;
        kWh = (((float)rawGids) * WH_PER_GID) / 1000.0F;
        range = km_per_kWh * ((rawGids - GIDS_TURTLE) * WH_PER_GID) / 1000;
      } else if (rxId == 0x54b) {     // Read Climate Controls from EV-Can (100ms)
        rawCCStatus1 = (rxBuf[0]);
        rawCCVentTarget = (rxBuf[2]);
        rawCCVentIntake  = (rxBuf[3]);
        rawCCFanSpeed = (rxBuf[4] >> 3);
        rawCCButtonPress = (rxBuf[7]);
      } else if (rxId == 0x55b) {     // read SOC from EV-CAN (100ms)
        rawSoc = (rxBuf[0] << 2) | (rxBuf[1] >> 6);
        SocPct = rawSoc / 10.0F;
        SocPctSkewed = (SocPct - 5.0F) * 1.1F; // adjusting so that it goes from 0 to 100%
      } else if (rxId == 0x11a) {     // Read Gear Position from EV-Can (10ms)
        rawGearPos = (rxBuf[0] >> 4); // 1 = Park, 2 = Reverse, 3 = Neutral, 4 = Drive
      } else if (rxId == 0x54a) {     // Read Climate Control Setpoint (target temp) from EV-Can (100ms)
        rawCCSetpoint = (rxBuf[4]);   // 18.0C = 36, 18.5C = 37 ... 31.5.0C = 63, 32.0C = 64
      }
    }
  }

  Serial.println("SocPctSkewed: " + String(SocPctSkewed));

}
