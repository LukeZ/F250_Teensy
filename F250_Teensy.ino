
//#include "SPI.h"
//#include "src/Adafruit_GFX/Adafruit_GFX.h"
//#include "src/Adafruit_ILI9340/Adafruit_ILI9340.h"
#include "src/ILI9341_t3/ILI9341_t3.h"                              // PJRC's optimized version of Adafruit's ILI9341 library: https://github.com/PaulStoffregen/ILI9341_t3
#include "src/ILI9341_t3/font_Arial.h"                              // from ILI9341_t3
#include "src/ILI9341_t3/font_ArialBold.h"                          // from ILI9341_t3
#define ENCODER_OPTIMIZE_INTERRUPTS                                 // Use the optimized version of encoder since both pins can be used with interrupts. Must be defined before the include. 
#include "src/Encoder/Encoder.h"                                    // PJRC: https://www.pjrc.com/teensy/td_libs_Encoder.html. Only cost to optimized interrupts is you can not use attachInterrupt()
#include "COMMS.h"
#include "src/GraphicDisplayElements/GraphicDisplayElements.h"
#include "src/F250_TeensySettings/F250_TeensySettings.h"
#include "src/F250_TEENSY_EEPROM/F250_TEENSY_EEPROM.h"
#include "src/EEPROMex/EEPROMex.h"  
#include "src/F250_SimpleTimer/F250_SimpleTimer.h"
#include "src/OP_Button/OP_Button.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif


