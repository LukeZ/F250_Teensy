#ifndef F250_TEENSYSETTINGS_H
#define F250_TEENSYSETTINGS_H

// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// SERIAL PORTS
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
    #define USB_BAUD_RATE               115200


// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// SIMPLE TIMER
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
    // We use the F250_SimpleTimer class for convenient timing functions throughout the project, it is a modified and improved version 
    // of SimpleTimer: http://playground.arduino.cc/Code/SimpleTimer
    // The class needs to know how many simultaneous timers may be active at any one time. We don't want this number too low or operation will be eratic, 
    // but setting it too high will waste RAM. Each additional slot costs 19 bytes of global RAM. 

    #define MAX_SIMPLETIMER_SLOTS       20          
    
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// TIME ZONES
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
    #define AKST                            0                   // Alaska timezone - it may use different dates for DST if it even observes it, I don't know. 
    #define PST                             1                   // Pacific standard time
    #define MST                             2                   // Mountain standard time
    #define CST                             3                   // Central standard time
    #define EST                             4                   // Eastern standard time
    #define NUM_TIMEZONES                   5


// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// HOURS OF NIGHT
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
    #define START_NIGHT_HOUR                21                  // What hour do we start night time (screen will go to night mode) - 21 = 9 PM
    #define END_NIGHT_HOUR                  7                   // At the start of this hour night time is considered over - 7 AM

// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// TIME STRUCT
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
    struct _datetime
    {          
        int16_t hour;           // The int16_t variables need to be larger than uint8_t because they may need 
        uint8_t minute;         // to go temporarily negative or otherwise out of bounds while we adjust
        uint8_t second;         // for the UTC offset
        int16_t year;           // 2 digit year only
        int16_t month;
        int16_t day;
        uint8_t timezone;       // Timezone of this date
    };

// ------------------------------------------------------------------------------------------------------------------------------------------------------->>
// SAVED TEMPERATURE STRUCT (saved in EEPROM)
// ------------------------------------------------------------------------------------------------------------------------------------------------------->>    
    struct _saved_tempdata
    {
        int16_t AbsoluteMin;
        _datetime AbsoluteMinTimeStamp;
       int16_t AbsoluteMax; 
       _datetime AbsoluteMaxTimeStamp;
    };

#endif
