
// -------------------------------------------------------------------------------------------------------------------------------------------------->
// SERIAL COMMANDS - TRANSMITTING
// -------------------------------------------------------------------------------------------------------------------------------------------------->
void SendMega(byte command, byte value, byte modifier)
{
    MegaSerial.write(MASTER_ADDRESS);       // Sending to the MASTER
    MegaSerial.write(command);  
    MegaSerial.write(value);
    MegaSerial.write(modifier);
    MegaSerial.write((MASTER_ADDRESS + command + value + modifier) & B01111111);
}

void SendMega(byte command, byte value)
{
    SendMega(command, value, 0); 
}

void SendMega(byte command)
{
    SendMega(command, 0, 0);
}


// -------------------------------------------------------------------------------------------------------------------------------------------------->
// SERIAL COMMANDS - RECEIVING
// -------------------------------------------------------------------------------------------------------------------------------------------------->
boolean CheckSerial(void)   // THIS IS US READING COMMANDS _FROM_ THE MEGA2560 IN THE CONSOLE
{
    byte ByteIn;
    static char input_line[SENTENCE_BYTES];     // An array to store incoming bytes
    boolean SentenceReceived = false;           // Start off false, will get set to true if a valid sentence was received
    static boolean addressReceived = false;     // Have we received a byte that matches our address
    static uint8_t numBytes = 0;                // Start off with no data received
    static DataSentence Sentence;               // A struct to store incoming commands

    // Read all the bytes that are available, starting with the first byte that matches our address
    while(MegaSerial.available())               
    {
        ByteIn = MegaSerial.read();
        if (ByteIn == DISPLAY_ADDRESS)      // We are the DISPLAY
        {
            addressReceived = true;         // Matching address
            input_line[0] = ByteIn;         // Save it in our array
            numBytes = 1;                   // Subsequent bytes will be added to the array until we have enough to compare against INIT_STRING
        }
        else if (addressReceived)
        {
            input_line[numBytes++] = ByteIn;
            if (numBytes >= SENTENCE_BYTES) break;  // We have enough bytes for a full sentence, so evaluate it
        }
    }

    // If we have enough bytes for a full sentence, save it
    if (numBytes >= SENTENCE_BYTES)
    {   // We have enough bytes for a full sentence
        Sentence.Address  = input_line[0];
        Sentence.Command  = input_line[1];
        Sentence.Value    = input_line[2];
        Sentence.Modifier = input_line[3];
        Sentence.Checksum = input_line[4];

        // Now verify the checksum
        if (ChecksumValid(&Sentence))
        {
            SentenceReceived = true;        // Yes, a valid sentence has been received!  
            ProcessCommand(&Sentence);      // Do whatever we're told
        }

        // Start everything over
        input_line[0] = '\0';
        addressReceived = false;
        numBytes = 0;
    }

    return SentenceReceived;
}

boolean ChecksumValid(DataSentence * sentence)
{
    uint8_t check = (sentence->Address + sentence->Command + sentence->Value + sentence->Modifier) & B01111111;

    if (check == sentence->Checksum) return true;
    else                             return false;
}

