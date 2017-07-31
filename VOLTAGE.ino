
//=============================================================================================================================================== >> 
// VOLTAGE
//=============================================================================================================================================== >> 
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
        case SCREEN_MAIN:
        case SCREEN_ALTITUDE:
        case SCREEN_SPEED:
        case SCREEN_COORD:
            x = OX + 0;
            y = OY + 0;

            if (!lowVoltage && rawVolt <= LOW_VOLTAGE)
            {   // We've just dropped below the low voltage threshold
                StartLowVoltageBlinker();
            }
            if (lowVoltage && rawVolt > LOW_VOLTAGE)
            {   // We've just risen above the low voltage threshold
                StopLowVoltageBlinker();
            }

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
                nightTime ? color = NightColor : color = ILI9341_WHITE;
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
                            
            break;
        
        default:    // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Voltage);            
            return;
            break;            
    }      
}

void StartLowVoltageBlinker()
{                
    lowVoltage = true;
    if (TimerID_VoltageWarning > 0) timer.deleteTimer(TimerID_VoltageWarning);
    TimerID_VoltageWarning = timer.setInterval(400, RenderVoltage);
}


void StopLowVoltageBlinker()
{
    if (TimerID_VoltageWarning > 0) timer.deleteTimer(TimerID_VoltageWarning);
    TimerID_VoltageWarning = 0;
    lowVoltage = false;         // Clear the low voltage flag so it will get set the next time we fall below the threshold
}