// GLOBAL VARIABLES
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------>>
    // USEFUL NAMES
        const byte On                           = 1;
        const byte Off                          = 0;

    // SERIAL   
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        usb_serial_class                        *DebugSerial;       // Which serial port to print debug messages to (HardwareSerial is equal to Serial0/Serial Port 0/USART0)
        boolean DEBUG                           = true;             // Print debugging messages to the PC
        #define MegaSerial                      Serial1             // What serial port are we communicating with the Mega on

        #define SENTENCE_BYTES                  5                   // How many bytes in a valid sentence. 
        struct DataSentence {                                       // Serial commands should have four bytes (plus termination). 
            uint8_t    Address                  = 0;                // We use a struct for convenience
            uint8_t    Command                  = 0;
            uint8_t    Value                    = 0;
            uint8_t    Modifier                 = 0;
            uint8_t    Checksum                 = 0;
        };
        
    // PROJECT SPECIFIC EEPROM
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        F250_TEENSY_EEPROM eeprom;                                  // Wrapper class for dealing with eeprom. Note that EEPROM is also a valid object, it is the name of the EEPROMex class instance. Use the correct one!
                                                                    // F250_TEENSY_EEPROM basically provides some further functionality beyond EEPROMex. 
    // SIMPLE TIMER 
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        F250_SimpleTimer timer;                                     // SimpleTimer named "timer"
        boolean TimeUp                          = true;

    // PINS
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        // UNUSED                                                   // A0    
        // UNUSED                                                   // A1    
        // UNUSED                                                   // A2
        // UNUSED                                                   // A3
        // UNUSED                                                   // A4    
        // UNUSED                                                   // A5    
        // RESERVED                             0                   // D0    Input  - This is the serial RX pin
        // RESERVED                             1                   // D1    Output - This is the serial TX pin
        #define     ROT_A                       2                   // D2    Input  - Rotary encoder "A"
        #define     ROT_B                       3                   // D3    Input  - Rotary encoder "B"
        #define     ROT_C                       4                   // D4    Input  - Pushbutton switch (ground when pressed)
        //UNUSED                                5                   // D5    unused
        #define     TFT_PWM                     6                   // D6    Output - backlight PWM value
        #define     _sd_cs                      7                   // D7    Output - SD CS is not really needed since I am not accessing the SD card
        #define     _rst                        8                   // D8    Output - TFT reset
        #define     _dc                         9                   // D9    Output - DC
        #define     _cs                         10                  // D10   Output - Called "SS" on my board, this is the TFT CS (chip select)
        #define     _mosi                       11                  // D11   Output - data out
        #define     _miso                       12                  // D12   Input  - Not really needed, this is data back from TFT (not TO TFT)
        #define     _sclk                       13                  // D13   Outupt - signal clock


    // TFT DISPLAY
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        // Use hardware SPI - if you don't submit miso/mosi/sck to the object it will use hardware SPI, which however are the same as the pins as I've defined them above
            ILI9341_t3 tft = ILI9341_t3(_cs, _dc, _rst); //, _mosi, _sclk, _miso);

        // Screen rotation - can be 0-4
            #define tftRotation 3

        // Top-left offset
        int16_t OX                              = 5;                // Offset left
        int16_t OY                              = 15;               // Offset top


    // ROTARY ENCODER / PUSHBUTTON
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        Encoder knob(ROT_A, ROT_B);
        long knobPosition =                     -999;
        OP_Button InputButton = OP_Button(ROT_C, true, true, 25);   // Initialize a button object. Set pin, internal pullup = true, inverted = true, debounce time = 25 mS
        enum {BUTTON_WAIT, BUTTON_TO_WAIT};
        uint8_t ButtonState              = BUTTON_WAIT;             //The current button state machine state

    // MY GRAPHIC DATA ELEMENTS
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        gde displayElement;                                         // Of type gde (graphic display element) - my gde class constructor

        #define SCREEN_MENU                     0
        #define SCREEN_AUTO                     1
        #define SCREEN_SPEED                    2
        #define SCREEN_TEMP                     3
        uint8_t currentScreen                   = SCREEN_AUTO;      // What screen are we on
        
        // Brightness
        #define ADJUST_BRIGHTNESS_TIMEOUT       4000                // How long do we stay in adjust brightness mode until we automatically exit
        int TimerID_adjustBrightness            = 0;
        uint8_t BacklightPWM                    = 255;              // What backlight strength to start (0-255)
        boolean adjustBrightness                = false;            // Are we adjusting the brightness on the screen

        // Nighttime text color
        boolean nightTime                       = false;            // Night time means put all text on the screen to green

        // My colors
        uint16_t CurrentBackgroundColor         = ILI9341_BLACK;            
        //#define TEXT_COLOR_NIGHT              0x0F96              // 15, 240, 170
        #define TEXT_COLOR_NIGHT                0x1514              // 23, 161, 165
        #define COLOR_DESELECT                  0x3186              // 51,  51,  51
        #define COLOR_DARK_YELLOW               0xF400              // 241, 128, 0

    // GLOBAL VARS - TO STORE INCOMING DATA
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        boolean GPS_Fix                         = false;
        uint8_t GPS_FixQuality                  = 0;
        uint8_t GPS_NumSatellites               = 0;

        // Temperature
        typedef char _TEMP_SENSOR;                                  // Convenient names for our sensors
        #define TS_INTERNAL                     0
        #define TS_EXTERNAL                     1
        #define TS_AUX                          2
        #define NUM_TEMP_SENSORS                3
        // Temperature sensor data for each individual sensor
        struct _tempsensor{                             
            boolean sensorPresent;                      // Is the sensor attached
            boolean lastSensorPresent;                  // 
            int16_t currentTemp;                        // Integer value between -255 and 255
            int16_t priorTemp;                          // Last reading since change
            int16_t sessionMinTemp;                     // Min temp of this session
            int16_t priorSessionMinTemp;                // 
            int16_t sessionMaxTemp;                     // Max temp of this session
            int16_t priorSessionMaxTemp;                // 
            int16_t allTimeMaxTemp;                     // Max temp forever
            _datetime allTimeMaxDT;                     // Max temp forever timestamp
            int16_t allTimeMinTemp;                     // Min temp forever
            _datetime allTimeMinDT;                     // Min temp forever timestamp
            boolean updateMinDT_Flag;                   // If true, the next timestamp that comes through is the date of the all time minimum temperature
            boolean updateMaxDT_Flag;                   // If true, the next timestamp that comes through is the date of the all time maximum temperature
            _TEMP_SENSOR sensorName;                    // These will get assigned in InitTempStructs()
        };
        _tempsensor InternalTemp;
        _tempsensor ExternalTemp;
        _tempsensor AuxTemp;

        // Altitude
        int16_t GPS_Altitude                    = 0;
        int16_t Barometric_Altitude             = 0;

        // Voltage
        float Voltage                           = 0;
        boolean lowVoltage                      = false;// If true, we will change the font color and start blinking. 
        #define LOW_VOLTAGE                     124     // According to Optima, Red and Blue (regular) should read 12.6-12.8 at rest. Yellow (what I have) should be 13-13.2. 
                                                        // Engine running should be anywhere above 13.3 all the way up to 15
                                                        // https://www.optimabatteries.com/en-us/support/charging/recommended-battery-voltage
                                                        // I set this to 12.4 but instead of doing a float (which has problems with comparison), I set it to the pre-convert value
        
        // Date/time
        _datetime DT;

        // Fuel pump
        boolean FuelPump                        = false;

        // Low air warning
        boolean LowAirWarning                   = false;

        // Ham/CB
        boolean Ham_On                          = false;

        // GSM
        uint8_t GSM_Bars                        = 0;

        // Speed/direction
        uint8_t Speed                           = 0; 
        uint16_t Angle                          = 0;    // 0-360 degrees
        uint8_t Heading                         = 0;    // N, S, E, W, etc...

        // Transmission
        typedef char _TQC_STATE;                                     // Torque converted locked status
        #define TQC_AUTO                        0
        #define TQC_FORCE_LOCK                  1
        #define TQC_FORCE_UNLOCK                2
        #define TQC_UNKNOWN                     3                    // Not really used other than to force a message on startup
        _TQC_STATE TQCLockStatus                = TQC_UNKNOWN;

        boolean OverdriveEnabled                = false;
        uint8_t BaumannTable                    = 0;                // Should be either 1 or 2
        
    
    // TESTING ! ! TESTING ! ! TESTING 
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
    elapsedMillis testTime;


