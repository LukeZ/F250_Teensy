
// INCLUDES
// -------------------------------------------------------------------------------------------------------------------------------------------------->
    #include "src/GraphicDisplayElements/GraphicDisplayElements.h"
    #include "SPI.h"
    #include "Adafruit_GFX.h"
    #include "Adafruit_ILI9340.h"
    #include <EasyTransfer.h>

    #if defined(__SAM3X8E__)
        #undef __FlashStringHelper::F(string_literal)
        #define F(string_literal) string_literal
    #endif


// DEFINES
// -------------------------------------------------------------------------------------------------------------------------------------------------->
    // EEPROM defines
            #define eeprom_read_to(dst_p, eeprom_field, dst_size) eeprom_read_block(dst_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(dst_size, sizeof((__eeprom_data*)0)->eeprom_field))
            #define eeprom_read(dst, eeprom_field) eeprom_read_to(&dst, eeprom_field, sizeof(dst))
            #define eeprom_write_from(src_p, eeprom_field, src_size) eeprom_write_block(src_p, (void *)offsetof(__eeprom_data, eeprom_field), MIN(src_size, sizeof((__eeprom_data*)0)->eeprom_field))
            #define eeprom_write(src, eeprom_field) { typeof(src) x = src; eeprom_write_from(&x, eeprom_field, sizeof(x)); }

        // Useful functions
            #define MIN(x,y) ( x > y ? y : x )

        // Useful names 
            const byte On = 1;                          
            const byte ON = 1;
            const byte Off = 0;
            const byte OFF = 0;
            const byte Yes = 1;
            const byte YES = 1;
            const byte No = 0;
            const byte NO = 0;
            const byte SET = 1;
            const byte Set = 1;
            const byte CLEARED = 0;
            const byte Cleared = 0;
            const byte PRESSED = 0;                // Used for buttons pulled-up to Vcc, who are tied to ground when pressed
            const byte RELEASED = 1;               // Used for buttons pulled-up to Vcc, who are tied to ground when pressed
            
            boolean DEBUG = false;


    // SERIAL
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        #define BaudRate     9600      // Set baud rate here if you know what you're doing and don't like the default value


    // PINS
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        // Analog Pins - None used                  // A0    
                                                    // A1    
                                                    // A2
                                                    // A3
                                                    // A4    
                                                    // A5    
        // Digital Pins
            // RESERVED                    0        // D0    Input  - This is the serial RX pin
            // RESERVED                    1        // D1    Output - This is the serial TX pin
            #define     ROT_A              2        // D2    Input  - Rotary encoder "A"
            #define     ROT_B              3        // D3    Input  - Rotary encoder "B"
            #define     ROT_C              4        // D4    Input  - Pushbutton switch (ground when pressed)
            //UNUSED                       5        // D5    unused
            #define     TFT_PWM            6        // D6    Output - backlight PWM value
            #define     _sd_cs             7        // D7    Output - SD CS is not really needed since I am not accessing the SD card
            #define     _dc                8        // D8    ?
            #define     _rst               9        // D9    Output - TFT reset