void ProcessCommand(DataSentence * sentence)
{
int16_t temp;
uint16_t tAngle;
_tempsensor *ts; 
boolean regularDT = true; 
static uint8_t volts;
static int TimerID_VoltageWarning = 0;
static boolean lastFix = false; 
    
    switch (sentence->Command)
    {
        case CMD_TEMP_POSITIVE:
        case CMD_TEMP_NEGATIVE:
            temp = (int16_t)sentence->Value;
            if (sentence->Command == CMD_TEMP_NEGATIVE) temp = -temp;
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   if (InternalTemp.currentTemp != temp) { InternalTemp.currentTemp = temp; InternalTemp.sensorPresent = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_EXTERNAL:   if (ExternalTemp.currentTemp != temp) { ExternalTemp.currentTemp = temp; ExternalTemp.sensorPresent = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_AUX:        if (AuxTemp.currentTemp != temp)      { AuxTemp.currentTemp = temp;      AuxTemp.sensorPresent = true;      displayElement.setDataFlag(gde_Temperature); } break;
            }
            break;

        case CMD_TEMP_MIN_POS:
        case CMD_TEMP_MIN_NEG:
            temp = (int16_t)sentence->Value;
            if (sentence->Command == CMD_TEMP_MIN_NEG) temp = -temp;
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   { InternalTemp.sessionMinTemp = temp; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_EXTERNAL:   { ExternalTemp.sessionMinTemp = temp; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_AUX:        { AuxTemp.sessionMinTemp = temp;      displayElement.setDataFlag(gde_Temperature); } break;
            }
            break;
            
        case CMD_TEMP_MAX_POS:
        case CMD_TEMP_MAX_NEG:
            temp = (int16_t)sentence->Value;
            if (sentence->Command == CMD_TEMP_MAX_NEG) temp = -temp;
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   { InternalTemp.sessionMaxTemp = temp; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_EXTERNAL:   { ExternalTemp.sessionMaxTemp = temp; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_AUX:        { AuxTemp.sessionMaxTemp = temp;      displayElement.setDataFlag(gde_Temperature); } break;
            }
            break;
        
        case CMD_TEMP_ALLTIME_MIN_POS:
        case CMD_TEMP_ALLTIME_MIN_NEG:
            temp = (int16_t)sentence->Value;
            if (sentence->Command == CMD_TEMP_ALLTIME_MIN_NEG) temp = -temp;
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   { InternalTemp.allTimeMinTemp = temp; InternalTemp.updateMinDT_Flag = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_EXTERNAL:   { ExternalTemp.allTimeMinTemp = temp; ExternalTemp.updateMinDT_Flag = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_AUX:        { AuxTemp.allTimeMinTemp = temp;      AuxTemp.updateMinDT_Flag = true;      displayElement.setDataFlag(gde_Temperature); } break;
            }            
            break;
    
        case CMD_TEMP_ALLTIME_MAX_POS:
        case CMD_TEMP_ALLTIME_MAX_NEG:
            temp = (int16_t)sentence->Value;
            if (sentence->Command == CMD_TEMP_ALLTIME_MAX_NEG) temp = -temp;
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   { InternalTemp.allTimeMaxTemp = temp; InternalTemp.updateMaxDT_Flag = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_EXTERNAL:   { ExternalTemp.allTimeMaxTemp = temp; ExternalTemp.updateMaxDT_Flag = true; displayElement.setDataFlag(gde_Temperature); } break;
                case TS_AUX:        { AuxTemp.allTimeMaxTemp = temp;      AuxTemp.updateMaxDT_Flag = true;      displayElement.setDataFlag(gde_Temperature); } break;
            }            
            break;

        
        case CMD_TEMP_LOST:
            // Command gets sent if a sensor is lost, this lets the display not to rely on the last reading forever. Next time a temp is sent means it has been found again.
            switch (sentence->Modifier)
            {
                case TS_INTERNAL:   InternalTemp.sensorPresent = false; break;
                case TS_EXTERNAL:   ExternalTemp.sensorPresent = false; break;
                case TS_AUX:        AuxTemp.sensorPresent = false;      break;
            }
            displayElement.setDataFlag(gde_Temperature);
            break;


        case CMD_GPS_FIX:
            GPS_Fix = (boolean)sentence->Value; 
            if (GPS_Fix)
            {
                GPS_FixQuality = sentence->Modifier;
                Menu[MENU_SET_ALT_TO_GPS].enabled = true;       // Enable GPS menu items
                Menu[MENU_SET_HOME_COORD].enabled = true;
                if (lastFix != GPS_Fix && currentScreen == SCREEN_MENU ) displayElement.setDataFlag(gde_Menu);
            }
            else
            {
                GPS_FixQuality = 0;
                GPS_NumSatellites = 0;
                Menu[MENU_SET_ALT_TO_GPS].enabled = false;       // Disable GPS menu items
                Menu[MENU_SET_HOME_COORD].enabled = false;                
                if (lastFix != GPS_Fix && currentScreen == SCREEN_MENU ) displayElement.setDataFlag(gde_Menu);
            }
            lastFix = GPS_Fix;
            displayElement.setDataFlag(gde_GPS);
            break;

        case CMD_GPS_SATELLITES:
            GPS_Fix ? GPS_NumSatellites = sentence->Value : GPS_NumSatellites = 0;
            displayElement.setDataFlag(gde_GPS);
            break;

        case CMD_SPEED_MPH:
            if (Speed != sentence->Value)
            { 
                Speed = sentence->Value;
                displayElement.setDataFlag(gde_Speed);
            }

        case CMD_GPS_ANGLE:
            // If Modifier is 1, add 180 to value
            if (sentence->Modifier) tAngle = sentence->Value + 180;
            else                    tAngle = sentence->Value;
            if (Angle != tAngle)
            {
                Angle = tAngle;
                displayElement.setDataFlag(gde_Speed);
            }
            break; 

        case CMD_GPS_HEADING:
            if (Heading != sentence->Value)
            {
                Heading = sentence->Value;
                displayElement.setDataFlag(gde_Speed);
            }
            break; 

        case CMD_GPS_ALTITUDE_POS:
        case CMD_GPS_ALTITUDE_NEG:
            temp = (sentence->Value * 100) + sentence->Modifier;
            if (sentence->Command == CMD_GPS_ALTITUDE_NEG) temp = -temp;
            if (GPS_Altitude != temp)
            {
                GPS_Altitude = temp;
                displayElement.setDataFlag(gde_Altitude);
            }
            break;

        case CMD_PRESSURE_ALTITUDE_POS:
        case CMD_PRESSURE_ALTITUDE_NEG:
            temp = (sentence->Value * 100) + sentence->Modifier;
            if (sentence->Command == CMD_PRESSURE_ALTITUDE_NEG) temp = -temp;
            if (Pressure_Altitude != temp)
            {
                Pressure_Altitude = temp;
                displayElement.setDataFlag(gde_Altitude);
            }
            break;

        case CMD_HOME_ALT:
            Home_Altitude = (sentence->Value * 100) + sentence->Modifier;
            // I don't think we need to do a screen update, this only gets sent once at the beginning of startup, or when manually changed
            break;

        case CMD_PRESSURE_MERCURY:
            inHg = (float)sentence->Value + ((float)sentence->Modifier / 100.0);   // Value is integer inches, modifier is the 2 digit decimal fraction
            Pressure = convert_inHG_kPa(inHg);
            displayElement.setDataFlag(gde_Altitude); 
            break;

        case CMD_USE_PRESSURE_ALT:
            UsePressureAltitude = sentence->Value;
            displayElement.setDataFlag(gde_Altitude);             
            break;

        case CMD_YEAR:  
            // Value holds year after 2000, Modifier holds timezone

            // Date might be a timestamp for one of the temp sensors
            regularDT = true;
            for (uint8_t i=0; i<NUM_TEMP_SENSORS; i++)
            {
                if (i == TS_INTERNAL) { ts = &InternalTemp; }
                if (i == TS_EXTERNAL) { ts = &ExternalTemp; }
                if (i == TS_AUX)      { ts = &AuxTemp;      }            
                if      (ts->updateMinDT_Flag) { ts->allTimeMinDT.year = sentence->Value; ts->allTimeMinDT.timezone = sentence->Modifier; regularDT = false; }
                else if (ts->updateMaxDT_Flag) { ts->allTimeMaxDT.year = sentence->Value; ts->allTimeMaxDT.timezone = sentence->Modifier; regularDT = false; }
            }
            
            // Ok, this is just the regular time
            if (regularDT) 
            {
                DT.year = sentence->Value;
                DT.timezone = sentence->Modifier;
            }

            // Only update on hour/minute
            break;

        case CMD_MONTH_DAY:
            // Value holds month (1-12), Modifier holds day (0-31)

            // Date might be a timestamp for one of the temp sensors
            regularDT = true;
            for (uint8_t i=0; i<NUM_TEMP_SENSORS; i++)
            {
                if (i == TS_INTERNAL) { ts = &InternalTemp; }
                if (i == TS_EXTERNAL) { ts = &ExternalTemp; }
                if (i == TS_AUX)      { ts = &AuxTemp;      }            
                if      (ts->updateMinDT_Flag) { ts->allTimeMinDT.month = sentence->Value; ts->allTimeMinDT.day = sentence->Modifier; regularDT = false; }
                else if (ts->updateMaxDT_Flag) { ts->allTimeMaxDT.month = sentence->Value; ts->allTimeMaxDT.day = sentence->Modifier; regularDT = false; }
            }

            // Ok, this is just the regular time
            if (regularDT)
            {
                DT.month = sentence->Value;
                DT.day = sentence->Modifier;
            }
            
            // Only update on hour/minute
            break;            
    
        case CMD_HOUR_MINUTE: 
            // Value holds hour (0-23), Modifier holds minute (0-59)

            // Date might be a timestamp for one of the temp sensors
            regularDT = true;
            for (uint8_t i=0; i<NUM_TEMP_SENSORS; i++)
            {
                if (i == TS_INTERNAL) { ts = &InternalTemp; }
                if (i == TS_EXTERNAL) { ts = &ExternalTemp; }
                if (i == TS_AUX)      { ts = &AuxTemp;      }            
                if      (ts->updateMinDT_Flag) { ts->allTimeMinDT.hour = sentence->Value; ts->allTimeMinDT.minute = sentence->Modifier; ts->updateMinDT_Flag = false; regularDT = false; }  // Also clear updateDT flag
                else if (ts->updateMaxDT_Flag) { ts->allTimeMaxDT.hour = sentence->Value; ts->allTimeMaxDT.minute = sentence->Modifier; ts->updateMaxDT_Flag = false; regularDT = false; }  // Also clear updateDT flag
            }            

            // Ok, this is just the regular time
            if (regularDT)
            {
                DT.hour = sentence->Value;
                DT.minute = sentence->Modifier;
                displayElement.setDataFlag(gde_DateTime);   // Update
            }
            break;

        case CMD_VOLTAGE:
            if (volts != sentence->Value)
            {
                Voltage = float(sentence->Value) / 10.0;    // Actual voltage
                volts = sentence->Value;                    // Just used to check for changes                
                if (!lowVoltage && sentence->Value <= LOW_VOLTAGE)
                {   // We've just dropped below the low voltage threshold
                    lowVoltage = true;
                    TimerID_VoltageWarning = timer.setInterval(400, RenderVoltage);
                }
                if (lowVoltage && sentence->Value > LOW_VOLTAGE)
                {   // We've just risen above the low voltage threshold
                    lowVoltage = false;
                    if (TimerID_VoltageWarning > 0) 
                    {
                        timer.deleteTimer(TimerID_VoltageWarning);
                        TimerID_VoltageWarning = 0;
                    }
                }
                displayElement.setDataFlag(gde_Voltage);
            }
            break;

        case CMD_LOW_AIR_WARN:
            // Low air warning (suspension air tank). Value = 1 if Low, 0 = if OK
            if (LowAirWarning != sentence->Value)
            {
                LowAirWarning = sentence->Value;
                displayElement.setDataFlag(gde_Air);
            }
            break;

        case CMD_FUEL_PUMP:
            if (FuelPump != sentence->Value)
            {
                FuelPump = sentence->Value;
                displayElement.setDataFlag(gde_FuelPump);
            }
            break;

        case CMD_HAM_ON:
            if (Ham_On != sentence->Value)
            {
                Ham_On = sentence->Value;
                displayElement.setDataFlag(gde_Radio);
            }
            break;

        case CMD_TQC_LOCK_STATUS:
            // Torque converter lockup status controlled by Baumann controller
            if (TQCLockStatus != sentence->Value)
            {   
                TQCLockStatus = sentence->Value;
                displayElement.setDataFlag(gde_Transmission);
            }
            break;
            
        case CMD_OVERDRIVE:
            // Overdrive enabled or not
            if (OverdriveEnabled != sentence->Value)
            {
                OverdriveEnabled = sentence->Value;
                displayElement.setDataFlag(gde_Transmission);
            }
            break;
            
        case CMD_TRANS_TABLE:
            // Baumann using Table 1 (default) or Table 2 (alternate) transmission settings
            if (BaumannTable != sentence->Value)
            {
                BaumannTable = sentence->Value;
                displayElement.setDataFlag(gde_Transmission);
            }
            break;

        case CMD_ACTION_TAKEN:
            // This is a response back from the Mega letting us know it received our request to do something. 
            // We can set a flag on whatever the current menu item is to let us know it was successful.
            Menu[currentMenu].success = true;
            break;

    }
}