void setup()
{

    // Pins
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        // TFT pins are handled by the TFT library, except the backlight: 
        pinMode(TFT_PWM, OUTPUT);                                   // Display backlight
        
        // Encoder pins are handled by the encoder library. If you try to set them to INPUT here the library will not work! 
        // Pushbutton pin is handled by Button library
       
    // Init Serial & Comms
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        Serial.begin(USB_BAUD_RATE); 
        MegaSerial.begin(USB_BAUD_RATE);                            // Serial used to communicate with the Mega

    // Wait
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        // The button library will return a wasPressed when we first boot, unless we do two reads farther apart than debounce time. 
        // It can also be useful to give a bit of time so the serial port can init
        InputButton.read();
        delay(100);
        InputButton.read();

    // Load EEPROM
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        boolean did_we_init = eeprom.begin();                       // begin() will initialize EEPROM if it never has been before, and load all EEPROM settings into our ramcopy struct
        if (did_we_init && DEBUG) { Serial.println(F("EEPROM Initalized")); }    

    // TFT
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        tft.begin();
        FullBrightness();
        tft.setRotation(tftRotation);
        tft.fillScreen(CurrentBackgroundColor);

        tft.setFontAdafruit();                      // To go back to default font
        tft.setTextSize(6);                         // Then use this to set text size, instead of specifying the size in the custom font name.
        
//        tft.setFont(Arial_60);                  // See the .h file for the list of sizes
//        tft.setTextColor(ILI9341_WHITE);
//        tft.setCursor(OX, OY);      // x (left), y (top)
//        tft.println("65");        



    // GRAPHIC DISPLAY ELEMENTS
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        displayElement.setupElement(gde_GPS, RenderGPS);                    // GPS display
        displayElement.setupElement(gde_Temperature, RenderTemperature);    // Temperature display
        displayElement.setupElement(gde_GSM, RenderGSM);                    // GSM cell phone
        displayElement.setupElement(gde_Voltage, RenderVoltage);            // Voltage
        displayElement.setupElement(gde_Speed, RenderSpeed);                // Speed
        displayElement.setupElement(gde_DateTime, RenderDateTime);          // Date/Time
            DT = getBlankDateTime();                                        // Initialize DT
        displayElement.setupElement(gde_FuelPump, RenderFuelPump);          // Fuel pump
        displayElement.setupElement(gde_Altitude, RenderAltitude);          // Altitude
        displayElement.setupElement(gde_Air, RenderLowAirWarn);             // Low air warning
        displayElement.setupElement(gde_Radio, RenderHamCB);                // Ham/CB
        displayElement.setupElement(gde_Transmission, RenderTransmission);  // Transmission settings

        UpdateAllElements();                                                // Update everything to start
/*
    gde_Alarm, 
    gde_Ign,
*/
}



