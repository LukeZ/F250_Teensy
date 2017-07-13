
//=============================================================================================================================================== >> 
// LOW AIR WARNING
//=============================================================================================================================================== >> 
void RenderLowAirWarn()
{
int x = 0;
int y = 0;  
uint16_t color;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 5;
            y = OY + 150;
            break;

        default:        // Do nothing
            return;
            break;  
    }  

    // Font and Color
    tft.setFont(Arial_13_Bold);
    nightTime ? color = TEXT_COLOR_NIGHT : color = COLOR_DARK_YELLOW;   // Day or night-time on color
    if (!LowAirWarning) color = COLOR_DESELECT;                         // Warning off color
    
    // Prior warning - overwrite
    tft.setCursor(x+5, y);
    tft.setTextColor(CurrentBackgroundColor);
    tft.print("LO");
    tft.setCursor(x, y+18);
    tft.print("AIR");
        // Current warning
        tft.setCursor(x+5, y);
        tft.setTextColor(color);
        tft.print("LO");
        tft.setCursor(x, y+18);
        tft.print("AIR");      

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Air);
}


