
/*
gde_Transmission
gde_Temperature
gde_Altitude
gde_GPS
gde_Speed
_Heading
gde_Radio
gde_FuelPump
_Alarm 
gde_Air 
gde_Ign
gde_Voltage
gde_GSM
gde_DateTime

// #define ILI9341_BLACK       0x0000         0,   0,   0 
// #define ILI9341_NAVY        0x000F         0,   0, 128
// #define ILI9341_DARKGREEN   0x03E0         0, 128,   0 
// #define ILI9341_DARKCYAN    0x03EF         0, 128, 128 
// #define ILI9341_MAROON      0x7800       128,   0,   0 
// #define ILI9341_PURPLE      0x780F       128,   0, 128 
// #define ILI9341_OLIVE       0x7BE0       128, 128,   0 
// #define ILI9341_LIGHTGREY   0xC618       192, 192, 192 
// #define ILI9341_DARKGREY    0x7BEF       128, 128, 128 
// #define ILI9341_BLUE        0x001F         0,   0, 255 
// #define ILI9341_GREEN       0x07E0         0, 255,   0 
// #define ILI9341_CYAN        0x07FF         0, 255, 255 
// #define ILI9341_RED         0xF800       255,   0,   0 
// #define ILI9341_MAGENTA     0xF81F       255,   0, 255 
// #define ILI9341_YELLOW      0xFFE0       255, 255,   0 
// #define ILI9341_WHITE       0xFFFF       255, 255, 255 
// #define ILI9341_ORANGE      0xFD20       255, 165,   0 
// #define ILI9341_GREENYELLOW 0xAFE5       173, 255,  47 
// #define ILI9341_PINK        0xF81F
   #define ILI9341_NIGHT_GREEN 0x0F96       15, 240, 170
*/

#define GPS_GREEN_2         0x07E0
#define GPS_GREEN           0x0300          // 0, 99, 0
#define GPS_RED             0x5000          // 84, 0, 0


void RenderVoltage()
{
int x = 0;
int y = 0;  
uint16_t color = ILI9341_WHITE;
static float lastVoltage;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 0;
            y = OY + 0;
            break;
    }  

    // tft.setFontAdafruit();                        // To go back to default font
    // tft.setTextSize(2);
    // tft.setTextColor(color, CurrentBackgroundColor);

    // Arial
    tft.setFont(Arial_13);                  
    if (nightTime) color = TEXT_COLOR_NIGHT;
    
    // Prior voltage - overwrite
    tft.setCursor(x, y);
    tft.setTextColor(CurrentBackgroundColor);
    tft.print(lastVoltage,2);
    tft.print(" v");
        // Current voltage
        tft.setCursor(x, y);
        tft.setTextColor(color);
        tft.print(Voltage,2);
        tft.print(" v");        

    // Save current to last
    lastVoltage = Voltage; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Voltage);
    
}

void RenderFuelPump()
{
int xT = 0;     // Text location
int yT = 0;  
int xP = 0;     // Symbol location
int yP = 0;     
#define COLOR_PUMP_ON   0xFE60        // Yellow 255, 204, 0
uint16_t pumpColor; 
#define COLOR_PUMP_TEXT 0x3186        // Deselect 51,  51,  51      //  0x9CD3    // Grey 153, 153, 153 but too light
uint16_t textColor; 
static boolean lastPump;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            xT = OX + 0;       // Text
            yT = OY + 130;
            xP = OX + 20;       // Symbol
            yP = OY + 160;
            break;
    }  

    // tft.setFontAdafruit();                        // To go back to default font
    // tft.setTextSize(2);
    // tft.setTextColor(color, CurrentBackgroundColor);

    // PUMP word - this never changes, so we don't have to overwrite first with background
    textColor = COLOR_PUMP_TEXT;
    tft.setFont(Arial_12_Bold);                  
    tft.setCursor(xT, yT);
    tft.setTextColor(textColor);
    tft.print("PUMP");

    // Symbol color
    nightTime ? pumpColor = TEXT_COLOR_NIGHT : pumpColor = COLOR_PUMP_ON;      // Day or night-time on color
    if (!FuelPump) pumpColor = COLOR_DESELECT;                                  // Off color

    // Pump symbol is a circle with two lines coming out of it
    tft.fillCircle(xP, yP, 6, pumpColor);
    //tft.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);   // Lines will actually be rectangles so we can get some thickness
    // Top line to the right
    tft.drawFastHLine(xP - 2, yP - 6, 15, pumpColor);
    tft.drawFastHLine(xP - 2, yP - 5, 10, pumpColor);    
    // Bottom line to the left
    tft.drawFastHLine(xP - 13, yP + 6, 15, pumpColor);
    tft.drawFastHLine(xP - 10, yP + 5, 12, pumpColor);    
    
    // Save current to last
    lastPump = FuelPump; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_FuelPump);
    
}

