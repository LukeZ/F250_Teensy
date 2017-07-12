

void RenderVoltage()
{
int x = 0;
int y = 0;  
static uint16_t color;
static float lastVoltage;
static uint32_t lastBlink = millis(); 
#define V_BLINK_FREQ        800

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

    // Color
    if (lowVoltage)
    {
        if ((millis() - lastBlink) > V_BLINK_FREQ)
        {
            if (color == COLOR_DARK_YELLOW) color = COLOR_DESELECT;
            else                            color = COLOR_DARK_YELLOW;
            lastBlink = millis();
        }
    }
    else
    {
        nightTime ? color = TEXT_COLOR_NIGHT : color = ILI9341_WHITE;
    }
    
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

void RenderTemperature()
{
int x, y;
int rO;
int iO;
int xOver;
uint16_t color = ILI9341_WHITE;
int degreesymbol = 2;           // size of circle used for degree symbol
char buf[5];                    // To convert temp to string so we can calculate how wide it is. Leave space for max plus terminator (-999) = 5


    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 80;
            y = OY + 30;
            iO = 40;        // How much space from indicator (I, E, A) to temp
            rO = 32;        // How much vertical space for each row
           
            // Color
            nightTime ? color = TEXT_COLOR_NIGHT : color = ILI9341_WHITE;   // Day or night-time on color
            
            // Overwrite prior
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y);
            tft.setTextColor(CurrentBackgroundColor);
            tft.print("I:"); 
            tft.setCursor(x+iO, y);
            if (InternalTemp.lastSensorPresent)
            {
                tft.print(InternalTemp.priorTemp);
                sprintf(buf, "%i", InternalTemp.priorTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);    // How far over to put the symbol / min/max
                tft.fillCircle(xOver, y+1, degreesymbol, CurrentBackgroundColor);      
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y-1);
                tft.print(InternalTemp.priorSessionMaxTemp);
                tft.setCursor(xOver + 14, y+13);
                tft.print(InternalTemp.priorSessionMinTemp);              
            }
            else
            {
                tft.print("- -");
            }
            
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y+rO);
            tft.setTextColor(CurrentBackgroundColor);
            tft.print("E:");
            tft.setCursor(x+iO, y+rO);
            if (ExternalTemp.lastSensorPresent)
            {            
                tft.print(ExternalTemp.priorTemp);
                sprintf(buf, "%i", ExternalTemp.priorTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);    
                tft.fillCircle(xOver, y+rO+1, degreesymbol, CurrentBackgroundColor);     
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y+rO+1-1);
                tft.print(ExternalTemp.priorSessionMaxTemp);
                tft.setCursor(xOver + 14, y+rO+1+13);
                tft.print(ExternalTemp.priorSessionMinTemp); 
            }
            else
            {
                tft.print("- -");
            }
            //* AUX TEMP
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y+(rO*2));
            tft.setTextColor(CurrentBackgroundColor);
            tft.print("A:");
            tft.setCursor(x+iO, y+(rO*2));
            if (AuxTemp.lastSensorPresent)
            {
                tft.print(AuxTemp.priorTemp);
                sprintf(buf, "%i", AuxTemp.priorTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);    
                tft.fillCircle(xOver, y+(rO*2)+1, degreesymbol, CurrentBackgroundColor);  
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y+(rO*2)+1-1);
                tft.print(AuxTemp.priorSessionMaxTemp);
                tft.setCursor(xOver + 14, y+(rO*2)+1+13);
                tft.print(AuxTemp.priorSessionMinTemp);               
            }
            else
            {
                tft.print("- -");
            }            

            // Write current - Internal
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y);
            InternalTemp.sensorPresent ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
            tft.print("I:"); 
            tft.setCursor(x+iO, y);
            if (InternalTemp.sensorPresent)
            {            
                tft.print(InternalTemp.currentTemp);
                sprintf(buf, "%i", InternalTemp.currentTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);    // How far over to put the symbol / min/max
                tft.fillCircle(xOver, y+1, degreesymbol, color);                
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y-1);
                tft.print(InternalTemp.sessionMaxTemp);
                tft.setCursor(xOver + 14, y+13);
                tft.print(InternalTemp.sessionMinTemp);
            }
            else
            {
                tft.print("- -");
            }

            // Write current - External
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y+rO);
            ExternalTemp.sensorPresent ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
            tft.print("E:");
            tft.setCursor(x+iO, y+rO);
            if (ExternalTemp.sensorPresent)
            {  
                tft.print(ExternalTemp.currentTemp);            
                sprintf(buf, "%i", ExternalTemp.currentTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);
                tft.fillCircle(xOver, y+rO+1, degreesymbol, color);
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y+rO+1-1);
                tft.print(ExternalTemp.sessionMaxTemp);
                tft.setCursor(xOver + 14, y+rO+1+13);
                tft.print(ExternalTemp.sessionMinTemp);              
            }
            else
            {
                tft.print("- -");
            }   
            
            // Write current - Aux
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x, y+(rO*2));
            AuxTemp.sensorPresent ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
            tft.print("A:");
            tft.setCursor(x+iO, y+(rO*2));
            if (AuxTemp.sensorPresent)
            {            
                tft.print(AuxTemp.currentTemp);
                sprintf(buf, "%i", AuxTemp.currentTemp);
                xOver = x+iO+3+tft.strPixelLen(buf);    
                tft.fillCircle(xOver, y+(rO*2)+1, degreesymbol, color);
                tft.setFont(Arial_8);
                tft.setCursor(xOver + 14, y+(rO*2)+1-1);
                tft.print(AuxTemp.sessionMaxTemp);
                tft.setCursor(xOver + 14, y+(rO*2)+1+13);
                tft.print(AuxTemp.sessionMinTemp);               
            }
            else
            {
                tft.print("- -");
            }   
    
            // Save new text to last
            InternalTemp.priorTemp = InternalTemp.currentTemp;
            InternalTemp.priorSessionMaxTemp = InternalTemp.sessionMaxTemp;
            InternalTemp.priorSessionMinTemp = InternalTemp.sessionMinTemp;
            InternalTemp.lastSensorPresent = InternalTemp.sensorPresent;
            ExternalTemp.priorTemp = ExternalTemp.currentTemp;
            ExternalTemp.priorSessionMaxTemp = ExternalTemp.sessionMaxTemp;
            ExternalTemp.priorSessionMinTemp = ExternalTemp.sessionMinTemp;            
            ExternalTemp.lastSensorPresent = ExternalTemp.sensorPresent;
            AuxTemp.priorTemp = AuxTemp.currentTemp;
            AuxTemp.priorSessionMaxTemp = AuxTemp.sessionMaxTemp;
            AuxTemp.priorSessionMinTemp = AuxTemp.sessionMinTemp;
            AuxTemp.lastSensorPresent = AuxTemp.sensorPresent;

            break;
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Temperature);
    
}

