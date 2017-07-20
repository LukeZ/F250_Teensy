
//=============================================================================================================================================== >> 
// ALTITUDE
//=============================================================================================================================================== >> 
void RenderAltitude()
{
int x = 0;
int y = 0;  
int xOffset = 0;
int yOffset = 0;
int xD = 0;
int xT = 0;
uint16_t color = ILI9341_WHITE;
static float lastinHg;
static int16_t lastGPS_Alt;
static int16_t lastPresAlt;
static int16_t lastHomeAlt;
static int16_t lastMergedAlt;
static boolean lastUsePressure;
int16_t Merged_Altitude;
static _datetime lastDT = getBlankDateTime(); 
_datetime *dt; 
static uint8_t fHour;
static uint8_t last_fHour;
    
    // Pointer dt is faster than typing eeprom.ramcopy over and over
    dt = &eeprom.ramcopy.lastAltitudeAdjust;

    // In some cases we only show one altitude, this decides which one (actually the Mega tells us)
    UsePressureAltitude ? Merged_Altitude = Pressure_Altitude : Merged_Altitude = GPS_Altitude;

    // Color
    if (nightTime) color = NightColor;

    switch (currentScreen)
    {
        case SCREEN_MAIN:
        case SCREEN_SPEED:
            x = OX + 100;
            y = OY + 0;

            // Prior altitude - overwrite
            tft.setFont(Arial_13);    
            tft.setCursor(x, y);
            tft.setTextColor(CurrentBackgroundColor);
            if (lastMergedAlt > -1000 && lastMergedAlt < 1000) tft.print(" ");
            tftPrintWithComma(lastMergedAlt);
            tft.print(" '  ");
            tft.setFont(Arial_11);
            lastUsePressure ? tft.print("P") : tft.print("G");
                // Current altitude
                tft.setFont(Arial_13);    
                tft.setCursor(x, y);
                tft.setTextColor(color);
                if (Merged_Altitude > -1000 && Merged_Altitude < 1000) tft.print(" ");
                tftPrintWithComma(Merged_Altitude);        
                tft.print(" '  ");
                tft.setFont(Arial_11);
                UsePressureAltitude ? tft.print("P") : tft.print("G");
            break;

        case SCREEN_ALTITUDE:
            // This screen has all the usual elements around the edges, but the center area is devoted to altitudes (both) and barometric pressure
            
            x = OX + 70;
            y = OY + 34;
            xOffset = 80;   // How far across to the right after the labels do we print the altitude
            yOffset = 26;   // Distance between each row
            
            // Doesn't need blacking out first
            tft.setFont(Arial_20_Bold); 
            tft.setCursor(x, y - 4);
            tft.setTextColor(color);
            tft.print("ALTITUDE"); 

            // Only write once
            tft.setTextColor(color);
            tft.setFont(Arial_16_Bold); 
            tft.setCursor(x, y + yOffset);
            tft.print("GPS: ");                
            tft.setCursor(x, y + (yOffset * 2));
            tft.print("BAR: ");
            tft.setCursor(x, y + (yOffset * 3));
            tft.print("Home: ");
            tft.setCursor(x, y + (yOffset * 4));
            tft.print("in Hg: ");
            tft.setCursor(x, y + (yOffset * 5));
            tft.print("Use:");

            // Prior - clear
            tft.setTextColor(CurrentBackgroundColor);
            tft.setFont(Arial_16_Bold); 
            tft.setCursor(x + xOffset, y + yOffset);
            tftPrintWithComma(lastGPS_Alt);
            tft.print(" '");
            tft.setCursor(x + xOffset, y + (yOffset * 2));
            tftPrintWithComma(lastPresAlt);
            tft.print(" '");
            tft.setCursor(x + xOffset, y + (yOffset * 3));
            tftPrintWithComma(lastHomeAlt);
            tft.print(" '");
            tft.setCursor(x + xOffset, y + (yOffset * 4));
            tft.print(lastinHg, 2);
            tft.setCursor(x + xOffset, y + (yOffset * 5));
            lastUsePressure ? tft.print("Barometric") : tft.print("GPS");
                // Current - overwrite
                tft.setTextColor(color);
                tft.setFont(Arial_16_Bold); 
                tft.setCursor(x + xOffset, y + yOffset);
                tftPrintWithComma(GPS_Altitude);
                tft.print(" '");
                tft.setCursor(x + xOffset, y + (yOffset * 2));
                tftPrintWithComma(Pressure_Altitude);
                tft.print(" '");   
                tft.setCursor(x + xOffset, y + (yOffset * 3));
                tftPrintWithComma(Home_Altitude);
                tft.print(" '");
                tft.setCursor(x + xOffset, y + (yOffset * 4));
                tft.print(inHg, 2);                
                tft.setCursor(x, y + (yOffset * 5));
                tft.print("Use:");
                tft.setCursor(x + xOffset, y + (yOffset * 5));
                UsePressureAltitude ? tft.print("Barometric") : tft.print("GPS");

            // Time of last adjustment along the bottom
            xD = OX + x + 40;   // across - date
            xT = OX + x + 140;  // across - time
            y = OY + 200;   // Down at the botom

            // Font & color
            tft.setFont(Arial_14);                  

            // Text on bottom left - doesn't change
            tft.setTextColor(color);
            tft.setCursor(OX, y);
            tft.print("Last adjust:");   

            // Date in middle - Overwrite
            tft.setTextColor(CurrentBackgroundColor);
            tft.setCursor(xD, y);
            tft.print(lastDT.month);
            tft.print("/");
            tft.print(lastDT.day);
            lastDT.year < 100 ? tft.print("/20") : tft.print("/2");
            tft.print(lastDT.year);
                // Date in the middle - Current
                tft.setTextColor(color);
                tft.setCursor(xD, y);
                tft.print(dt->month);
                tft.print("/");
                tft.print(dt->day);
                lastDT.year < 100 ? tft.print("/20") : tft.print("/2");
                tft.print(dt->year);
        
            // Time on the right - Overwrite
            tft.setTextColor(CurrentBackgroundColor);
            tft.setCursor(xT, y);
            if (lastDT.hour < 10) tft.print(" ");
            last_fHour = lastDT.hour;
            if (lastDT.hour > 12) last_fHour = lastDT.hour - 12;
            tft.print(last_fHour);
            tft.print(":");
            if (lastDT.minute < 10) tft.print("0");
            tft.print(lastDT.minute);
            lastDT.hour < 12 ? tft.print(" AM") : tft.print(" PM");
                // Time on the Right - Current
                tft.setTextColor(color);
                tft.setCursor(xT, y);
                if (dt->hour < 10) tft.print(" ");
                fHour = dt->hour;
                if (dt->hour > 12) fHour = dt->hour - 12; 
                tft.print(fHour);                
                tft.print(":");
                if (dt->minute < 10) tft.print("0");
                tft.print(dt->minute);
                dt->hour < 12 ? tft.print(" AM") : tft.print(" PM");
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Altitude);
            return;
            break;             
    }  

    // Save current to last
    lastinHg = inHg;
    lastGPS_Alt = GPS_Altitude; 
    lastPresAlt = Pressure_Altitude;
    lastHomeAlt = Home_Altitude;
    lastMergedAlt = Merged_Altitude;
    lastUsePressure = UsePressureAltitude;
    CopyDateTime(*dt, &lastDT);

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Altitude);    
    
}


float convert_inHG_kPa(float hg)
{
    return (hg * 3.38639);   
}




