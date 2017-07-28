

//=============================================================================================================================================== >> 
// GPS SPEED 
//=============================================================================================================================================== >> 
void RenderSpeed()
{
int xS = 0;     // Speed location
int yS = 0;  
int xH = 0;     // Heading location
int yH = 0;     
#define COLOR_SPEED_TEXT    ILI9341_WHITE
uint16_t color; 
static uint8_t displaySpeed = 0;
static uint8_t lastSpeed;
static int8_t lastHeading;  
static uint16_t lastAngle;

    switch (currentScreen)
    {
        case SCREEN_MAIN:
            xS = OX + 122;      // Speed
            yS = OY + 112;
            xH = OX + 222;      // Heading
            yH = OY + 162;

            // Color
            nightTime ? color = NightColor : color = COLOR_SPEED_TEXT;

            if (displaySpeed < Speed) 
            {
                displaySpeed += 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }
            else if (displaySpeed > Speed) 
            {
                displaySpeed -= 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }   
            else
            {
                displaySpeed = Speed;
                if (timer.isEnabled(TimerID_SpeedUpdate)) timer.disable(TimerID_SpeedUpdate);   // Disable the update timer
            }
            
            // Speed
            tft.setFont(Arial_48_Bold);                  
            // Overwrite prior
            tft.setTextColor(CurrentBackgroundColor);    
            (lastSpeed < 10) ? tft.setCursor(xS + 19, yS): tft.setCursor(xS, yS);
            tft.print(lastSpeed);
                // Write current
                tft.setTextColor(color);
                (displaySpeed < 10) ? tft.setCursor(xS + 19, yS): tft.setCursor(xS, yS);
                tft.print(displaySpeed);
                // Save current display to last
                lastSpeed = displaySpeed;
            
            // MPH text - doesn't change, don't need to pre-write background color
            tft.setFont(Arial_20_Bold);
            tft.setCursor(xS + 5, yS + 58);
            tft.print("MPH");
        
            // Heading - but only if speed is positive
            if (Speed > 0)
            {
                tft.setFont(Arial_24_Bold);
                // Overwrite prior
                tft.setTextColor(CurrentBackgroundColor);
                tft.setCursor(xH + returnHeadingXOffset(lastHeading), yH);
                tftPrintHeading(lastHeading);
                    // Write current
                    tft.setTextColor(color);
                    tft.setCursor(xH + returnHeadingXOffset(Heading), yH);
                    tftPrintHeading(Heading);
                    // Save current to last
                    lastHeading = Heading;
            }            
            break;

        case SCREEN_SPEED:
            xS = OX + 80;      // Speed
            yS = OY + 40;

            // Color
            nightTime ? color = NightColor : color = COLOR_SPEED_TEXT;

            if (displaySpeed < Speed) 
            {
                displaySpeed += 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }
            else if (displaySpeed > Speed) 
            {
                displaySpeed -= 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }   
            else
            {
                displaySpeed = Speed;
                if (timer.isEnabled(TimerID_SpeedUpdate)) timer.disable(TimerID_SpeedUpdate);   // Disable the update timer
            }
            
            // Speed
            tft.setFont(Arial_96_Bold);                  
            // Overwrite prior
            tft.setTextColor(CurrentBackgroundColor);    
            (lastSpeed < 10) ? tft.setCursor(xS + 19, yS): tft.setCursor(xS, yS);
            tft.print(lastSpeed);
                // Write current
                tft.setTextColor(color);
                (displaySpeed < 10) ? tft.setCursor(xS + 19, yS): tft.setCursor(xS, yS);
                tft.print(displaySpeed);
                // Save current display to last
                lastSpeed = displaySpeed;
        
            
            // Heading
            // -----------------------------------------------------------------------------------------------------
            
            // What I want to do is print 5 headings, the actual one plus two to either side in smaller font
            PrintHeadingStrip(lastHeading, lastAngle, true);   // True meaning erase
            PrintHeadingStrip(Heading, Angle, false);      // False meaning write
            lastHeading = Heading;
            lastAngle = Angle;
            
            displayElement.clearDataFlag(gde_Speed);        
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Speed);
            return;
            break;  
    }      


        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Speed);
}

