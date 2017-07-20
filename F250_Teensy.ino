
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
        boolean DEBUG                           = false;            // Print debugging messages to the PC
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

        // Screen on/off status
        boolean screenOff                       = true;             // 
        #define SCREEN_MODE_OFF                 0
        #define SCREEN_MODE_DAY                 1
        #define SCREEN_MODE_NIGHT               2
        uint8_t screenMode                      = SCREEN_MODE_OFF;

    // ROTARY ENCODER / PUSHBUTTON
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        Encoder knob(ROT_A, ROT_B);
        long knobPosition =                     -999;
        OP_Button InputButton = OP_Button(ROT_C, true, true, 25);   // Initialize a button object. Set pin, internal pullup = true, inverted = true, debounce time = 25 mS
        enum {BUTTON_WAIT, BUTTON_TO_WAIT};
        uint8_t ButtonState                     = BUTTON_WAIT;      // The current button state machine state
        boolean RotarySwap                      = true;             // Swap the direction of movement


    // MY GRAPHIC DATA ELEMENTS
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>
        gde displayElement;                                         // Of type gde (graphic display element) - my gde class constructor
        
        // Brightness
        #define ADJUST_BRIGHTNESS_TIMEOUT       4000                // How long do we stay in adjust brightness mode until we automatically exit
        int TimerID_adjustBrightness            = 0;
        uint8_t BacklightPWM                    = 255;              // What backlight strength to start (0-255)

        // Nighttime text color
        boolean nightTime                       = false;            // Night time means put all text on the screen to green

        // My colors
        uint16_t CurrentBackgroundColor         = ILI9341_BLACK;            
        //#define NightColor                    0x0F96              // 15, 240, 170
        uint16_t NightColor                     = 0x1514;           // 23, 161, 165
        uint8_t Night_R                         = 23;
        uint8_t Night_G                         = 161;
        uint8_t Night_B                         = 165;
        
        #define COLOR_DESELECT                  0x3186              // 51,  51,  51
        #define COLOR_DARK_YELLOW               0xF400              // 241, 128, 0


    // SCREENS
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>    
        #define SCREEN_MENU                     0
        #define SCREEN_MAIN                     1
        #define SCREEN_SPEED                    2
        #define SCREEN_TEMPERATURE              3
        #define SCREEN_ALTITUDE                 4       
        #define SCREEN_MAX_SCREEN               4                   // Number of last in the list
        
        int8_t currentScreen                   = SCREEN_MAIN;       // What screen are we on (SIGNED)

        #define NUM_MENUS                       10
        
        #define MENU_SET_ALT_TO_GPS             0
        #define MENU_SET_ALT                    1
        #define MENU_SET_HOME_COORD             2
        #define MENU_SET_HOME_ALT               3
        #define MENU_SET_TIMEZONE               4
        #define MENU_CLEAR_ALLTIME_TEMP_I       5
        #define MENU_CLEAR_ALLTIME_TEMP_E       6
        #define MENU_CLEAR_ALLTIME_TEMP_A       7
        #define MENU_ADJUST_NIGHT_COLOR         8
        #define MENU_EXIT_MENU                  9

        #define MENU_DEFAULT_MENU               MENU_SET_ALT_TO_GPS 
        
        const char* menuName[NUM_MENUS] = {"Set Alt to GPS", "Set Alt", "Set Home Coord", "Set Home Alt", "Set Timezone", "Clear Int Temps", "Clear Ext Temps", "Clear Aux Temps", "Adjust Night", "Exit Menu"};
        int8_t currentMenu                      = 0;                // What menu item are we on (SIGNED) 
        boolean inMenu                          = false;            // Have we entered the menu screen
        boolean inSelection                     = false;            // Have we entered a menu item

        struct _menu_item{
            boolean enabled;
            boolean entered;
            boolean subMenu_entered;
            boolean val_YN;                     // True if Yes, False if No
            int16_t val_Int;                    // Integer value
            boolean complete;                   // Are we trying to complete the action (aka, hit enter)
            uint8_t cmdToMega;                  // What is the command this menu item sends to the Mega
            uint8_t valueToMega;                // If a value is to be sent
            uint8_t modifierToMega;             // If a modifier is to be sent
            boolean success;                    // Was the action successful (may require a response from the Mega)
        };
        _menu_item Menu[NUM_MENUS];

    // KNOB STATE MACHINE
    //--------------------------------------------------------------------------------------------------------------------------------------------------->>    
        boolean adjustBrightness                = false;            // Are we adjusting the brightness on the screen

        typedef char _KNOB_STATE;                                   // State machine for knob control
        #define KS_ADJUST_BRIGHTNESS            0
        #define KS_CHANGE_SCREEN                1
        #define KS_CHANGE_MENU                  2
        #define KS_ADJUST_VAL                   3
        #define KS_DEFAULT                      KS_CHANGE_SCREEN
        _KNOB_STATE knobState                   = KS_CHANGE_SCREEN;
        
    
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

        // Altitude / Barometric pressure
        int16_t GPS_Altitude                    = 0;
        int16_t Pressure_Altitude               = 0;
        float inHg                              = 0.0;  // Barometric pressure in inches of mercury
        float Pressure                          = 0.0;  // Baromtric pressure in kPa
        boolean UsePressureAltitude             = false;    // Should we display altitude based on the barometer true/false, if false show GPS
        int16_t Home_Altitude                   = 0;    // What is the home altitude. This may be given us by the Mega, or if we set it ourselves we will also know it. 

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
        displayElement.setupElement(gde_Menu, RenderMenu);                  // Menu items
            SetupMenus();

    // Load EEPROM
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        boolean did_we_init = eeprom.begin();                       // begin() will initialize EEPROM if it never has been before, and load all EEPROM settings into our ramcopy struct
        if (did_we_init && DEBUG) { Serial.println(F("EEPROM Initalized")); }    

        // Load night color from EEPROM into RAM
        NightColor = eeprom.ramcopy.NightColor;
        tft.color565toRGB(NightColor, Night_R, Night_G, Night_B);
            
    // TFT
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
        tft.begin();
        nightTime ? setBacklight(eeprom.ramcopy.DimLevel_Night) : setBacklight(eeprom.ramcopy.DimLevel_Day);
        tft.setRotation(tftRotation);

    // Clear inputs
    // -------------------------------------------------------------------------------------------------------------------------------------------------->    
        // The button library will return a wasPressed when we first boot, unless we do two reads farther apart than debounce time. 
        // It can also be useful to give a bit of time so the serial port can init. I've also had issues with the rotary being read and setting the screen
        InputButton.read();
        knob.read();
        delay(100);
        InputButton.read();
        knob.read();
        HandleRotary();
        
    // Set screen explicitly
    // -------------------------------------------------------------------------------------------------------------------------------------------------->          
        ClearScreen(); 
        currentScreen = SCREEN_MAIN;
        UpdateAllElements();
        ShutdownScreen();        // Start with screen off until we are told to turn it on, or until the user pushes the button to force it on. 
        
    // FOR TESTING ! ! ! ! ! ! !        
    // -------------------------------------------------------------------------------------------------------------------------------------------------->          
        StartScreen();
        currentScreen = SCREEN_TEMPERATURE;
}


void loop()
{
    CheckSerial();                                              // Communication from the Mega
    HandleRotary();                                             // Take care of the rotary encoder/push-button
    timer.run();

    if (!screenOff)
    {
        for (int i = 0; i < NUM_ELEMENTS; i++) 
        {
            if (displayElement.hasData(i)) displayElement.renderElement(i);
        }
    }
}