void RenderSpeed()
{
int xS = 0;     // Speed location
int yS = 0;  
int xH = 0;     // Heading location
int yH = 0;     
#define COLOR_SPEED_TEXT    ILI9341_WHITE
uint16_t textColor; 
static uint8_t lastSpeed;
static uint8_t lastHeading;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            xS = OX + 110;      // Speed
            yS = OY + 120;
            xH = OX + 222;      // Heading
            yH = OY + 150;
            break;
    }      

    // Color
    nightTime ? textColor = TEXT_COLOR_NIGHT : textColor = COLOR_SPEED_TEXT;
    
    // Speed
    tft.setFont(Arial_48_Bold);                  
    // Overwrite prior
    tft.setTextColor(CurrentBackgroundColor);    
    tft.setCursor(xS, yS);  
    if (lastSpeed < 10); tft.print(" ");
    tft.print(lastSpeed);
        // Write current
        tft.setTextColor(textColor);
        tft.setCursor(xS, yS);  
        if (Speed < 10); tft.print(" ");        
        tft.print(Speed);
    
    // MPH text - doesn't change, don't need to pre-write background color
    tft.setFont(Arial_20_Bold);
    tft.setCursor(xS + 8, yS + 58);
    tft.print("MPH");

    // Heading
    tft.setFont(Arial_24_Bold);
    // Overwrite prior
    tft.setTextColor(CurrentBackgroundColor);
    tft.setCursor(xH + returnHeadingXOffset(lastHeading), yH);
    tftPrintHeading(lastHeading);
        // Write current
        tft.setTextColor(textColor);
        tft.setCursor(xH + returnHeadingXOffset(Heading), yH);
        tftPrintHeading(Heading);
    
    // Save current to last
    lastSpeed = Speed;
    lastHeading = Heading;
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Speed);
}

uint8_t returnHeadingXOffset(uint8_t heading)
{
uint8_t ofst = 0;
uint8_t maxO = 85;      // Set this to the width of the widest heading
    switch(heading)
    {
        case 0:  ofst = maxO-23;     break;      // N        23
        case 1:  ofst = maxO-68;     break;      // NNE      68
        case 2:  ofst = maxO-45;     break;      // NE       45
        case 3:  ofst = maxO-67;     break;      // ENE      67
        case 4:  ofst = maxO-22;     break;      // E        22      
        case 5:  ofst = maxO-66;     break;      // ESE      66
        case 6:  ofst = maxO-44;     break;      // SE       44
        case 7:  ofst = maxO-66;     break;      // SSE      66
        case 8:  ofst = maxO-22;     break;      // S        22      
        case 9:  ofst = maxO-75;     break;      // SSW      75
        case 10: ofst = maxO-53;     break;      // SW       53
        case 11: ofst = maxO-84;     break;      // WSW      84      
        case 12: ofst = maxO-31;     break;      // W        31
        case 13: ofst = maxO-85;     break;      // WNW      85     // Longest
        case 14: ofst = maxO-54;     break;      // NW       54
        case 15: ofst = maxO-77;     break;      // NNW      77
    }
    return ofst;

    // You can calculate the width in pixels of each direction using this: 
    /*
    char* directions[16] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};    
    for (uint8_t i=0; i<16; i++)
    {
        Serial.print(directions[i]);
        Serial.print(" ");
        Serial.println(tft.strPixelLen(directions[i]));
    } 
    */   

    // Then put this in the main loop (where testTime is elapsedMillis defined in Globals). This will cycle through all so you can quickly verify placement.
    /*
    if (testTime > 500)
    {
        Heading += 1;
        if (Heading > 15) Heading = 0;
        testTime = 0;
        displayElement.setDataFlag(gde_Speed);
    } 
    */   
}