void PrintHeadingStrip(int8_t actualHeading, uint16_t angle, boolean erase)
{
int x = 32;             // NO OFFSET FROM OX
int xOffset = 64;
int y = OY + 195;
int adjust = 0;
int8_t heading;
uint16_t color;
// THIS IS WRONG - What you need is to offset tick by 64 pixels for every 22.5 degrees the angle changes. After each 22.5 degree increment from 0 pixel adjust needs
// to start over at 0 and go back to 64 (xOffset)
int16_t tickAdjust = (((float)angle/22.5) * ((float)xOffset/100.0));
Serial.print(angle); Serial.print(" "); Serial.println(tickAdjust);

    heading = actualHeading - 2;    // This our start, and we will print five headings from this. Actual heading will end up in the middle. 
    if (heading < 0) heading = NUM_HEADINGS + heading;   // rollover
    // Now we have our first heading

    erase ? color = CurrentBackgroundColor : color = ILI9341_WHITE;

    tft.setTextColor(color);
    
    for (uint8_t i=0; i<5; i++)
    {
        if (i == 2) 
        {
            tft.setFont(Arial_24_Bold);     // Middle heading, actual, larger
            adjust = tft.strPixelLen(directions[heading]);
            tft.setCursor(x + (xOffset * i) - (adjust >> 1), y - 8); // scootch up
            if (!erase) 
            {
                nightTime ? color = NightColor : color = ILI9341_WHITE;
            }
        }
        else if (i == 1 || i == 3)
        {
            tft.setFont(Arial_16_Bold);
            adjust = tft.strPixelLen(directions[heading]);
            tft.setCursor(x + (xOffset * i) - (adjust >> 1), y);
//            if (!erase && (i == 0 || i == 4))  color = COLOR_DESELECT;
            if (!erase && (i == 1 || i == 3)) color = COLOR_DESELECT_MEDIUM;
        }
        else if (i == 0 || i == 4)
        {
            tft.setFont(Arial_14_Bold);
            adjust = tft.strPixelLen(directions[heading]);
            tft.setCursor(x + (xOffset * i) - (adjust >> 1), y);
            if (!erase && (i == 0 || i == 4))  color = COLOR_DESELECT;
//            else if (!erase && (i == 1 || i == 3)) color = COLOR_DESELECT_MEDIUM;            
        }
        tft.setTextColor(color);
        tft.print(directions[heading]);
        
        
        // Increment
        if (++heading >= NUM_HEADINGS) heading = 0;  // rollover

        tft.drawFastVLine(x - xOffset + (xOffset * i) - tickAdjust, y+10, 10, color);
    }
}

void ForceSpeedUpdate(void)
{
    displayElement.setDataFlag(gde_Speed);
}

uint8_t returnHeadingXOffset(uint8_t heading)
{
uint8_t ofst = 0;
uint8_t maxO = 85;      // Set this to the width of the widest heading
    switch(heading)
    {
        case 0:  ofst = maxO-23;     break;      // N        23
        case 1:  ofst = maxO-68;     break;      // NNE      68
        case 2:  ofst = maxO-45;     break;      // NE       45
        case 3:  ofst = maxO-67;     break;      // ENE      67
        case 4:  ofst = maxO-22;     break;      // E        22      
        case 5:  ofst = maxO-66;     break;      // ESE      66
        case 6:  ofst = maxO-44;     break;      // SE       44
        case 7:  ofst = maxO-66;     break;      // SSE      66
        case 8:  ofst = maxO-22;     break;      // S        22      
        case 9:  ofst = maxO-75;     break;      // SSW      75
        case 10: ofst = maxO-53;     break;      // SW       53
        case 11: ofst = maxO-84;     break;      // WSW      84      
        case 12: ofst = maxO-31;     break;      // W        31
        case 13: ofst = maxO-85;     break;      // WNW      85     // Longest
        case 14: ofst = maxO-54;     break;      // NW       54
        case 15: ofst = maxO-77;     break;      // NNW      77
    }
    return ofst;

    // You can calculate the width in pixels of each direction using this: 
    /*
    char* directions[16] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};    
    for (uint8_t i=0; i<16; i++)
    {
        Serial.print(directions[i]);
        Serial.print(" ");
        Serial.println(tft.strPixelLen(directions[i]));
    } 
    */   

    // Then put this in the main loop (where testTime is elapsedMillis defined in Globals). This will cycle through all so you can quickly verify placement.
    /*
    if (testTime > 500)
    {
        Heading += 1;
        if (Heading > 15) Heading = 0;
        testTime = 0;
        displayElement.setDataFlag(gde_Speed);
    } 
    */   
}

void tftPrintHeading(uint8_t heading)
{
    switch(heading)
    {
        case 0:  tft.print("N");    break;
        case 1:  tft.print("NNE");  break;
        case 2:  tft.print("NE");   break;
        case 3:  tft.print("ENE");  break;
        case 4:  tft.print("E");    break;
        case 5:  tft.print("ESE");  break;
        case 6:  tft.print("SE");   break;
        case 7:  tft.print("SSE");  break;
        case 8:  tft.print("S");    break;
        case 9:  tft.print("SSW");  break;
        case 10: tft.print("SW");   break;
        case 11: tft.print("WSW");  break;
        case 12: tft.print("W");    break;
        case 13: tft.print("WNW");  break;
        case 14: tft.print("NW");   break;
        case 15: tft.print("NNW");  break;
    }
}


