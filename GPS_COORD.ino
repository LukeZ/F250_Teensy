
//=============================================================================================================================================== >> 
// GPS COORDINATES
//=============================================================================================================================================== >> 

void RenderGPSCoords()
{
int x = 0;
int y = 0;
int xD = 0;
int xT = 0;
int xOffset = 0;
int yOffset = 0;
int xOver = 0;
int ySecondSet = 0;
uint16_t color;
static float lastLatDegrees = LatitudeDegrees.fval;
static float lastLonDegrees = LongitudeDegrees.fval;
static float lastLat = Latitude.fval;
static float lastLon = Longitude.fval;
int degreesymbol = 2;           // size of circle used for degree symbol
char buf[10];
static _datetime lastDT = getBlankDateTime(); 
static uint8_t fHour;
static uint8_t last_fHour;
    
    nightTime ? color = NightColor : color = ILI9341_WHITE;
    
    switch (currentScreen)
    {
        case SCREEN_COORD:
            // This screen has all the usual elements around the edges, but the center area is devoted to latitude & longitude
            
            x = OX + 68;
            y = OY + 34;
            xOffset = 60;       // How far across to the right after the labels do we print the lat/lon
            yOffset = 26;       // Distance between each row
            ySecondSet = 12;    // Distance between the degree set and the LatLon set
            
            // Doesn't need blacking out first
            tft.setFont(Arial_16_Bold); 
            tft.setCursor(x - 4, y - 4);
            tft.setTextColor(color);
            tft.print("COORDINATES"); 
            
            // Only write once
            tft.setTextColor(color);
            tft.setFont(Arial_14_Bold); 
            tft.setCursor(x, y + yOffset);
            tft.print("LAT: ");                
            tft.setCursor(x, y + (yOffset * 2));
            tft.print("LON: ");
            tft.setCursor(x, y + ySecondSet + (yOffset * 3));
            tft.print("LAT: ");
            tft.setCursor(x, y + ySecondSet + (yOffset * 4));
            tft.print("LON: ");

            // Prior - clear     - Degrees
            tft.setTextColor(CurrentBackgroundColor);
            tft.setCursor(x + xOffset, y + yOffset);
            sprintf(buf, "% 3.4f", lastLatDegrees);
            xOver = tft.strPixelLen(buf);    // How far over to put the symbol
            tft.drawCircle(x + xOffset + xOver + 5, y + yOffset, degreesymbol, CurrentBackgroundColor);
            tft.print(buf);
            tft.setCursor(x + xOffset, y + (yOffset * 2));
            sprintf(buf, "% 3.4f", lastLonDegrees);
            tft.print(buf);
            xOver = tft.strPixelLen(buf);
            tft.drawCircle(x + xOffset + xOver + 5, y + (yOffset * 2), degreesymbol, CurrentBackgroundColor);            
                // Current - write  - Degrees
                tft.setTextColor(color);
                tft.setCursor(x + xOffset, y + yOffset);
                sprintf(buf, "% 3.4f", LatitudeDegrees.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);    // How far over to put the symbol
                tft.drawCircle(x + xOffset + xOver + 5, y + yOffset, degreesymbol, color);                
                tft.setCursor(x + xOffset, y + (yOffset * 2));
                sprintf(buf, "% 3.4f", LongitudeDegrees.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);
                tft.drawCircle(x + xOffset + xOver + 5, y + (yOffset * 2), degreesymbol, color);            

            
            
            y = y + ySecondSet;     // Offset the second set of coordinates a bit further down
            xOffset = 50;           // These also don't need to be positioned so far to the right because they will not have a negtive sign in front of them

            // N and E are positive, S and W are negative  
            // Prior - clear     - LatLon
            tft.setTextColor(CurrentBackgroundColor);
            tft.setCursor(x + xOffset, y + (yOffset * 3));
            sprintf(buf, "% 3.4f", lastLat);
            tft.print(buf);
            xOver = tft.strPixelLen(buf);
            tft.setCursor(x + xOffset + xOver + 5, y + (yOffset * 3));
            lastLatDegrees > 0 ? tft.print("N") : tft.print("S");
            tft.setCursor(x + xOffset, y + (yOffset * 4));
            sprintf(buf, "% 3.4f", lastLon);
            tft.print(buf);
            xOver = tft.strPixelLen(buf);
            tft.setCursor(x + xOffset + xOver + 5, y + (yOffset * 4));
            lastLonDegrees > 0 ? tft.print("E") : tft.print("W");            
                // Current - write  - LatLon
                tft.setTextColor(color);
                tft.setCursor(x + xOffset, y + (yOffset * 3));
                sprintf(buf, "% 3.4f", Latitude.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);    
                tft.setCursor(x + xOffset + xOver + 5, y + (yOffset * 3));
                LatitudeDegrees.fval > 0 ? tft.print("N") : tft.print("S");
                tft.setCursor(x + xOffset, y + (yOffset * 4));
                sprintf(buf, "% 3.4f", Longitude.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);
                tft.setCursor(x + xOffset + xOver + 5, y + (yOffset * 4));
                LongitudeDegrees.fval > 0 ? tft.print("E") : tft.print("W");


            // Time of last coordinate along the bottom
            xD = OX + x + 40;   // across - date
            xT = OX + x + 140;  // across - time
            y = OY + 200;   // Down at the botom

            // Font & color
            tft.setFont(Arial_14);                  

            // Text on bottom left - doesn't change
            tft.setTextColor(color);
            tft.setCursor(OX, y);
            tft.print("Last read:");   

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
                tft.print(lastCoordinateTime.month);
                tft.print("/");
                tft.print(lastCoordinateTime.day);
                lastDT.year < 100 ? tft.print("/20") : tft.print("/2");
                tft.print(lastCoordinateTime.year);
        
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
                if (lastCoordinateTime.hour < 10) tft.print(" ");
                fHour = lastCoordinateTime.hour;
                if (lastCoordinateTime.hour > 12) fHour = lastCoordinateTime.hour - 12; 
                tft.print(fHour);                
                tft.print(":");
                if (lastCoordinateTime.minute < 10) tft.print("0");
                tft.print(lastCoordinateTime.minute);
                lastCoordinateTime.hour < 12 ? tft.print(" AM") : tft.print(" PM");
          

            lastLatDegrees = LatitudeDegrees.fval;
            lastLonDegrees = LongitudeDegrees.fval;
            lastLat = Latitude.fval;
            lastLon = Longitude.fval;
            CopyDateTime(lastCoordinateTime, &lastDT);
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_GPS_Coord);        
            return;
            break;  
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GPS_Coord);

    return;
}


