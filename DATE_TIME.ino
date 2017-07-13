
//=============================================================================================================================================== >> 
// DATE TIME
//=============================================================================================================================================== >> 
void RenderDateTime()
{
int xD = 0;
int xT = 0;
int y = 0;  
uint16_t color = ILI9341_WHITE;
static _datetime lastDT = getBlankDateTime(); 
static uint8_t fHour;
static uint8_t last_fHour;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            xD = OX + 0;   // across - date
            xT = OX + 230;  // across - time
            y = OY + 200;
            break;

        default:        // Do nothing
            return;
            break;  
    }      

    if (DT.day == 1 && DT.month == 1 && DT.year == 0 && DT.hour == 00 && DT.minute == 0)
    {
        // Datetime hasn't been updated yet - don't display anything
        displayElement.clearDataFlag(gde_DateTime); // clear flag
        return;                                     // exit
    }

    // Font & color
    tft.setFont(Arial_13);                  
    if (nightTime) color = TEXT_COLOR_NIGHT;
    
    // Date on the left - Overwrite
    tft.setTextColor(CurrentBackgroundColor);
    tft.setCursor(xD, y);
    tftPrintMonth(lastDT.month);
    tft.print(" ");
    tft.print(lastDT.day);
    lastDT.year < 100 ? tft.print(", 20") : tft.print(", 2");
    tft.print(lastDT.year);
        // Date on the left - Current
        tft.setTextColor(color);
        tft.setCursor(xD, y);
        tftPrintMonth(DT.month);
        tft.print(" ");
        tft.print(DT.day);
        lastDT.year < 100 ? tft.print(", 20") : tft.print(", 2");
        tft.print(DT.year);

    // Time on the right - Overwrite
    tft.setTextColor(CurrentBackgroundColor);
    tft.setCursor(xT, y);
    if (lastDT.hour < 10) tft.print(" ");
    if (lastDT.hour > 12) last_fHour = lastDT.hour - 12;
    tft.print(last_fHour);
    tft.print(":");
    if (lastDT.minute < 10) tft.print("0");
    tft.print(lastDT.minute);
    lastDT.hour < 13 ? tft.print(" AM") : tft.print(" PM");
        // Time on the Right - Current
        tft.setTextColor(color);
        tft.setCursor(xT, y);
        if (DT.hour < 10) tft.print(" ");
        if (DT.hour > 12) fHour = DT.hour - 12; 
        tft.print(fHour);
        tft.print(":");
        if (DT.minute < 10) tft.print("0");
        tft.print(DT.minute);
        DT.hour < 13 ? tft.print(" AM") : tft.print(" PM");
    
    // Save current to last
    CopyDateTime(DT, &lastDT);
    
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_DateTime);
}

void tftPrintMonth(int16_t m)
{
    switch (m)
    {
        case 1:  tft.print("Jan"); break;
        case 2:  tft.print("Feb"); break;
        case 3:  tft.print("Mar"); break;
        case 4:  tft.print("Apr"); break;
        case 5:  tft.print("May"); break;
        case 6:  tft.print("Jun"); break;
        case 7:  tft.print("Jul"); break;
        case 8:  tft.print("Aug"); break;
        case 9:  tft.print("Sep"); break;
        case 10: tft.print("Oct"); break;
        case 11: tft.print("Nov"); break;
        case 21: tft.print("Dec"); break;
    }
}

_datetime getBlankDateTime()
{
    _datetime dt; 
    dt.year = 0;
    dt.month = 1;
    dt.day = 1;
    dt.hour = dt.minute = dt.second = 0;
    return dt;
}

void CopyDateTime(_datetime FromDT, _datetime *ToDT)
{
    ToDT->hour = FromDT.hour;
    ToDT->minute = FromDT.minute;
    ToDT->second = FromDT.second;
    ToDT->year = FromDT.year;
    ToDT->month = FromDT.month;
    ToDT->day = FromDT.day;
}



