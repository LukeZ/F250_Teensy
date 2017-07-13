
//=============================================================================================================================================== >> 
// ALTITUDE
//=============================================================================================================================================== >> 
void RenderAltitude()
{
int x = 0;
int y = 0;  
uint16_t color = ILI9341_WHITE;
static int16_t lastAltitude;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 120;
            y = OY + 0;
            break;

        default:        // Do nothing
            return;
            break;             
    }  

    // Arial
    tft.setFont(Arial_13);                  
    if (nightTime) color = TEXT_COLOR_NIGHT;
    
    // Prior altitude - overwrite
    tft.setCursor(x, y);
    tft.setTextColor(CurrentBackgroundColor);
    if (lastAltitude > -1000 && lastAltitude < 1000) tft.print(" ");
    tftPrintWithComma(lastAltitude);
    tft.print(" '");
        // Current altitude
        tft.setCursor(x, y);
        tft.setTextColor(color);
        if (GPS_Altitude > -1000 && GPS_Altitude < 1000) tft.print(" ");
        tftPrintWithComma(GPS_Altitude);        
        tft.print(" '");

    // Save current to last
    lastAltitude = GPS_Altitude; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Altitude);    
    
}



