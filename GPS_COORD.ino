
//=============================================================================================================================================== >> 
// GPS COORDINATES
//=============================================================================================================================================== >> 

void RenderGPSCoords()
{
int x = 0;
int y = 0;
int xOffset = 0;
int yOffset = 0;
int xOver = 0;
uint16_t color;
static float lastLat = Current_Latitude.fval;
static float lastLon = Current_Longitude.fval;
int degreesymbol = 2;           // size of circle used for degree symbol
char buf[10];
    
    nightTime ? color = NightColor : color = ILI9341_WHITE;
    
    switch (currentScreen)
    {
        case SCREEN_COORD:
            // This screen has all the usual elements around the edges, but the center area is devoted to latitude & longitude
            
            x = OX + 70;
            y = OY + 34;
            xOffset = 60;   // How far across to the right after the labels do we print the lat/lon
            yOffset = 26;   // Distance between each row
            
            // Doesn't need blacking out first
            tft.setFont(Arial_16_Bold); 
            tft.setCursor(x - 2, y - 4);
            tft.setTextColor(color);
            tft.print("COORDINATES"); 
            
            // Only write once
            tft.setTextColor(color);
            tft.setFont(Arial_14_Bold); 
            tft.setCursor(x, y + yOffset);
            tft.print("LAT: ");                
            tft.setCursor(x, y + (yOffset * 2));
            tft.print("LON: ");

/*
            int16_t first = (int16_t)lastLat;
            uint16_t second = (uint16_t)((lastLat - first) * 10000);
            DebugSerial->print(F("Coordinates: ")); DebugSerial->print(first); DebugSerial->print("."); DebugSerial->print(second);
            DebugSerial->print(F(", ")); 
            first = (int16_t)lastLon;
            second = (uint16_t)((abs(lastLon) - abs(first)) * 10000);                
            DebugSerial->print(first); DebugSerial->print("."); DebugSerial->print(second);
            DebugSerial->println();
*/
            // N and E are positive, S and W are negative
            
            // Prior - clear
            tft.setTextColor(CurrentBackgroundColor);
            tft.setCursor(x + xOffset, y + yOffset);
            sprintf(buf, "% 3.4f", lastLat);
            xOver = tft.strPixelLen(buf);    // How far over to put the symbol
            tft.drawCircle(x + xOffset + xOver + 5, y + yOffset, degreesymbol, CurrentBackgroundColor);
            tft.print(buf);
            tft.setCursor(x + xOffset, y + (yOffset * 2));
            sprintf(buf, "% 3.4f", lastLon);
            tft.print(buf);
            xOver = tft.strPixelLen(buf);
            tft.drawCircle(x + xOffset + xOver + 5, y + (yOffset * 2), degreesymbol, CurrentBackgroundColor);            
                // Current - write
                tft.setTextColor(color);
                tft.setCursor(x + xOffset, y + yOffset);
                sprintf(buf, "% 3.4f", Current_Latitude.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);    // How far over to put the symbol
                tft.drawCircle(x + xOffset + xOver + 5, y + yOffset, degreesymbol, color);                
                tft.setCursor(x + xOffset, y + (yOffset * 2));
                sprintf(buf, "% 3.4f", Current_Longitude.fval);
                tft.print(buf);
                xOver = tft.strPixelLen(buf);
                tft.drawCircle(x + xOffset + xOver + 5, y + (yOffset * 2), degreesymbol, color);            

//            (lastLon < 0) ? tft.print("W") : tft.print("E");    // Negative longitudes are west of the prime meridian, positive are east


// ADD LAST COORDINATE TIME

            lastLat = Current_Latitude.fval;
            lastLon = Current_Longitude.fval;
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