void loop()
{
//    CheckAndFillVars();

//    static uint32_t     TimeLastSerial = 0;                     // Time of last received serial data

//    Serial.println(adjustBrightness);

    CheckSerial();                                              // Communication from the Mega
    HandleRotary();                                             // Take care of the rotary encoder/push-button
    timer.run();

    for (int i = 0; i < NUM_ELEMENTS; i++) 
    {
        if (displayElement.hasData(i)) displayElement.renderElement(i);
    }


}


void CheckAndFillVars()
{



//    if (ETin.receiveData())
//    {
/*
        // digitalWrite(13, rxdata.buttonstate);
        // myservo.write(map(rxdata.servoval, 0, 1023, 0, 179));

        // HERE WE MUST CHECK EACH AND EVERY VARIABLE FOR CHANGES

        // GPS_FIX
        //----------------------------------------------------------------------------------------->>
        if (GPS_FIX != rxdata.GPS_FIX)
        {
            GPS_FIX = rxdata.GPS_FIX;
            // This will affect all elements related to the GPS
            displayElement.setDataFlag(gde_GPS);
            displayElement.setDataFlag(gde_Speed);
            displayElement.setDataFlag(gde_Heading);
        }

        // TorqueConverter
        //----------------------------------------------------------------------------------------->>
        if (TorqueConverter != rxdata.TorqueConverter)
        {
            TorqueConverter = rxdata.TorqueConverter;
            displayElement.setDataFlag(gde_Transmission);
        }

        // Overdrive
        //----------------------------------------------------------------------------------------->>
        if (Overdrive != rxdata.Overdrive)
        {
            Overdrive = rxdata.Overdrive;
            displayElement.setDataFlag(gde_Transmission);
        }

        // CB_SEL
        //----------------------------------------------------------------------------------------->>
        if (CB_SEL != rxdata.CB_SEL)
        {
            CB_SEL = rxdata.CB_SEL;
            displayElement.setDataFlag(gde_Radio);
        }

        // FuelPump
        //----------------------------------------------------------------------------------------->>
        if (FuelPump != rxdata.FuelPump)
        {
            FuelPump = rxdata.FuelPump;
            displayElement.setDataFlag(gde_FuelPump);
        }
            
        // BaumannTable2
        //----------------------------------------------------------------------------------------->>
        if (BaumannTable2 != rxdata.BaumannTable2)
        {
            BaumannTable2 = rxdata.BaumannTable2;
            displayElement.setDataFlag(gde_Transmission);
        }

        // BaumannSS1
        //----------------------------------------------------------------------------------------->>
        if (BaumannSS1 != rxdata.BaumannSS1)
        {
            BaumannSS1 = rxdata.BaumannSS1;
            displayElement.setDataFlag(gde_Transmission);
        }

        // ViperArmed
        //----------------------------------------------------------------------------------------->>
        if (ViperArmed != rxdata.ViperArmed)
        {
            ViperArmed = rxdata.ViperArmed;
            displayElement.setDataFlag(gde_Alarm);
        }

        // ViperTripped
        //----------------------------------------------------------------------------------------->>
        if (ViperTripped != rxdata.ViperTripped)
        {
            ViperTripped = rxdata.ViperTripped;
            displayElement.setDataFlag(gde_Alarm);
        }

        // AirWarning
        //----------------------------------------------------------------------------------------->>
        if (AirWarning != rxdata.AirWarning)
        {
            AirWarning = rxdata.AirWarning;
            displayElement.setDataFlag(gde_Air);
        }

        // IndoorTemp & OutdoorTemp
        //----------------------------------------------------------------------------------------->>
        if ((IndoorTemp != rxdata.IndoorTemp) || (OutdoorTemp != rxdata.OutdoorTemp))
        {
            IndoorTemp = rxdata.IndoorTemp;
            OutdoorTemp = rxdata.OutdoorTemp;
            displayElement.setDataFlag(gde_Temperature);
        }

        // Altimeter
        //----------------------------------------------------------------------------------------->>
        if (Altimeter != rxdata.Altimeter)
        {
            Altimeter = rxdata.Altimeter;
            displayElement.setDataFlag(gde_Altitude);
        }

        // Speedometer
        //----------------------------------------------------------------------------------------->>
        if (Speedometer != rxdata.Speedometer)
        {
            Speedometer = rxdata.Speedometer;
            displayElement.setDataFlag(gde_Speed);
        }

        // Voltage
        //----------------------------------------------------------------------------------------->>
        if (Voltage != rxdata.Voltage)
        {
            Voltage = rxdata.Voltage;
            displayElement.setDataFlag(gde_Voltage);
        }
*/
//    }

    return;
}





