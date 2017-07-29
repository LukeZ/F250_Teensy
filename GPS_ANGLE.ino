

//=============================================================================================================================================== >> 
// GPS ANGLE
//=============================================================================================================================================== >> 
void RenderAngle()
{
static uint16_t lastAngle;

    switch (currentScreen)
    {
        case SCREEN_SPEED:
            PrintAngleStrip(lastAngle, true);   // True meaning erase
            PrintAngleStrip(Angle, false);      // False meaning write
            lastAngle = Angle;
            
            displayElement.clearDataFlag(gde_Angle);        
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Angle);
            return;
            break;  
    }      
    
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Angle);
}

void PrintAngleStrip(uint16_t angle, boolean erase)
{
int x = 32;             // NO OFFSET FROM OX
int xOffset = 64;
int y = OY + 205;
int lineHeight = 10;
uint16_t color;
uint16_t tickAdjust = 0;
const uint16_t adjustArray[NUM_HEADINGS] = { 338, 315, 293, 270, 248, 225, 203, 180, 158, 135, 113, 90, 68, 45, 23, 0 };

    // We divide 360 degrees into 16 segments of 22.5 each. We take the current angle, compare it to its present 22.5 degree segment, 
    // and return some value from 0-22.5 (not actualy 22.5 since these are ints). We then map this 22 degree range to the 64 pixel
    // range for each tick mark
    for (uint8_t i=0; i<NUM_HEADINGS; i++)
    {
        if (angle > adjustArray[i]) 
        {
            tickAdjust = (angle - adjustArray[i]);
            break;
        }
    }
    tickAdjust = map(tickAdjust, 0, 23, 0, xOffset);

    erase ? color = CurrentBackgroundColor : color = ILI9341_WHITE;
   
    for (uint8_t i=0; i<7; i++)
    {
        tft.drawFastVLine(x - xOffset + (xOffset * i) - tickAdjust, y, lineHeight, color);
    }
    
    tft.drawFastHLine(0, y + lineHeight, 320, color);
}


