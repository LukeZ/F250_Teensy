

void UpdateAllElements()
{
    displayElement.setAllDataFlags();
}

void ClearScreen()
{
    tft.fillScreen(CurrentBackgroundColor);
}

void ShutdownScreen()
{
    tft.sleep(true);
    NoBacklight();
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