//            #define     _dc                9        // D9    TEENSY VERSION
//            #define     _cs               10        // D10   TEENSY VERSINO
            #define     _cs               10        // D10   Output - Called "SS" on my board, this is the TFT CS (chip select)
            #define     _mosi             11        // D11   Output - data out
            #define     _miso             12        // D12   Input  - Not really needed, this is data back from TFT (not TO TFT)
            #define     _sclk             13        // D13   Outupt - signal clock



    // SIMPLE TIMER 
    //--------------------------------------------------------------------------------------------------------------------------------------------------->


    // TFT DISPLAY
    //--------------------------------------------------------------------------------------------------------------------------------------------------->
        // Using software SPI is really not suggested, its incredibly slow, but you can use it for testing
            // Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
        // Use hardware SPI
            Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

        // Screen rotation - can be 0-4
            #define tftRotation 3


    // MY GRAPHIC DATA ELEMENTS
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        gde displayElement;           // Of type gde (graphic display element) - my gde class consructor
        
        int currentScreen = 1;        // What screen are we on




    // EEPROM
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        // Change this any time the EEPROM content changes
            const long EEPROM_Init = 0xAA44;
        
        // __eeprom_data is the structure that maps all of the data we are storing in EEPROM
            struct __eeprom_data {
              long E_InitNum;                     // Number that indicates if EEPROM values have ever been initialized 
              int E_ThrottlePulseMin;
              int E_ThrottlePulseMax;
              int E_ThrottlePulseCenter;
              boolean E_ThrottleChannelReverse;
            };


    // EASY TRANSFER
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        EasyTransfer ETin;
        EasyTransfer ETout; 

        // Because I want to know if these have changed since our last reception, I actually make two copies. One loose, the other part of rxData. 
        // You should actually just make two structures. 
        bool GPS_FIX;           // Does the GPS have a fix or not
        bool TorqueConverter;   // Torque converter, true = locked, false = unlocked
        bool Overdrive;         // True = overdrive enabled, false = overdrive disabled (not the same as whether we are IN overdrive presently)
        bool CB_SEL;            // True = mic set to CB, False = mic set to Ham
        bool FuelPump;          // True = electric fuel pump is on, false = electric fuel pump is off
        bool BaumannTable2;     // True = we are in the alternate table selection
        bool BaumannSS1;        // Don't remember, something about Baumann
        bool ViperArmed;        // Is the alarm presently armed
        bool ViperTripped;      // Is the alarm presently tripped
        bool AirWarning;        // True = low air warning from air tank sensor

        unsigned int IndoorTemp;   // Indoor temp in degrees F, integer
        unsigned int OutdoorTemp;  // Outdoor temp in degrees F, integer
        unsigned int Altimeter;    // Altitude in feet, integer
        
        unsigned int Speedometer;  // Speed in miles per hour, integer          

        double Voltage;            // Battery voltage to 2 decimal places

        struct RECEIVE_DATA_STRUCTURE{
            //put your variable definitions here for the data you want to receive
            //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO

            bool GPS_FIX;           // Does the GPS have a fix or not
            bool TorqueConverter;   // Torque converter, true = locked, false = unlocked
            bool Overdrive;         // True = overdrive enabled, false = overdrive disabled (not the same as whether we are IN overdrive presently)
            bool CB_SEL;            // True = mic set to CB, False = mic set to Ham
            bool FuelPump;          // True = electric fuel pump is on, false = electric fuel pump is off
            bool BaumannTable2;     // True = we are in the alternate table selection
            bool BaumannSS1;        // Don't remember, something about Baumann
            bool ViperArmed;        // Is the alarm presently armed
            bool ViperTripped;      // Is the alarm presently tripped
            bool AirWarning;        // True = low air warning from air tank sensor

            unsigned int IndoorTemp;   // Indoor temp in degrees F, integer
            unsigned int OutdoorTemp;  // Outdoor temp in degrees F, integer
            unsigned int Altimeter;    // Altitude in feet, integer
            
            unsigned int Speedometer;  // Speed in miles per hour, integer          

            double Voltage;            // Battery voltage to 2 decimal places
        };

        struct SEND_DATA_STRUCTURE{
            //put your variable definitions here for the data you want to receive
            //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
            int buttonstate;
        };


        //give a name to the group of data
            RECEIVE_DATA_STRUCTURE rxdata;
            SEND_DATA_STRUCTURE txdata;



void setup(){

    // SERIAL
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        Serial.begin(BaudRate);  

    // TFT
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        tft.begin();
        tft.setRotation(tftRotation);
        tft.fillScreen(ILI9340_BLACK);

        tft.setTextSize(14);
        tft.setTextColor(ILI9340_WHITE);
        tft.println("65");        


    // PINS
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        // TFT pins are handled by the TFT librar
        pinMode(ROT_A, INPUT_PULLUP);             // Set these pins to input, with internal pullup resistors enabled
        pinMode(ROT_B, INPUT_PULLUP);
        pinMode(ROT_C, INPUT_PULLUP);

    // EASY TRANSFER
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
        ETin.begin(details(rxdata), &Serial);    // Set to Serial 1, which is the only hardware serial we have on this board. 
        ETout.begin(details(txdata), &Serial);

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

    delay(2000);
    GPS_FIX ? GPS_FIX = false : GPS_FIX = true;
    displayElement.setDataFlag(gde_GPS);

}


void CheckAndFillVars()
{
    if (ETin.receiveData())
    {
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
    }

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

