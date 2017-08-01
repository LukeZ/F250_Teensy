
void HandleRotary()
{
elapsedMillis waitForResponse;

    if (screenSplash) return;       // Ignore button and rotary if we are doing the splash screen

    // CHECK THE BUTTON
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
    InputButton.read();
    if (currentScreen != SCREEN_BLANK)  // Ignore button press only if we are on the blank screen
    {
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
                                    currentScreen = SCREEN_MAIN;    // Go back to main screen
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
                                        if (currentMenu == MENU_ADJUST_NIGHT_COLOR) nightTime = true;       // Set the scheme to night-time to help us adjust it
                                        if (currentMenu == MENU_SWAP_ROTARY)    Menu[MENU_SWAP_ROTARY].val_YN = eeprom.ramcopy.RotarySwap;          // Init to current y/n selection
                                        if (currentMenu == MENU_SHOW_MAX_SPEED) Menu[MENU_SHOW_MAX_SPEED].val_YN = eeprom.ramcopy.showMaxSpeed;     // Init to current y/n selection
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
                                                    
                                                    if (currentMenu == MENU_SET_ALT_TO_GPS && Menu[currentMenu].success) 
                                                    {   // Also if successful, save the time of this barometric adjustment
                                                        CopyDateTime(DT,&eeprom.ramcopy.lastAltitudeAdjust);  // Save the time of last adjustment if that's what we've just selected
                                                        EEPROM.updateBlock(offsetof(_eeprom_data, lastAltitudeAdjust), eeprom.ramcopy.lastAltitudeAdjust); // Also to EEPROM
                                                    }
                                                }
                                                // We don't bother updating our local ram/eeprom copies of all-time min/max values when we ask the Mega to clear them, because after the
                                                // Mega clears them it sends the cleared values back and they are updated that way. 
                                                
                                                // Now exit
                                                Menu[currentMenu].entered = false;
                                                inSelection = false;
                                                break;

                                            case MENU_SWAP_ROTARY:
                                                // This is also a Yes/No menu but we don't need to talk to the Mega
                                                Menu[currentMenu].complete = true;
                                                eeprom.ramcopy.RotarySwap = Menu[currentMenu].val_YN;       // Set the swap to Yes or No (true or false)    
                                                EEPROM.updateByte(offsetof(_eeprom_data, RotarySwap), eeprom.ramcopy.RotarySwap);   // Update EEPROM
                                                Menu[currentMenu].success = true;
                                                // Now exit
                                                Menu[currentMenu].entered = false;
                                                inSelection = false;
                                                break;
        
                                            case MENU_SHOW_MAX_SPEED:
                                                // This is also a Yes/No menu but we don't need to talk to the Mega
                                                Menu[currentMenu].complete = true;
                                                eeprom.ramcopy.showMaxSpeed = Menu[currentMenu].val_YN;       // Set the swap to Yes or No (true or false)    
                                                EEPROM.updateByte(offsetof(_eeprom_data, showMaxSpeed), eeprom.ramcopy.showMaxSpeed);   // Update EEPROM
                                                Menu[currentMenu].success = true;
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
                                                if (!Menu[currentMenu].subMenu_entered)
                                                {
                                                    if (Menu[currentMenu].val_YN)                               
                                                    {
                                                        // They have pushed the button while highlighting the altitude number, so now we let them edit it. There is no exiting now
                                                        // other than by editing the number
                                                        Menu[currentMenu].subMenu_entered = true;
                                                    }
                                                    else
                                                    {
                                                        // They are highlighting the "X" and want to exit without changing anything
                                                        Menu[currentMenu].complete = false;     // Nothing was completed
                                                        Menu[currentMenu].success = true;       // This actually doesn't matter here since we did nothing
                                                        Menu[currentMenu].entered = false;
                                                        inSelection = false;   
                                                    }
                                                }
                                                else
                                                {
                                                    // They decided to edit the number, and now they pressed the button again to indicate they are finished
                                                    // A button press here means we have finished setting the altitude and we can return to the main menu
                                                    if (currentMenu == MENU_SET_HOME_ALT) Home_Altitude = Menu[currentMenu].val_Int;    // Save the home altitude if that's what we've just adjusted
                                                    if (currentMenu == MENU_SET_ALT) 
                                                    {
                                                        CopyDateTime(DT,&eeprom.ramcopy.lastAltitudeAdjust);  // Save the time of last adjustment if that's what we've just selected
                                                        EEPROM.updateBlock(offsetof(_eeprom_data, lastAltitudeAdjust), eeprom.ramcopy.lastAltitudeAdjust); // Also to EEPROM
                                                    }
                                                    Menu[currentMenu].complete = true;                          // Each selection is a positive action here, there is no cancel
                                                    Menu[currentMenu].success = false;                          // Clear the success flag while we wait for a response
                                                    Menu[currentMenu].subMenu_entered = false;                  // Clear the sub-menu flag
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
                                                }
                                                break;
                                            
                                            case MENU_ADJUST_NIGHT_COLOR:
                                                if (!Menu[currentMenu].subMenu_entered)
                                                {
                                                    switch (Menu[currentMenu].val_Int)
                                                    {
                                                        case 0:     // R
                                                        case 1:     // G
                                                        case 2:     // B
                                                            // They have pushed the button while highlighting either R, G, or B, so now we let them edit it. 
                                                            Menu[currentMenu].subMenu_entered = true;
                                                            break;
                                                        case 3:     // X - cancel
                                                            // They are highlighting the "X" and want to exit without our changes
                                                            NightColor = tft.color565(Night_R, Night_G, Night_B);   // Update night color, although it's probably not necessary to do it here
                                                            eeprom.ramcopy.NightColor = NightColor;                 // Set to ramcopy
                                                            EEPROM.updateInt(offsetof(_eeprom_data, NightColor), eeprom.ramcopy.NightColor); // Also to EEPROM
                                                            Menu[currentMenu].complete = true;      // The color has been changed
                                                            Menu[currentMenu].success = true;       
                                                            Menu[currentMenu].entered = false;
                                                            inSelection = false;
                                                            break;
                                                    }
                                                }
                                                else
                                                {
                                                    // They were within a submenu, ie editing R, G, or B, and now they pushed the button and want to get back out
                                                    NightColor = tft.color565(Night_R, Night_G, Night_B);   // Update night color, although it's probably not necessary to do it here
                                                    Menu[currentMenu].subMenu_entered = false;              // Let them out
                                                }
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
                            ToggleNighttime();
                            if (!adjustBrightness) StartAdjustBrightness();
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
    }

    // CHECK THE ROTARY
    // -------------------------------------------------------------------------------------------------------------------------------------------------->
    int16_t tmp;
    long kp;
    long kp_diff;
    kp = knob.read() / 4;
    kp_diff = kp - knobPosition;
    if (eeprom.ramcopy.RotarySwap) kp_diff = -kp_diff;
    if (kp != knobPosition && !screenOff)   // Rotary does nothing if screen is off
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
                ChangeScreenActions();      // Any actions that need to take place when we change screens (Utils tab)
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
                        case MENU_SWAP_ROTARY:
                        case MENU_SHOW_MAX_SPEED:
                            Menu[currentMenu].val_YN = !Menu[currentMenu].val_YN;   // Toggle between yes / no
                            break;

                        case MENU_SET_TIMEZONE: 
                            // Here we are rotating through the 5 timezones
                            tmp = (int16_t)DT.timezone + kp_diff;                   // Convert to signed so we can go negative
                            if      (tmp >= NUM_TIMEZONES) DT.timezone = 0;
                            else if (tmp < 0) DT.timezone = NUM_TIMEZONES-1;        // Zero based
                            else    DT.timezone = (uint8_t)tmp;
                            break;

                        case MENU_SET_ALT:
                        case MENU_SET_HOME_ALT: 
                            if (!Menu[currentMenu].subMenu_entered) Menu[currentMenu].val_YN = !Menu[currentMenu].val_YN;   // Toggle between adjust number / cancel
                            else Menu[currentMenu].val_Int += (kp_diff * 10);            // Change number by 10 foot increments
                            break;

                        case MENU_ADJUST_NIGHT_COLOR:
                             if (!Menu[currentMenu].subMenu_entered)
                             {
                                // We are cycling through R, G, B, X
                                // We represent these as 0,1,2,3 in val_Int
                                Menu[currentMenu].val_Int += kp_diff;                   
                                if      (Menu[currentMenu].val_Int > 3 ) Menu[currentMenu].val_Int = 0;
                                else if (Menu[currentMenu].val_Int < 0)  Menu[currentMenu].val_Int = 3;
                             }
                             else
                             {
                                // Here we have selected R, G, or B and we want to adjust them
                                switch (Menu[currentMenu].val_Int)
                                {
                                    case 0:     // R
                                        tmp = (int16_t)Night_R + (kp_diff * 2);
                                        tmp = constrain(tmp, 0, 255);   // keep to 0-255
                                        Night_R = (uint8_t)tmp;
                                        break;
                                    case 1:     // G
                                        tmp = (int16_t)Night_G + (kp_diff * 2);
                                        tmp = constrain(tmp, 0, 255);   // keep to 0-255
                                        Night_G = (uint8_t)tmp;
                                        break;
                                    case 2:     // B
                                        tmp = (int16_t)Night_B + (kp_diff * 2);
                                        tmp = constrain(tmp, 0, 255);   // keep to 0-255
                                        Night_B = (uint8_t)tmp;
                                        break;
                                }
                             }
                            
                             NightColor = tft.color565(Night_R, Night_G, Night_B);
                             break;
                    }                    
                    displayElement.setDataFlag(gde_Menu);
                }
                break;
        }
    }
    knobPosition = kp;
}