void RenderTransmission()
{
int x = 0;
int y = 0;  
uint16_t color;
static _TQC_STATE lastTQC;  // Torque converter lockup status
static boolean lastOD;      // Overdrive
static uint8_t lastBT;      // Baumann table

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 250;
            y = OY + 30;
            break;
    }  

    // Color
    nightTime ? color = TEXT_COLOR_NIGHT : color = COLOR_DARK_YELLOW;   // Day or night-time on color

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
    lastTQC = TQCLockStatus; 
    lastOD = OverdriveEnabled;
    lastBT = BaumannTable;

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Transmission);
}

void RenderHamCB()
{
int x = 0;
int y = 0;  
uint16_t color;
static boolean lastHam;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 0;
            y = OY + 28;
            break;
    }  

    // Color
    nightTime ? color = TEXT_COLOR_NIGHT : color = COLOR_DARK_YELLOW;   // Day or night-time on color

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

    // Save current to last
    lastHam = Ham_On; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Radio);
}

void RenderFuelPump()
{
int xT = 0;     // Text location
int yT = 0;  
int xP = 0;     // Symbol location
int yP = 0;     
uint16_t pumpColor; 
uint16_t textColor; 
static boolean lastPump;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            xT = OX + 0;       // Text
            yT = OY + 104;
            xP = OX + 20;       // Symbol
            yP = OY + 129;
            break;
    }  

    // tft.setFontAdafruit();                        // To go back to default font
    // tft.setTextSize(2);
    // tft.setTextColor(color, CurrentBackgroundColor);

    // PUMP word - this never changes, so we don't have to overwrite first with background
    textColor = COLOR_DESELECT;
    tft.setFont(Arial_13_Bold);                  
    tft.setCursor(xT, yT);
    tft.setTextColor(textColor);
    tft.print("PUMP");

    // Symbol color
    nightTime ? pumpColor = TEXT_COLOR_NIGHT : pumpColor = COLOR_DARK_YELLOW;   // Day or night-time on color
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

