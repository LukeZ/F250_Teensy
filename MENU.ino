
//=============================================================================================================================================== >> 
// MENU
//=============================================================================================================================================== >> 
#define MENU_ROW_OFFSET         30
#define MENU_WIDTH_SELECT_BOX   190
#define MENU_WIDTH_ENTER_BOX    300

void RenderMenu()
{
int x = 0;
int y = 0;
uint8_t rows = 0;
uint8_t menuNum;
uint16_t color;
int8_t topMenu;
#define NUM_MENUS_ABOVE_BELOW_SELECTED    3   // How many menus appear above and below the selected menu
    
    switch (currentScreen)
    {
        case SCREEN_MENU:
            x = OX + 5;
            y = OY + 10;
            
            // Font and color
            tft.setFont(Arial_16_Bold);
            nightTime ? color = TEXT_COLOR_NIGHT : color = ILI9341_WHITE;
            ClearScreen();

            // Initialize loop list
            topMenu = currentMenu - NUM_MENUS_ABOVE_BELOW_SELECTED;
            if (topMenu < 0) topMenu = NUM_MENUS + topMenu;     // Loop back
            menuNum = topMenu;
            rows = 0;

            // Create list
            for (uint8_t i=0; i<((NUM_MENUS_ABOVE_BELOW_SELECTED*2)+1); i++)
            {
                if (menuNum >= NUM_MENUS) menuNum = 0;          // Loop
                
                tft.setCursor(x, y + (rows * MENU_ROW_OFFSET));       // Print menu
                Menu[menuNum].enabled ? tft.setTextColor(color) : tft.setTextColor(COLOR_DESELECT);
                tft.print(menuName[menuNum]);

                if (Menu[menuNum].complete == true && Menu[menuNum].entered == false)
                {
                    tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 2, y + (rows * MENU_ROW_OFFSET));
                    if (Menu[menuNum].success)
                    {
                        tft.setTextColor(ILI9341_GREEN);
                        tft.print(" Success");
                    }
                    else
                    {
                        tft.setTextColor(ILI9341_RED);
                        tft.print(" Error");
                    }
                }
                rows += 1;
                menuNum += 1;
            }

            // If we have entered the item, expand the selection box
            Menu[currentMenu].entered ? DrawMenuEnterBox(x, y) : DrawMenuSelectBox(x, y);

            // Is there some message or item to display? 
            if (Menu[currentMenu].entered)
            {
                switch (currentMenu)
                {   // All these are Yes / No actions so we can deal with them the same
                    case MENU_SET_ALT_TO_GPS:
                    case MENU_SET_HOME_COORD: 
                    case MENU_CLEAR_ALLTIME_TEMP_I: 
                    case MENU_CLEAR_ALLTIME_TEMP_E: 
                    case MENU_CLEAR_ALLTIME_TEMP_A:                 
                         DrawYesNoMenuItems(x, y, Menu[currentMenu].val_YN);
                        break;
    
                    case MENU_SET_TIMEZONE: 
                        DrawTimeZoneSelections(x, y, DT.timezone);
                        break;
                        
                    case MENU_SET_ALT: 
                    case MENU_SET_HOME_ALT: 
                            tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 3, y + (3 * MENU_ROW_OFFSET));
                            tftPrintWithComma(Menu[currentMenu].val_Int);
                        break;
                        
                    case MENU_SET_DEFAULT_SCREEN: 
                        break;
    
                }
            }            

            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_Menu);
            break;

        default:        // Do nothing
            displayElement.clearDataFlag(gde_Menu);
            return;
            break;  

    }
}



void DrawMenuSelectBox(int x, int y)
{
    ClearMenuEnterBox(x, y);
    tft.drawRect(x - 5, y + (3 * MENU_ROW_OFFSET) - 7, MENU_WIDTH_SELECT_BOX,   31, COLOR_DARK_YELLOW );
    tft.drawRect(x - 4, y + (3 * MENU_ROW_OFFSET) - 6, MENU_WIDTH_SELECT_BOX-2, 29, COLOR_DARK_YELLOW );
}

void ClearMenuSelectBox(int x, int y)
{
    tft.drawRect(x - 5, y + (3 * MENU_ROW_OFFSET) - 7, MENU_WIDTH_SELECT_BOX,   31, CurrentBackgroundColor );
    tft.drawRect(x - 4, y + (3 * MENU_ROW_OFFSET) - 6, MENU_WIDTH_SELECT_BOX-2, 29, CurrentBackgroundColor );
}

void DrawMenuEnterBox(int x, int y)
{
    ClearMenuSelectBox(x, y);
    tft.drawRect(x - 5, y + (3 * MENU_ROW_OFFSET) - 7, MENU_WIDTH_ENTER_BOX,   31, COLOR_DARK_YELLOW );
    tft.drawRect(x - 4, y + (3 * MENU_ROW_OFFSET) - 6, MENU_WIDTH_ENTER_BOX-2, 29, COLOR_DARK_YELLOW );    
}

