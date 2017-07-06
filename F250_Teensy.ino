
//#include "SPI.h"
//#include "src/Adafruit_GFX/Adafruit_GFX.h"
//#include "src/Adafruit_ILI9340/Adafruit_ILI9340.h"
#include "src/ILI9341_t3/ILI9341_t3.h"                              // PJRC's optimized version of Adafruit's ILI9341 library: https://github.com/PaulStoffregen/ILI9341_t3
#include "src/ILI9341_t3/font_Arial.h"                              // from ILI9341_t3
#define ENCODER_OPTIMIZE_INTERRUPTS                                 // Use the optimized version of encoder since both pins can be used with interrupts. Must be defined before the include. 
#include "src/Encoder/Encoder.h"                                    // PJRC: https://www.pjrc.com/teensy/td_libs_Encoder.html. Only cost to optimized interrupts is you can not use attachInterrupt()
#include "COMMS.h"
#include "src/GraphicDisplayElements/GraphicDisplayElements.h"
#include "src/F250_TeensySettings/F250_TeensySettings.h"
#include "src/F250_TEENSY_EEPROM/F250_TEENSY_EEPROM.h"
#include "src/EEPROMex/EEPROMex.h"  
#include "src/F250_SimpleTimer/F250_SimpleTimer.h"
#include "src/OSL_Button/OSL_Button.h"

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
        int16_t OL                              = 10;               // Offset left
        int16_t OT                              = 20;               // Offset top


    // ROTARY ENCODER
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        Encoder knob(ROT_A, ROT_B);
        long knobPosition =                     -999;


    // MY GRAPHIC DATA ELEMENTS
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        gde displayElement;                                         // Of type gde (graphic display element) - my gde class consructor
        
        int currentScreen                       = 1;                // What screen are we on


    // GLOBAL VARS 
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        boolean GPS_FIX =                       false;

    
    // TESTING ! ! TESTING ! ! TESTING 
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
    elapsedMillis swapTime;


void setup(){

    // Pins
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        // TFT pins are handled by the TFT library, except the backlight: 
        pinMode(TFT_PWM, OUTPUT);                                   // Display backlight
        
        // Encoder pins are handled by the encoder library. If you try to set them to INPUT here the library will not work! 
        
        // Button pin, not actually part of the encoder: 
        pinMode(ROT_C, INPUT_PULLUP);
        
    // Init Serial & Comms
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        Serial.begin(USB_BAUD_RATE); 
        DebugSerial = &Serial;                                      // Use Serial 0 for debugging messages

    // Load EEPROM
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        boolean did_we_init = eeprom.begin();                       // begin() will initialize EEPROM if it never has been before, and load all EEPROM settings into our ramcopy struct
        if (did_we_init && DEBUG) { DebugSerial->println(F("EEPROM Initalized")); }    

    // TFT
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        tft.begin();
        tft.setRotation(tftRotation);
        tft.fillScreen(ILI9341_BLACK);
        
        tft.setFont(Arial_60);                  // See the .h file for the list of sizes
        //tft.setTextSize(14);
        tft.setTextColor(ILI9341_WHITE);
        tft.setCursor(OL, OT);      // x (left), y (top)
        tft.println("65");        

        digitalWrite(TFT_PWM, 500);

    // GRAPHIC DISPLAY ELEMENTS
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        displayElement.setupElement(gde_GPS, RenderGPS);

        displayElement.setDataFlag(gde_GPS);
        GPS_FIX = true;
}






void loop()
{
//    CheckAndFillVars();

    for (int i = 0; i < NUM_ELEMENTS; i++) 
    {
        if (displayElement.hasData(i)) displayElement.renderElement(i);
    }

    if (swapTime > 2000)
    {
        GPS_FIX ? GPS_FIX = false : GPS_FIX = true;
        displayElement.setDataFlag(gde_GPS);
        swapTime = 0;
    }
    
    
    long knobPos;
    knobPos = knob.read() / 4;
    if (knobPos != knobPosition) 
    {
        Serial.print("Knob = ");
        Serial.print(knobPos);
        Serial.println();
        knobPosition = knobPos;
    }

    // if a character is sent from the serial monitor,
    // reset both back to zero.
    if (Serial.available()) 
    {
        Serial.read();
        Serial.println("Reset both knobs to zero");
        knob.write(0);
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

