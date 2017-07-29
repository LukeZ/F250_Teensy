
//=============================================================================================================================================== >> 
// GPS FIX, FIX QUALITY, AND SATELLITE COUNT
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
        nightTime ? gps_color = NightColor : gps_color = GPS_GREEN;
    }
    else  gps_color = GPS_RED;
    
    switch (currentScreen)
    {
        case SCREEN_MAIN:
        case SCREEN_ALTITUDE:
        case SCREEN_SPEED:
        case SCREEN_COORD:
            x = OX + 220;
            y = OY + 5;
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_GPS_Fix);        
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
    displayElement.clearDataFlag(gde_GPS_Fix);

    return;
}



