/*
||
|| @file GraphicDisplayElements.h
|| @version 1.1
|| @author Luke Middleton
||
|| @description
|| | Attempt to abstract certain parameters of disparate graphic design elements
|| #
||
|| @license
|| | Copyright 2014 Luke Middleton.
|| #
||
|| @changelog
|| | 1.0 2014-11-12 - LM: Initial Release
|| #
*/

#ifndef GDE_H
#define GDE_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// maximum number of elements
const static int NUM_ELEMENTS = 13;

enum elementNames {
	gde_Transmission = 0,
	gde_Temperature,
	gde_Altitude,
	gde_GPS,        // Fix and satellites
    gde_GPS_Coord,  // Coordinates only
	gde_Speed,      // Includes heading
	gde_Radio,
	gde_FuelPump, 
	gde_Air, 
	gde_Voltage, 
    gde_GSM, 
    gde_DateTime,
    gde_Menu
};
	
typedef void (*gde_callback)(void);

// Graphic Display Element (gde) Class
class gde{
  
public:

    // Constructor
    gde();

    // Save the callback function for this element
    int setupElement(int numElement, gde_callback f);

    // This will call the callback for the given element
    void renderElement(int numElement); 

    // returns true if the specified timer is enabled
    bool hasData(int numElement);

    // Set data flag
    void setDataFlag(int numElement);

    // Set all flags
    void setAllDataFlags(void);

    // Clear data flag
    void clearDataFlag(int numElement);


private:
    // pointers to the callback functions
    gde_callback callback[NUM_ELEMENTS];

    // do elements have new data
    boolean newdata[NUM_ELEMENTS];

};



#endif