void tftPrintHeading(uint8_t heading)
{
    switch(heading)
    {
        case 0:  tft.print("N");    break;
        case 1:  tft.print("NNE");  break;
        case 2:  tft.print("NE");   break;
        case 3:  tft.print("ENE");  break;
        case 4:  tft.print("E");    break;
        case 5:  tft.print("ESE");  break;
        case 6:  tft.print("SE");   break;
        case 7:  tft.print("SSE");  break;
        case 8:  tft.print("S");    break;
        case 9:  tft.print("SSW");  break;
        case 10: tft.print("SW");   break;
        case 11: tft.print("WSW");  break;
        case 12: tft.print("W");    break;
        case 13: tft.print("WNW");  break;
        case 14: tft.print("NW");   break;
        case 15: tft.print("NNW");  break;
    }
}


void RenderAltitude()
{
    // Altitude
    tft.setFont(Arial_13);
    tft.setCursor(130, OY+200);
    tft.print(" ");
    tft.print("342 '");
//    tft.print("9,840 ft");

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Altitude);    
}

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
    if (lastDT.hour > 12) last_fHour = lastDT.hour - 12;
    tft.print(last_fHour);
    tft.print(":");
    if (lastDT.minute < 10) tft.print("0");
    tft.print(lastDT.minute);
    lastDT.hour < 13 ? tft.print(" AM") : tft.print(" PM");
        // Time on the Right - Current
        tft.setTextColor(color);
        tft.setCursor(xT, y);
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

void RenderGSM()
{
int x = 0;
int y = 0;  
#define GSM_BAR             0x0679          // 0, 204, 204
#define GSM_NOT_BAR         COLOR_DESELECT
#define BAR_SPACE           8
#define BAR_WIDTH           5
uint16_t bar_color;
uint16_t final_color;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 270;
            y = OY + 11;
            break;
    }

    nightTime ? bar_color = TEXT_COLOR_NIGHT : bar_color = GSM_BAR;
    GSM_Bars > 0 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x, y, BAR_WIDTH, BAR_WIDTH, final_color); 
    GSM_Bars > 1 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+BAR_SPACE, y-BAR_WIDTH, BAR_WIDTH, BAR_WIDTH*2, final_color);
    GSM_Bars > 2 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+(BAR_SPACE*2), y-(BAR_WIDTH*2), BAR_WIDTH, BAR_WIDTH*3, final_color);
    GSM_Bars > 3 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+(BAR_SPACE*3), y-(BAR_WIDTH*3), BAR_WIDTH, BAR_WIDTH*4, final_color);

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GSM);

}


