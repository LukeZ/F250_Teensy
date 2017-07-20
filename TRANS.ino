
//=============================================================================================================================================== >> 
// TRANSMISSION
//=============================================================================================================================================== >> 
void RenderTransmission()
{
int x = 0;
int y = 0;  
uint16_t color;
static uint8_t lastBT;      // Baumann table

    switch (currentScreen)
    {
        case SCREEN_MAIN:
        case SCREEN_ALTITUDE:
        case SCREEN_SPEED:
            x = OX + 250;
            y = OY + 30;
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Transmission);        
            return;
            break;  
    }  

    // Color
    nightTime ? color = NightColor : color = COLOR_DARK_YELLOW;   // Day or night-time on color

    // Trans box
    tft.drawRect(x, y, 56, 123, color);
    tft.fillRect(x, y, 56, 16, color);
    tft.drawFastHLine(x, y+79, 56, color);
    tft.setFont(Arial_9_Bold);
    tft.setTextColor(CurrentBackgroundColor);
    tft.setCursor(x+6, y+3);
    tft.print("TRANS");
    
    // Font for rest
    tft.setFont(Arial_12_Bold);
    // Prior status
    tft.setCursor(x+5, y+22);
    tft.setTextColor(CurrentBackgroundColor);
    tft.print("LOCK");
    tft.setCursor(x+5, y+42);
    tft.print("AUTO");
    tft.setCursor(x+5, y+62);
    tft.print("UNLK");
    tft.setCursor(x+16, y+85);
    tft.print("OD");        
    tft.setCursor(x+20, y+105);
    if (lastBT == 1)      tft.print("T1");
    else if (lastBT == 2) tft.print("T2");
    else                  tft.print("T?");
        // Current warning
        // Torque converter status
        tft.setCursor(x+5, y+22);
        TQCLockStatus == TQC_FORCE_LOCK ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
        tft.print("LOCK");
        tft.setCursor(x+5, y+42);
        TQCLockStatus == TQC_AUTO ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
        tft.print("AUTO");
        tft.setCursor(x+5, y+62);
        TQCLockStatus == TQC_FORCE_UNLOCK ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
        tft.print("UNLK");
        // Overdrive
        tft.setCursor(x+16, y+85);
        OverdriveEnabled  ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
        tft.print("OD");        
        // Baumann Table
        tft.setCursor(x+20, y+105);
        tft.setTextColor(color);
        if (BaumannTable == 2)      tft.print("T2");
        else 
        {   // We only highlight if we have Baumann table 2 selected. 
            // Otherwise we show the selection but there is no need to highlight. 
            tft.setTextColor(COLOR_DESELECT);
            if (BaumannTable == 1) tft.print("T1");
            else                   tft.print("T?");
        }
        
    // Save current to last
    lastBT = BaumannTable;

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Transmission);
}



