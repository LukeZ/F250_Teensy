/*
||
|| @file GraphicDisplayElements.cpp
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
*/


#include "GraphicDisplayElements.h"


gde::gde() 
{
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        newdata[i] = false;
        callback[i] = 0;
    }
}


int gde::setupElement(int numElement, gde_callback f)
{
    //  Only accept element numbers up to the max
    if (numElement >= NUM_ELEMENTS) 
    {
        return -1;
    }
    
    // Save the callback function. Initialize to no new data. 
    callback[numElement] = f;
    newdata[numElement] = false;

    return numElement;
}

void gde::renderElement(int numElement)
{
    // Call the function assigned to this element
    if (callback[numElement]) {
		(*callback[numElement])();
	}
}


bool gde::hasData(int numElement)
{
    if (numElement >= NUM_ELEMENTS) {
        return false;
    }
    return newdata[numElement];   
}


void gde::setDataFlag(int numElement)
{
    if (numElement >= NUM_ELEMENTS) {
        return;
    }

    newdata[numElement] = true;
}

void gde::setAllDataFlags(void)
{
    for (uint8_t i=0; i<NUM_ELEMENTS; i++)
    {
        gde::setDataFlag(i);
    }
}

void gde::clearDataFlag(int numElement)
{
    if (numElement >= NUM_ELEMENTS) {
        return;
    }

    newdata[numElement] = false;
}