void RenderLowAirWarn()
{
int x = 0;
int y = 0;  
uint16_t color;
static boolean lastLowAir;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 5;
            y = OY + 150;
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

    // Save current to last
    lastLowAir = LowAirWarning; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Air);
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
            xS = OX + 120;      // Speed
            yS = OY + 120;
            xH = OX + 222;      // Heading
            yH = OY + 162;
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
        // Save current to last
        lastSpeed = Speed;
    
    // MPH text - doesn't change, don't need to pre-write background color
    tft.setFont(Arial_20_Bold);
    tft.setCursor(xS + 8, yS + 58);
    tft.print("MPH");

    // Heading - but only if speed is positive
//    if (Speed > 0)
//    {
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
            lastHeading = Heading;
//    }
        
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
int x = 0;
int y = 0;  
uint16_t color = ILI9341_WHITE;
static int16_t lastAltitude;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 120;
            y = OY + 0;
            break;
    }  

    // Arial
    tft.setFont(Arial_13);                  
    if (nightTime) color = TEXT_COLOR_NIGHT;
    
    // Prior altitude - overwrite
    tft.setCursor(x, y);
    tft.setTextColor(CurrentBackgroundColor);
    if (lastAltitude > -1000 && lastAltitude < 1000) tft.print(" ");
    tftPrintWithComma(lastAltitude);
    tft.print(" '");
        // Current altitude
        tft.setCursor(x, y);
        tft.setTextColor(color);
        if (GPS_Altitude > -1000 && GPS_Altitude < 1000) tft.print(" ");
        tftPrintWithComma(GPS_Altitude);        
        tft.print(" '");

    // Save current to last
    lastAltitude = GPS_Altitude; 

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Altitude);    
    
}

void tftPrintWithComma(int16_t val) // Will not work right for exactly the value -32768!
{
    if (val < 0) 
    {
        val = -val;
        tft.print("-");
    }
    
    if (val >= 1000) 
    {
        int16_t v = val / 1000;
        tft.print(v);
        tft.print(",");
        val = val - (v * 1000);
        char buf[4];                // Generate a string with exactly 3 characters representing val (plus one for terminator)
        sprintf(buf, "%03d", val);
        tft.print(buf);
    }
    else 
    {
        tft.print(val);
    }
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

#define GPS_GREEN           0x0300          // 0, 99, 0
#define GPS_RED             0x5000          // 84, 0, 0

    
    if (GPS_Fix)
    {
        nightTime ? gps_color = TEXT_COLOR_NIGHT : gps_color = GPS_GREEN;
    }
    else  gps_color = GPS_RED;
    
    switch (currentScreen)
    {
        case SCREEN_AUTO:
            x = OX + 220;
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

        ts->sensorPresent = false;
        ts->lastSensorPresent = false; 
        ts->currentTemp = 999;
        ts->priorTemp = 999;
        ts->sessionMinTemp = 999;
        ts->priorSessionMinTemp = 999;
        ts->sessionMaxTemp = 999;
        ts->priorSessionMaxTemp = 999;
        ts->allTimeMinTemp = 999;
        ts->allTimeMinDT = getBlankDateTime();        
        ts->allTimeMaxTemp = 999;
        ts->allTimeMaxDT = getBlankDateTime();   
        ts->updateMinDT_Flag = false;
        ts->updateMaxDT_Flag = false;
        ts->sensorName = i;
    }
}



