

//=============================================================================================================================================== >> 
// GPS SPEED 
//=============================================================================================================================================== >> 
void RenderSpeed()
{
int x = 0;     
int y = 0;  
uint16_t color; 
static uint8_t displaySpeed = 0;
static uint8_t lastSpeed;

    switch (currentScreen)
    {
        case SCREEN_MAIN:
            x = OX + 122;      // Speed
            y = OY + 112;

            // Color
            nightTime ? color = NightColor : color = ILI9341_WHITE;

            if (displaySpeed < Speed) 
            {
                displaySpeed += 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }
            else if (displaySpeed > Speed) 
            {
                displaySpeed -= 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }   
            else
            {
                displaySpeed = Speed;
                if (timer.isEnabled(TimerID_SpeedUpdate)) timer.disable(TimerID_SpeedUpdate);   // Disable the update timer
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
            x = OX + 80;      
            y = OY + 40;

            // Color
            nightTime ? color = NightColor : color = ILI9341_WHITE;

            if (displaySpeed < Speed) 
            {
                displaySpeed += 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }
            else if (displaySpeed > Speed) 
            {
                displaySpeed -= 1;
                if (!timer.isEnabled(TimerID_SpeedUpdate)) timer.enable(TimerID_SpeedUpdate);   // Enable the update timer
            }   
            else
            {
                displaySpeed = Speed;
                if (timer.isEnabled(TimerID_SpeedUpdate)) timer.disable(TimerID_SpeedUpdate);   // Disable the update timer
            }
            
            // Speed
            tft.setFont(Arial_96_Bold);                  
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




