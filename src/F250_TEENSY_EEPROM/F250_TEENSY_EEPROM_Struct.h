#ifndef F250_TEENSY_EEPROM_STRUCT_H
#define F250_TEENSY_EEPROM_STRUCT_H

// EEPROM DATA STRUCTURE
//--------------------------------------------------------------------------------------------------------------------------------------->>
// This defines every variable held in EEPROM
//--------------------------------------------------------------------------------------------------------------------------------------->>

#include "../F250_TeensySettings/F250_TeensySettings.h"     

struct _eeprom_data 
{

// OneWire temp sensor addresses
    byte TempAddress_Int[8];
    byte TempAddress_Ext[8];
    byte TempAddress_Aux[8];

// Time stuff
    uint8_t Timezone;

// Temp stuff
    _saved_tempdata SavedInternalTemp;
    _saved_tempdata SavedExternalTemp;
    _saved_tempdata SavedAuxTemp;

// Altitude stuff
    _datetime lastAltitudeAdjust;       // When was the last adjustment made. 

// GPS stuff
    float Lat_Home;     // Coordinates of home location
    float Lon_Home;
    int16_t Alt_Home;   // Home altitude, will use to adjust barometric pressure on startup

// Night-time color
    uint16_t NightColor;

// Marker
    uint32_t InitStamp;                          
};


#endif  // Define F250_TEENSY_EEPROM_STRUCT_H