/*
    //first, lets read our button and store it in our data structure
    if(!digitalRead(12))
        txdata.buttonstate = HIGH;
    else
        txdata.buttonstate = LOW;
    //then we will go ahead and send that data out
        ETout.sendData();
*/
  

/*
int x = 0;
int ditlength = 25;
int readout = 125;
int hundreds = 0;
int tens = 0;
int ones = 0;

typedef void (* displayElementPtr) (); // this is a typedef to my displayElement functions
*/


//the following declares an arry of 10 function pointers of type DigitFuncPtr 
/*
displayElementPtr dElement[10] = {zero,one,two,three,four,five,six,seven,eight,nine};

void zero(void) {dash(); dash(); dash(); dash(); dash(); pause();};
void one(void) { dit(); dash(); dash(); dash(); dash(); pause();};
void two(void) {dit(); dit(); dash(); dash(); dash(); pause();};
void three(void) {dit(); dit(); dit(); dash(); dash(); pause();};
void four(void) {dit(); dit(); dit(); dit(); dash(); pause();};
void five(void) {dit(); dit(); dit(); dit(); dit(); pause();};
void six(void) {dash(); dit();dit();dit();dit(); pause();};
void seven(void) {dash(); dash(); dit(); dit(); dit(); pause();};
void eight(void) {dash(); dash(); dash(); dit(); dit(); pause();};
void nine(void) {dash(); dash(); dash(); dash(); dit(); pause();}; 
void dit() {
   Serial.print(".");
}
void dash() {
 Serial.print("-");
}
void pause(){
 Serial.println("" );
}
void loop() {
for(int i = 0; i < 10; i ++){
 Serial.print(i);
 Serial.print("= ");
// numeral[i]();   // this calls each of the 10 digit functions
}
 Serial.println(readout);
 hundreds = int(readout/100);
 tens = int((readout - (hundreds*100))/10);
 ones = int(((readout - (hundreds*100)) - (tens*10)));
// numeral[hundreds]();
// numeral[tens]();
// numeral[ones]();
 Serial.println("");
 delay(1000);
}
*/

