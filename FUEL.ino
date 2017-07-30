
//=============================================================================================================================================== >> 
// FUEL PUMP
//=============================================================================================================================================== >> 
void RenderFuelPump()
{
int xT = 0;     // Text location
int yT = 0;  
int xP = 0;     // Symbol location
int yP = 0;     
uint16_t color; 

    switch (currentScreen)
    {
        case SCREEN_MAIN:
        case SCREEN_ALTITUDE:
        case SCREEN_COORD:
            xT = OX + 0;       // Text
            yT = OY + 104;
            xP = OX + 20;       // Symbol
            yP = OY + 129;
            break;

        case SCREEN_SPEED:
            xT = OX + 0;       // Text
            yT = OY + 102;
            xP = OX + 20;       // Symbol
            yP = OY + 127;
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_FuelPump);        
            return;
            break;  
    }  

    // tft.setFontAdafruit();                        // To go back to default font
    // tft.setTextSize(2);
    // tft.setTextColor(color, CurrentBackgroundColor);

    // Symbol color
    nightTime ? color = NightColor : color = COLOR_DARK_YELLOW;         // Day or night-time on color
    if (!FuelPump) color = COLOR_DESELECT;                             // Off color

    // PUMP word - this never changes, so we don't have to overwrite first with background
    tft.setFont(Arial_13_Bold);                  
    tft.setCursor(xT, yT);
    tft.setTextColor(color);
    tft.print("PUMP");

    // Pump symbol is a circle with two lines coming out of it
    tft.fillCircle(xP, yP, 6, color);
    //tft.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);   // Lines will actually be rectangles so we can get some thickness
    // Top line to the right
    tft.drawFastHLine(xP - 2, yP - 6, 15, color);
    tft.drawFastHLine(xP - 2, yP - 5, 10, color);    
    // Bottom line to the left
    tft.drawFastHLine(xP - 13, yP + 6, 15, color);
    tft.drawFastHLine(xP - 10, yP + 5, 12, color);    
    
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_FuelPump);
    
}