void RenderGPS()
{
    static uint8_t lastNumSats = 0;
    int x = 0;
    int y = 0;
    uint16_t gps_color;
    // size of circles
    int r_inner = 7;
    int r_outer = 11;

    GPS_Fix ? gps_color = GPS_GREEN : gps_color = GPS_RED;
    
    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 210;
            y = OY + 5;
            break;

        default:
            break;
    }
    
    // Draw GPS circles
    tft.fillCircle(x, y, r_inner, gps_color);
    tft.drawCircle(x, y, r_outer, gps_color);
    tft.drawCircle(x, y, r_outer + 1, gps_color);    // We draw two outer circles to give the line double width

    // D-GPS
    tft.setFont(Arial_10_Bold);                  
    tft.setCursor(x+17, y-13);                      // Move to the right slightly, and up. Display a "D" for D-GPS
    GPS_FixQuality == 2 ? tft.setTextColor(gps_color) : tft.setTextColor(COLOR_DESELECT);
    tft.print("D");

    // Num satellites
    tft.setFont(Arial_11);                      
    tft.setCursor(x+18, y+3);                       // Move to the right slightly, and down
    tft.setTextColor(CurrentBackgroundColor);
    tft.print(lastNumSats);                         // Overwrite prior value in background
    tft.setCursor(x+18, y+3);                       // Go back
    GPS_Fix ? tft.setTextColor(gps_color) : tft.setTextColor(COLOR_DESELECT);
    tft.print(GPS_NumSatellites);                   // Write current value
    lastNumSats = GPS_NumSatellites;                // Save value for next time

    
    // Fix quality supposedly: 
    //      0 = invalid
    //      1 = GPS fix (SPS)
    //      2 = DGPS fix
    //      3 = PPS fix
    //      4 = Real Time Kinematic
    //      5 = Float RTK
    //      6 = estimated (dead reckoning) (2.3 feature)
    //      7 = Manual input mode
    //      8 = Simulation mode    
    // You will probably not see above 2
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GPS);

    return;
}

void RenderTemperature()
{
    int x, y;
    uint16_t color = ILI9341_WHITE;
    int degreesymbol = 4;   // size of circle used for degree symbol

    static uint16_t lastInternalTemp;
    static uint16_t lastExternalTemp;
    static uint16_t lastAuxTemp;

/*
    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = 40;
            y = 70;
            tft.setFont(Aubrey_48);                  // See the .h file for the list of sizes
//            tft.setTextSize(6);
            // Clear old text by over-writing it in the background color
            tft.setCursor(OX+x, OY+y);
            tft.setTextColor(CurrentBackgroundColor);
                tft.print(lastInternalTemp);
                tft.print(" / ");
                tft.print(lastExternalTemp);            
            // Now write the new text
            tft.setCursor(OX+x, OY+y);
            nightTime ? tft.setTextColor(TEXT_COLOR_NIGHT) : tft.setTextColor(color);
                tft.print(InternalTemp);
                tft.print(" / ");
                tft.print(ExternalTemp);
            // Save new text to last
            lastInternalTemp = InternalTemp;
            lastExternalTemp = ExternalTemp;
            lastAuxTemp = AuxTemp;
            // Symbol
//            tft.fillCircle(OX+x, OY+y, degreesymbol, color);
            break;
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Temperature);
*/
    
}


void UpdateAllElements()
{
    displayElement.setAllDataFlags();
}

void InitTempStructs(void)
{
    _tempsensor *ts; 

    for (uint8_t i=0; i<NUM_TEMP_SENSORS; i++)
    {
        if (i == TS_INTERNAL) { ts = &InternalTemp; }
        if (i == TS_EXTERNAL) { ts = &ExternalTemp; }
        if (i == TS_AUX)      { ts = &AuxTemp;      }

        ts->currentTemp = 999;
        ts->priorTemp = 999;
        ts->sessionMinTemp = 999;
        ts->sessionMaxTemp = 999;
        ts->allTimeMinTemp = 999;
        ts->allTimeMinDT = getBlankDateTime();        
        ts->allTimeMaxTemp = 999;
        ts->allTimeMaxDT = getBlankDateTime();   
        ts->updateMinDT_Flag = false;
        ts->updateMaxDT_Flag = false;
        ts->sensorName = i;
    }
}



