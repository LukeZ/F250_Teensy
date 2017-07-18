
//=============================================================================================================================================== >> 
// TEMPERATURE
//=============================================================================================================================================== >> 
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
                tft.drawCircle(xOver, y+1, degreesymbol, CurrentBackgroundColor);      
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
                tft.drawCircle(xOver, y+rO+1, degreesymbol, CurrentBackgroundColor);     
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
                tft.drawCircle(xOver, y+(rO*2)+1, degreesymbol, CurrentBackgroundColor);  
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
                tft.drawCircle(xOver, y+1, degreesymbol, color);                
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
                tft.drawCircle(xOver, y+rO+1, degreesymbol, color);
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
                tft.drawCircle(xOver, y+(rO*2)+1, degreesymbol, color);
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

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Temperature);        
            return;
            break;  
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Temperature);
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