void ToggleNighttime()
{
    screenMode += 1;
    if (screenMode > 2) screenMode = 0;
    SetScreenMode(screenMode);
}

void SetScreenMode(uint8_t sm)
{
    // If the user is manually changing the screen mode, clear the auto-set flags, otherwise it will get cancelled ten seconds later when the next time update arrives. 
    hasAutoNightBeenSet = true;         
    hasAutoDayBeenSet = true;     

    switch(sm)
    {
        case SCREEN_MODE_OFF:  
            ClearScreen();
            ImmediateScreenShutdown();
            StopAdjustBrightness();
            if (currentScreen == SCREEN_MENU)
                currentScreen = SCREEN_MAIN;    // We shouldn't be, but make sure we are not on the MENU screen, otherwise further button presses won't turn the screen on
            break;
        
        case SCREEN_MODE_DAY:  
            if (screenOff) StartScreen(); 
            nightTime = false;
            break;
            
        case SCREEN_MODE_NIGHT:
            if (screenOff) StartScreen(); 
            nightTime = true;
            break;        
    }

    if (!screenOff)
    {
        nightTime ? setBacklight(eeprom.ramcopy.DimLevel_Night) : setBacklight(eeprom.ramcopy.DimLevel_Day);
        ReStartAdjustBrightness();   // Keep the brightness adjustment mode going since we've pushed a button
        UpdateAllElements();         // This causes the screens to update
    
        if (DEBUG)
        {   Serial.print(F("Nighttime mode: "));
            if (nightTime) Serial.println(F("ON"));
            else           Serial.println(F("OFF"));
        }
    }
}

