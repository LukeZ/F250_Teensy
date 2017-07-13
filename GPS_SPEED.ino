
//=============================================================================================================================================== >> 
// GPS
//=============================================================================================================================================== >> 

void RenderGPS()
{
static uint8_t lastNumSats = 0;
int x = 0;
int y = 0;
uint16_t gps_color;
// size of circles
int r_inner = 7;
int r_outer = 11;

#define GPS_GREEN           0x0300          // 0, 99, 0
#define GPS_RED             0x5000          // 84, 0, 0

    
    if (GPS_Fix)
    {
        nightTime ? gps_color = TEXT_COLOR_NIGHT : gps_color = GPS_GREEN;
    }
    else  gps_color = GPS_RED;
    
    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 220;
            y = OY + 5;
            break;

        default:        // Do nothing
            return;
            break;  
    }
    
    // Draw GPS circles
    tft.fillCircle(x, y, r_inner, gps_color);
    tft.drawCircle(x, y, r_outer, gps_color);
    tft.drawCircle(x, y, r_outer + 1, gps_color);    // We draw two outer circles to give the line double width

    // D-GPS
    tft.setFont(Arial_10_Bold);                  
    tft.setCursor(x+17, y-13);                      // Move to the right slightly, and up. Display a "D" for D-GPS
    GPS_FixQuality == 2 ? tft.setTextColor(gps_color) : tft.setTextColor(COLOR_DESELECT);
    tft.print("D");

    // Num satellites
    tft.setFont(Arial_11);                      
    tft.setCursor(x+18, y+3);                       // Move to the right slightly, and down
    tft.setTextColor(CurrentBackgroundColor);
    tft.print(lastNumSats);                         // Overwrite prior value in background
    tft.setCursor(x+18, y+3);                       // Go back
    GPS_Fix ? tft.setTextColor(gps_color) : tft.setTextColor(COLOR_DESELECT);
    tft.print(GPS_NumSatellites);                   // Write current value
    lastNumSats = GPS_NumSatellites;                // Save value for next time

    
    // Fix quality supposedly: 
    //      0 = invalid
    //      1 = GPS fix (SPS)
    //      2 = DGPS fix
    //      3 = PPS fix
    //      4 = Real Time Kinematic
    //      5 = Float RTK
    //      6 = estimated (dead reckoning) (2.3 feature)
    //      7 = Manual input mode
    //      8 = Simulation mode    
    // You will probably not see above 2
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GPS);

    return;
}

//=============================================================================================================================================== >> 
// SPEED 
//=============================================================================================================================================== >> 
void RenderSpeed()
{
int xS = 0;     // Speed location
int yS = 0;  
int xH = 0;     // Heading location
int yH = 0;     
#define COLOR_SPEED_TEXT    ILI9341_WHITE
uint16_t textColor; 
static uint8_t lastSpeed;
static uint8_t lastHeading;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            xS = OX + 120;      // Speed
            yS = OY + 120;
            xH = OX + 222;      // Heading
            yH = OY + 162;
            break;

        default:        // Do nothing
            return;
            break;  
    }      

    // Color
    nightTime ? textColor = TEXT_COLOR_NIGHT : textColor = COLOR_SPEED_TEXT;
    
    // Speed
    tft.setFont(Arial_48_Bold);                  
    // Overwrite prior
    tft.setTextColor(CurrentBackgroundColor);    
    tft.setCursor(xS, yS);  
    if (lastSpeed < 10); tft.print(" ");
    tft.print(lastSpeed);
        // Write current
        tft.setTextColor(textColor);
        tft.setCursor(xS, yS);  
        if (Speed < 10); tft.print(" ");        
        tft.print(Speed);
        // Save current to last
        lastSpeed = Speed;
    
    // MPH text - doesn't change, don't need to pre-write background color
    tft.setFont(Arial_20_Bold);
    tft.setCursor(xS + 8, yS + 58);
    tft.print("MPH");

    // Heading - but only if speed is positive
//    if (Speed > 0)
//    {
        tft.setFont(Arial_24_Bold);
        // Overwrite prior
        tft.setTextColor(CurrentBackgroundColor);
        tft.setCursor(xH + returnHeadingXOffset(lastHeading), yH);
        tftPrintHeading(lastHeading);
            // Write current
            tft.setTextColor(textColor);
            tft.setCursor(xH + returnHeadingXOffset(Heading), yH);
            tftPrintHeading(Heading);
            // Save current to last
            lastHeading = Heading;
//    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Speed);
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