void ClearMenuEnterBox(int x, int y)
{
    tft.drawRect(x - 5, y + (3 * MENU_ROW_OFFSET) - 7, MENU_WIDTH_ENTER_BOX,   31, CurrentBackgroundColor );
    tft.drawRect(x - 4, y + (3 * MENU_ROW_OFFSET) - 6, MENU_WIDTH_ENTER_BOX-2, 29, CurrentBackgroundColor );    
}

void DrawYesNoMenuItems(int x, int y, boolean YN)
{
uint16_t color;

    tft.setFont(Arial_16_Bold);
    nightTime ? color = TEXT_COLOR_NIGHT : color = ILI9341_WHITE;

    if (YN)
    {
        // Yes is selected
        tft.fillRect(x + MENU_WIDTH_SELECT_BOX, y + (3 * MENU_ROW_OFFSET) - 7, 50, 31, COLOR_DARK_YELLOW);
        tft.setTextColor(CurrentBackgroundColor);
        tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 2, y + (3 * MENU_ROW_OFFSET));
        tft.print("YES");
        // No is not
        tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 60, y + (3 * MENU_ROW_OFFSET));
        tft.setTextColor(color);
        tft.print("NO");
    }
    else
    {
        // Yes is not selected
        tft.setTextColor(color);
        tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 2, y + (3 * MENU_ROW_OFFSET));
        tft.print("YES");
        // No is selected
        tft.fillRect(x + MENU_WIDTH_SELECT_BOX + 55, y + (3 * MENU_ROW_OFFSET) - 7, 40, 31, COLOR_DARK_YELLOW);
        tft.setCursor(x + MENU_WIDTH_SELECT_BOX + 60, y + (3 * MENU_ROW_OFFSET));
        tft.setTextColor(CurrentBackgroundColor);
        tft.print("NO");
    }
}

void DrawTimeZoneSelections(int x, int y, uint8_t tz)
{
uint16_t color;
int startX;
int startY;
#define tzOffset 20

    tft.setFont(Arial_16_Bold);
    nightTime ? color = TEXT_COLOR_NIGHT : color = ILI9341_WHITE;

    const char* tzName[NUM_TIMEZONES] = {"A", "P", "M", "C", "E"};  // Alaska, Pacific, Mountain, Central, Eastern

    startX = x + MENU_WIDTH_SELECT_BOX;
    startY = y + (3 * MENU_ROW_OFFSET);
    
    for (uint8_t i=0; i<NUM_TIMEZONES; i++)
    {
        if (i == tz)    // Highlighted entry
        {
            tft.fillRect(startX + (tzOffset * i) - 2, startY - 7, 20, 31, COLOR_DARK_YELLOW);
            tft.setTextColor(CurrentBackgroundColor);
        }
        else            // Not-selected entry
        {
            tft.setTextColor(color);            
        }
        tft.setCursor(startX + (tzOffset * i), startY);
        tft.print(tzName[i]);
    }
}

void SetupMenus()
{
    for (uint8_t i=0; i<NUM_MENUS; i++)
    {
        Menu[i].enabled = true;     // Enable menu item
        Menu[i].entered = false;    // But we are not presently in it
        Menu[i].val_YN = false;     // Default to "No"
        Menu[i].val_Int = 0;        // Default value to 0
        Menu[i].complete = false;   // Action not yet complete
        Menu[i].success = false;    // Action not yet successful
        Menu[i].cmdToMega = 0;      // 
        Menu[i].valueToMega = 0;    // 
        Menu[i].modifierToMega = 0; // 
    }

    // But alter a few do by hand
    Menu[MENU_SET_ALT_TO_GPS].enabled = false;          // don't enable until we have a fix
    Menu[MENU_SET_HOME_COORD].enabled = false;          // don't enable until we have a fix


    // And each have to have the Mega command set manually: 
    Menu[MENU_SET_ALT_TO_GPS].cmdToMega         = RCV_CMD_SET_GPS_ALT;
    Menu[MENU_SET_ALT].cmdToMega                = RCV_CMD_SET_CURRENT_ALT;
    Menu[MENU_SET_HOME_COORD].cmdToMega         = RCV_CMD_SET_HOME_COORD;
    Menu[MENU_SET_HOME_ALT].cmdToMega           = RCV_CMD_SET_HOME_ALT;
    Menu[MENU_SET_DEFAULT_SCREEN].cmdToMega     = 0;                            // This is an internal setting
    Menu[MENU_SET_TIMEZONE ].cmdToMega          = RCV_CMD_SET_TIMEZONE;
    Menu[MENU_CLEAR_ALLTIME_TEMP_I].cmdToMega   = RCV_CMD_RESET_ABS_TEMP;
        Menu[MENU_CLEAR_ALLTIME_TEMP_I].modifierToMega   = TS_INTERNAL;
    Menu[MENU_CLEAR_ALLTIME_TEMP_E].cmdToMega   = RCV_CMD_RESET_ABS_TEMP;
        Menu[MENU_CLEAR_ALLTIME_TEMP_E].modifierToMega   = TS_EXTERNAL;
    Menu[MENU_CLEAR_ALLTIME_TEMP_A].cmdToMega   = RCV_CMD_RESET_ABS_TEMP;
        Menu[MENU_CLEAR_ALLTIME_TEMP_A].modifierToMega   = TS_AUX;
}




