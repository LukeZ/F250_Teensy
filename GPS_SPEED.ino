

//=============================================================================================================================================== >> 
// GPS SPEED 
//=============================================================================================================================================== >> 
void RenderSpeed()
{
int x = 0;     
int y = 0;  
uint16_t color; 
static int16_t displaySpeed = 0;
static int16_t speedDiff = 0;
static int16_t lastSpeed;
long updateRate = 0;

    switch (currentScreen)
    {
        case SCREEN_MAIN:
            x = OX + 122;      // Speed
            y = OY + 112;

            // Color
            nightTime ? color = NightColor : color = ILI9341_WHITE;

            speedDiff = abs(Speed - displaySpeed);
            if (speedDiff > 0)
            {
                if (displaySpeed < Speed)       displaySpeed += 1;
                else if (displaySpeed > Speed)  displaySpeed -= 1;

                if      (speedDiff > 15)    updateRate = 30;
                else if (speedDiff > 10)    updateRate = 60;
                else if (speedDiff > 5)     updateRate = 80;
                else                        updateRate = 100;

                timer.deleteTimer(TimerID_SpeedUpdate);
                TimerID_SpeedUpdate = timer.setInterval(updateRate, ForceSpeedUpdate); 
            }
            else
            {
                displaySpeed = Speed;
                timer.deleteTimer(TimerID_SpeedUpdate);   // Delete the update timer
                TimerID_SpeedUpdate = 0;
            }
            
            // Speed
            tft.setFont(Arial_48_Bold);                  
            // Overwrite prior
            tft.setTextColor(CurrentBackgroundColor);    
            (lastSpeed < 10) ? tft.setCursor(x + 19, y): tft.setCursor(x, y);
            tft.print(lastSpeed);
                // Write current
                tft.setTextColor(color);
                (displaySpeed < 10) ? tft.setCursor(x + 19, y): tft.setCursor(x, y);
                tft.print(displaySpeed);
                // Save current display to last
                lastSpeed = displaySpeed;
            
            // MPH text - doesn't change, don't need to pre-write background color
            tft.setFont(Arial_20_Bold);
            tft.setCursor(x + 5, y + 58);
            tft.print("MPH");
            
            break;

        case SCREEN_SPEED:
            x = OX + 76;      
            y = OY + 40;

            // Color
            nightTime ? color = NightColor : color = ILI9341_WHITE;

            speedDiff = abs(Speed - displaySpeed);
            if (speedDiff > 0)
            {
                if (displaySpeed < Speed)       displaySpeed += 1;
                else if (displaySpeed > Speed)  displaySpeed -= 1;

                if      (speedDiff > 15)    updateRate = 30;
                else if (speedDiff > 10)    updateRate = 60;
                else if (speedDiff > 5)     updateRate = 80;
                else                        updateRate = 100;

                timer.deleteTimer(TimerID_SpeedUpdate);
                TimerID_SpeedUpdate = timer.setInterval(updateRate, ForceSpeedUpdate); 
            }
            else
            {
                displaySpeed = Speed;
                timer.deleteTimer(TimerID_SpeedUpdate);   // Delete the update timer
                TimerID_SpeedUpdate = 0;
            }
            
            // Speed
            tft.setFont(Arial_96_Bold);                  
            // Overwrite prior
            tft.setTextColor(CurrentBackgroundColor);    
            (lastSpeed < 10) ? tft.setCursor(x + 40, y): tft.setCursor(x, y);
            tft.print(lastSpeed);
                // Write current
                tft.setTextColor(color);
                (displaySpeed < 10) ? tft.setCursor(x + 40, y): tft.setCursor(x, y);
                tft.print(displaySpeed);
                // Save current display to last
                lastSpeed = displaySpeed;
           
            displayElement.clearDataFlag(gde_Speed);        
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Speed);
            return;
            break;  
    }      
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_Speed);
}

void ForceSpeedUpdate(void)
{
    displayElement.setDataFlag(gde_Speed);
}




