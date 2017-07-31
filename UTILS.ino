

void UpdateAllElements()
{
    displayElement.setAllDataFlags();
}

void ChangeScreenActions()
{
    if (currentScreen != SCREEN_MAIN && currentScreen != SCREEN_SPEED)
    {   
        timer.deleteTimer(TimerID_SpeedUpdate);   // Stop this timer if we're not showing speed
        TimerID_SpeedUpdate = 0;
    }

    if (currentScreen != SCREEN_SPEED)
    {
        timer.deleteTimer(TimerID_AngleUpdate);   // Stop this timer if we're not showing speed
        TimerID_AngleUpdate = 0;        

        SessionMaxSpeed = 0;                      // Also reset max speed each time we move off the screen, so in fact it's even less than "session" max. 
    }   

    if (currentScreen == SCREEN_LOGO)
    {
        DrawLogo();

        // We also clear the low voltage timer
        StopLowVoltageBlinker();
    }
}

void DrawLogo(void)
{
    // Image is 312 x 140 pixels
    tft.writeRect(4, 55, 312, 140, (uint16_t*)ford); // Load image, but you won't see it
}

void ClearScreen()
{
    tft.fillScreen(CurrentBackgroundColor);
}

void SplashScreen()
{
    tft.setRotation(tftRotation);
    tft.sleep(false);
    screenOff = false;
    screenSplash = true;                    // Have the main loop run through the splash screen animation 
    splashDir = true;                       // We want to fade in
}

void StartScreen()
{
    // We come here after the splash is done
    tft.setRotation(tftRotation);
    tft.sleep(false);
    screenOff = false;
    screenSplash = false;
    nightTime = false;                      // Start back on in day-time mode
    setBacklight(eeprom.ramcopy.DimLevel_Day);
    UpdateAllElements();
    
    if (currentScreen == SCREEN_LOGO) DrawLogo();   // Re-draw if this is the screen we're on
}

void ImmediateScreenShutdown()
{
    tft.sleep(true);
    NoBacklight();
    screenOff = true;
    screenMode = SCREEN_MODE_OFF;      
    ResetKnob();
    ShutdownScreenTimers();
}

void ShutdownScreen()
{   
    screenSplash = true;                    // Have the main loop run through the splash screen animation
    splashDir = false;                      // We want to fade out
    ResetKnob();
    ShutdownScreenTimers();
}

void FinalizeShutdown()
{
    tft.sleep(true);
    NoBacklight();
    screenOff = true;
    screenMode = SCREEN_MODE_OFF;    
}

void ShutdownScreenTimers()
{    
    timer.deleteTimer(TimerID_SpeedUpdate);
    TimerID_SpeedUpdate = 0;
    
    timer.deleteTimer(TimerID_AngleUpdate);
    TimerID_AngleUpdate = 0;
}

void setBacklight(uint8_t PWM)
{
    analogWrite(TFT_PWM, PWM);
}

void FullBrightness()
{
    setBacklight(255);
}

void NoBacklight()
{
    setBacklight(0);
}

void RevertDefaultFont()
{
    tft.setFontAdafruit();                      // To go back to default font
    tft.setTextSize(6);                         // Then you set the size this way instead of selecting the font size in the font name
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

void tftPrintSpaces(uint8_t numSpaces)
{
    for (uint8_t i=0; i<numSpaces; i++)
        tft.print(" ");
}

int16_t RoundToNearestTen(int16_t val)
{
    val = (val + 5) / 10;
    return val * 10;
}





    