void ResetKnob()
{
    knob.write(0);
    knobPosition = 0;
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
    // Save new brightness adjustment to EEPROM
    if (nightTime)
    {
        EEPROM.updateByte(offsetof(_eeprom_data, DimLevel_Night), eeprom.ramcopy.DimLevel_Night);
        if (DEBUG) { Serial.print(F("New night-time brightness level saved: ")); Serial.println(eeprom.ramcopy.DimLevel_Night); }
    }
    else
    {
        EEPROM.updateByte(offsetof(_eeprom_data, DimLevel_Day), eeprom.ramcopy.DimLevel_Day); // Also to EEPROM        
        if (DEBUG) { Serial.print(F("New daytime brightness level saved: ")); Serial.println(eeprom.ramcopy.DimLevel_Day); }
    }
    
}

void AdjustBacklight(long kp)
{
uint8_t PWM_INCREMENT = 0;

    nightTime ? BacklightPWM = eeprom.ramcopy.DimLevel_Night : BacklightPWM = eeprom.ramcopy.DimLevel_Day;

    // Brightness is not a linear adjustment, we want finer control the lower we get
    if      (BacklightPWM < 10)  PWM_INCREMENT =  1;
    else if (BacklightPWM < 20)  PWM_INCREMENT =  2;
    else if (BacklightPWM < 40)  PWM_INCREMENT =  4;
    else if (BacklightPWM < 100) PWM_INCREMENT =  8;
    else if (BacklightPWM < 200) PWM_INCREMENT = 16;
    else                         PWM_INCREMENT = 20;

    kp = BacklightPWM + (PWM_INCREMENT * kp);
    kp = constrain(kp, 0, 255);    
    BacklightPWM = (uint8_t)kp;
    setBacklight(BacklightPWM);
    ReStartAdjustBrightness();
    nightTime ? eeprom.ramcopy.DimLevel_Night = BacklightPWM : eeprom.ramcopy.DimLevel_Day = BacklightPWM;
    if (DEBUG) { Serial.print(F("Backlight: ")); Serial.println(kp); }     
}




