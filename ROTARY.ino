
void HandleRotary()
{
elapsedMillis waitForResponse;

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
                        if (!inMenu)
                        {   // This "enters" the menu, meaning we can now scroll through the menu items
                            knobState = KS_CHANGE_MENU;
                            inMenu = true;
                        }
                        else
                        {
                            // Here we are selecting some menu item or action
                            if (currentMenu == MENU_EXIT_MENU)
                            {   // Exit menu
                                currentScreen = SCREEN_AUTO;    // Go back to main screen
                                knobState = KS_CHANGE_SCREEN;
                                currentMenu = MENU_DEFAULT_MENU;
                                inMenu = false;
                                // Clear item settings
                                for (uint8_t i=0; i<NUM_MENUS; i++)
                                {
                                    Menu[i].entered = false;        // But we are not presently in it
                                    Menu[i].val_YN = false;         // Default to "No"
                                    Menu[i].val_Int = 0;            // Default value to 0
                                    Menu[i].complete = false;       // Action not yet complete
                                }                                        
                                ClearScreen();
                                UpdateAllElements();
                            }
                            else if (Menu[currentMenu].enabled)     // Ignore button presses on menu items that are not enabled
                            {
                                if (!Menu[currentMenu].entered)     // If not entered means this is the first button press, so we are "entering" the menu item 
                                {
                                    Menu[currentMenu].entered = true;   // We are inside the menu
                                    Menu[currentMenu].success = false;  // Clear flag, it will get set on exit
                                    inSelection = true;

                                    // We initialize these altitude set menus to start with
                                    if (currentMenu == MENU_SET_ALT)        Menu[MENU_SET_ALT].val_Int = RoundToNearestTen(Pressure_Altitude);  // Init to current pressure altitude
                                    if (currentMenu == MENU_SET_HOME_ALT)   Menu[MENU_SET_HOME_ALT].val_Int = RoundToNearestTen(GPS_Altitude);  // Would be better to init to current home altitude
                                }
                                else                                // We had already entered this menu item, so now the button press is doing something
                                {
                                    switch (currentMenu)
                                    {   // All these are Yes / No actions so we can deal with them the same
                                        case MENU_SET_ALT_TO_GPS:
                                        case MENU_SET_HOME_COORD: 
                                        case MENU_CLEAR_ALLTIME_TEMP_I: 
                                        case MENU_CLEAR_ALLTIME_TEMP_E: 
                                        case MENU_CLEAR_ALLTIME_TEMP_A: 
                                            // We are pushing the button on a selection within this menu
                                            Menu[currentMenu].complete = Menu[currentMenu].val_YN;      // Is the user trying to complete yes or no
                                            if (Menu[currentMenu].val_YN)                               // If Yes, proceed
                                            {
                                                Menu[currentMenu].success = false;                      // Clear the success flag while we wait for a response
                                                SendMega(Menu[currentMenu].cmdToMega, Menu[currentMenu].valueToMega, Menu[currentMenu].modifierToMega);   // Tell the Mega to do something
                                                waitForResponse = 0;
                                                while (!Menu[currentMenu].success && waitForResponse < 300) // Wait briefly for a response (300 mS) - but if it comes at all, will be almost instant
                                                {
                                                    CheckSerial();                                      // Wait for a response
                                                }
                                            }
                                            // Now exit
                                            Menu[currentMenu].entered = false;
                                            inSelection = false;
                                            break;
    
                                        case MENU_SET_TIMEZONE: 
                                            // We are pushing a button to select a timezone
                                            Menu[currentMenu].complete = true;                          // Each selection is a positive action here, there is no cancel
                                            Menu[currentMenu].success = false;                          // Clear the success flag while we wait for a response
                                            SendMega(RCV_CMD_SET_TIMEZONE, DT.timezone);                // Tell the Mega to change timezone to value stored in Int
                                            waitForResponse = 0;
                                            while (!Menu[currentMenu].success && waitForResponse < 300) // Wait briefly for a response (300 mS) - but if it comes at all, will be almost instant
                                            {
                                                CheckSerial();                                      // Wait for a response
                                            }
                                            // Now exit
                                            Menu[currentMenu].entered = false;
                                            inSelection = false;
                                            break; 

                                        case MENU_SET_HOME_ALT: 
                                        case MENU_SET_ALT: 
                                            // A button press here means we have finished setting the altitude and we can return to the main menu
                                            if (currentMenu == MENU_SET_HOME_ALT) Home_Altitude = Menu[currentMenu].val_Int;    // Save the home altitude if that's what we've just adjusted
                                            Menu[currentMenu].complete = true;                          // Each selection is a positive action here, there is no cancel
                                            Menu[currentMenu].success = false;                          // Clear the success flag while we wait for a response
                                            Menu[currentMenu].valueToMega = Menu[currentMenu].val_Int / 100;    // Value -    Number of feet over 100, divided by 100
                                            Menu[currentMenu].modifierToMega = Menu[currentMenu].val_Int % 100; // Modifier - Number of feet under 100
                                            
                                            SendMega(Menu[currentMenu].cmdToMega, Menu[currentMenu].valueToMega, Menu[currentMenu].modifierToMega);   // Tell the Mega to do something
                                            waitForResponse = 0;
                                            while (!Menu[currentMenu].success && waitForResponse < 300) // Wait briefly for a response (300 mS) - but if it comes at all, will be almost instant
                                            {
                                                CheckSerial();                                      // Wait for a response
                                            }
                                            // Now exit
                                            Menu[currentMenu].entered = false;
                                            inSelection = false;                                            
                                            break;
                                            
                                        case MENU_SET_DEFAULT_SCREEN: 
                                            break;
                                    }
                                }
                            }
                        }
                        displayElement.setDataFlag(gde_Menu);
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
    int16_t tmp;
    long kp;
    long kp_diff;
    kp = knob.read() / 4;
    kp_diff = kp - knobPosition;
    if (RotarySwap) kp_diff = -kp_diff;
    if (kp != knobPosition) 
    {
        switch (knobState)
        {
            case KS_ADJUST_BRIGHTNESS:
                // In this case, the knob position adjusts the brightness of the screen
                AdjustBacklight(kp_diff);
                break;

            case KS_CHANGE_SCREEN:
                // Rotary changes screen
                currentScreen += (kp_diff);
                if (currentScreen < 0) currentScreen = SCREEN_MAX_SCREEN;
                if (currentScreen > SCREEN_MAX_SCREEN) currentScreen = 0;

                ClearScreen();              // Clear screen
                UpdateAllElements();        // Set to new
                break;

            case KS_CHANGE_MENU:
                // Rotary changes menu item, or selects/alters menu sub-item
                if (!inSelection)
                {
                    // We are not within a selection yet, so here we change the menu item
                    currentMenu += kp_diff;
                    if (currentMenu >= NUM_MENUS) currentMenu = 0;
                    if (currentMenu < 0) currentMenu = NUM_MENUS - 1;   // For zero based
                    displayElement.setDataFlag(gde_Menu);
                }
                else
                {
                    // We are within a menu item so the rotary is adjusting/selecting a value
                    switch (currentMenu)
                    {   // All these are Yes / No actions so we can deal with them the same
                        case MENU_SET_ALT_TO_GPS:
                        case MENU_SET_HOME_COORD: 
                        case MENU_CLEAR_ALLTIME_TEMP_I: 
                        case MENU_CLEAR_ALLTIME_TEMP_E: 
                        case MENU_CLEAR_ALLTIME_TEMP_A: 
                            Menu[currentMenu].val_YN = !Menu[currentMenu].val_YN;   // Toggle between yes / no
                            break;

                        case MENU_SET_TIMEZONE: 
                            // Here we are rotating through the 5 timezones
                            tmp = (int16_t)DT.timezone + kp_diff;                   // Convert to signed so we can go negative
                            if      (tmp >= NUM_TIMEZONES) DT.timezone = 0;
                            else if (tmp < 0) DT.timezone = NUM_TIMEZONES-1;        // Zero based
                            else    DT.timezone = (uint8_t)tmp;
                            break;

                        case MENU_SET_DEFAULT_SCREEN: 
                            break;

                        case MENU_SET_ALT:
                        case MENU_SET_HOME_ALT: 
                            Menu[currentMenu].val_Int += (kp_diff * 10);            // Change by 10 foot increments
                            break;
                    }                    
                    displayElement.setDataFlag(gde_Menu);
                }
                break;
        }
        knobPosition = kp;
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
    adjustBrightness = true;                // Set flag
    knobState = KS_ADJUST_BRIGHTNESS;       // Set knob state
    ResetKnob();                            // Set the knob to zero to start
    TimerID_adjustBrightness = timer.setTimeout(ADJUST_BRIGHTNESS_TIMEOUT, StopAdjustBrightness);
    if (DEBUG) Serial.println(F("Start adjust backlight"));    
}

void StopAdjustBrightness()
{
    adjustBrightness = false;       // Set flag
    knobState = KS_DEFAULT;         // Knob state back to default
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



