
void HandleRotary()
{
    // CHECK THE BUTTON
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
    InputButton.read();
    switch (ButtonState) 
    {
        // This state watches for short and long presses
        case BUTTON_WAIT:   
            if (InputButton.wasReleased())
            {   
                // A single press (short) of the button
                switch (currentScreen)
                {
                    case SCREEN_MENU:
                        break;

                    default:
                        // In all other cases, a single push will enter brighness adjust mode
                        // If we are already in brightness adjust mode, subsequent button presses toggle night/daytime mode
                        if (!adjustBrightness) StartAdjustBrightness();
                        else                   ToggleNighttime();
                        break;
                }
            }
            else if (InputButton.pressedFor(1800)) // Two seconds in real life feels like longer than two seconds, so we do 1.8
            {
                // User has held down the input button for two seconds. 
                // Here we can do something
                do { delay(10); InputButton.read(); } while (!InputButton.wasReleased()); 

                // All the menus above wait for the button to be released before exiting, so we can go straight from here to BUTTON_WAIT
                ButtonState = BUTTON_WAIT;
            }
            break;

        //This is a transition state where we just wait for the user to release the button before moving back to the WAIT state.
        case BUTTON_TO_WAIT:
            if (InputButton.wasReleased()) ButtonState = BUTTON_WAIT;
            break;
    }

    // CHECK THE ROTARY
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
    long kp;
    kp = knob.read() / 4;
    if (kp != knobPosition) 
    {
        if (adjustBrightness)
        {
            // In this case, the knob position adjusts the brightness of the screen
            AdjustBacklight(kp - knobPosition);
        }
        knobPosition = kp;
        // Serial.print("Knob = "); Serial.print(kp); Serial.println();
    }
}

void ToggleNighttime()
{
    nightTime = !nightTime;
    ReStartAdjustBrightness();   // Keep the brightness adjustment mode going since we've pushed a button
    UpdateAllElements();         // This causes the screens to update

    if (DEBUG)
    {   Serial.print(F("Nighttime mode: "));
        if (nightTime) Serial.println(F("ON"));
        else           Serial.println(F("OFF"));
    }
}

void ResetKnob()
{
    knob.write(0);
}

void ReStartAdjustBrightness()
{
    if (timer.isEnabled(TimerID_adjustBrightness)) timer.restartTimer(TimerID_adjustBrightness);
}

void StartAdjustBrightness()
{   
    adjustBrightness = true;        // Set flag
    ResetKnob();                    // Set the knob to zero to start
    TimerID_adjustBrightness = timer.setTimeout(ADJUST_BRIGHTNESS_TIMEOUT, StopAdjustBrightness);
    if (DEBUG) Serial.println(F("Start adjust backlight"));    
}

void StopAdjustBrightness()
{
    adjustBrightness = false;       // Set flag
    if (TimerID_adjustBrightness > 0) timer.deleteTimer(TimerID_adjustBrightness);
    if (DEBUG) Serial.println(F("Stop adjust backlight"));
}

void AdjustBacklight(long kp)
{
    #define PWM_INCREMENT   16
    kp = BacklightPWM + (PWM_INCREMENT * kp);
    kp = constrain(kp, 0, 255);    
    BacklightPWM = (uint8_t)kp;
    setBacklight(BacklightPWM);
    ReStartAdjustBrightness();
    if (DEBUG) { Serial.print(F("Backlight: ")); Serial.println(kp); }     
}

void setBacklight(uint8_t PWM)
{
    analogWrite(TFT_PWM, PWM);
}

void FullBrightness()
{
    setBacklight(255);
}



