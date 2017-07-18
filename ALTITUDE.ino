
//=============================================================================================================================================== >> 
// ALTITUDE
//=============================================================================================================================================== >> 
void RenderAltitude()
{
int x = 0;
int y = 0;  
int xOffset = 0;
int yOffset = 0;
uint16_t color = ILI9341_WHITE;
static float lastinHg;
static int16_t lastGPS_Alt;
static int16_t lastPresAlt;
static int16_t lastMergedAlt;
static boolean lastUsePressure;
int16_t Merged_Altitude;


    // In some cases we only show one altitude, this decides which one (actually the Mega tells us)
    UsePressureAltitude ? Merged_Altitude = Pressure_Altitude : Merged_Altitude = GPS_Altitude;

    // Color
    if (nightTime) color = TEXT_COLOR_NIGHT;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
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

            // Prior - clear
            tft.setTextColor(CurrentBackgroundColor);
            tft.setFont(Arial_16_Bold); 
            tft.setCursor(x, y + yOffset);
            tft.print("GPS: ");
            tft.setCursor(x + xOffset, y + yOffset);
            tftPrintWithComma(lastGPS_Alt);
            tft.print(" '");
            tft.setCursor(x, y + (yOffset * 2));
            tft.print("BAR: ");
            tft.setCursor(x + xOffset, y + (yOffset * 2));
            tftPrintWithComma(lastPresAlt);
            tft.print(" '");
            tft.setCursor(x, y + (yOffset * 3));
            tft.print("Home: ");
            tft.setCursor(x + xOffset, y + (yOffset * 3));
            tftPrintWithComma(Home_Altitude);
            tft.print(" '");
            tft.setCursor(x, y + (yOffset * 4));
            tft.print("in Hg: ");
            tft.setCursor(x + xOffset, y + (yOffset * 4));
            tft.print(lastinHg, 2);
            tft.setCursor(x, y + (yOffset * 5));
            tft.print("Use:");
            tft.setCursor(x + xOffset, y + (yOffset * 5));
            lastUsePressure ? tft.print("Barometric") : tft.print("GPS");
                // Current - overwrite
                tft.setTextColor(color);
                tft.setFont(Arial_16_Bold); 
                tft.setCursor(x, y + yOffset);
                tft.print("GPS: ");
                tft.setCursor(x + xOffset, y + yOffset);
                tftPrintWithComma(GPS_Altitude);
                tft.print(" '");
                tft.setCursor(x, y + (yOffset * 2));
                tft.print("BAR: ");
                tft.setCursor(x + xOffset, y + (yOffset * 2));
                tftPrintWithComma(Pressure_Altitude);
                tft.print(" '");   
                 tft.setCursor(x, y + (yOffset * 3));
                tft.print("Home: ");
                tft.setCursor(x + xOffset, y + (yOffset * 3));
                tftPrintWithComma(Home_Altitude);
                tft.print(" '");
                tft.setCursor(x, y + (yOffset * 4));
                tft.print("in Hg: ");
                tft.setCursor(x + xOffset, y + (yOffset * 4));
                tft.print(inHg, 2);                
                tft.setCursor(x, y + (yOffset * 5));
                tft.print("Use:");
                tft.setCursor(x + xOffset, y + (yOffset * 5));
                UsePressureAltitude ? tft.print("Barometric") : tft.print("GPS");

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
    lastMergedAlt = Merged_Altitude;
    lastUsePressure = UsePressureAltitude;

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Altitude);    
    
}


float convert_inHG_kPa(float hg)
{
    return (hg * 3.38639);   
}




