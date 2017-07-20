
//=============================================================================================================================================== >> 
// HAM / CB
//=============================================================================================================================================== >> 
void RenderHamCB()
{
int x = 0;
int y = 0;  
uint16_t color;

    switch (currentScreen)
    {
        case SCREEN_MAIN:
        case SCREEN_ALTITUDE:
        case SCREEN_SPEED:
            x = OX + 0;
            y = OY + 28;
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Radio);        
            return;
            break;  
    }  

    // Color
    nightTime ? color = NightColor : color = COLOR_DARK_YELLOW;   // Day or night-time on color

    // ON AIR box
    tft.drawRect(x, y, 50, 61, color);
    tft.fillRect(x, y, 50, 16, color);
    tft.setFont(Arial_9_Bold);
    tft.setTextColor(CurrentBackgroundColor);
    tft.setCursor(x+2, y+3);
    tft.print("ON AIR");
    
    // Font for Ham/CB
    tft.setFont(Arial_13_Bold);
    // Prior status
    tft.setCursor(x+11, y+22);
    tft.setTextColor(CurrentBackgroundColor);
    tft.print("CB");
    tft.setCursor(x+5, y+42);
    tft.print("HAM");
        // Current warning
        tft.setCursor(x+11, y+22);
        Ham_On ? tft.setTextColor(COLOR_DESELECT) : tft.setTextColor(color);
        tft.print("CB");
        tft.setCursor(x+5, y+42);
        Ham_On ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT); 
        tft.print("HAM");      

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Radio);
}




