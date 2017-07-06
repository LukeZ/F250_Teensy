
/*
gde_Transmission
gde_Temperature
gde_Altitude
gde_GPS
gde_Speed
_Heading
gde_Radio
gde_FuelPump
_Alarm 
gde_Air 
gde_Ign
gde_Voltage

// #define ILI9341_BLACK       0x0000         0,   0,   0 
// #define ILI9341_NAVY        0x000F         0,   0, 128
// #define ILI9341_DARKGREEN   0x03E0         0, 128,   0 
// #define ILI9341_DARKCYAN    0x03EF         0, 128, 128 
// #define ILI9341_MAROON      0x7800       128,   0,   0 
// #define ILI9341_PURPLE      0x780F       128,   0, 128 
// #define ILI9341_OLIVE       0x7BE0       128, 128,   0 
// #define ILI9341_LIGHTGREY   0xC618       192, 192, 192 
// #define ILI9341_DARKGREY    0x7BEF       128, 128, 128 
// #define ILI9341_BLUE        0x001F         0,   0, 255 
// #define ILI9341_GREEN       0x07E0         0, 255,   0 
// #define ILI9341_CYAN        0x07FF         0, 255, 255 
// #define ILI9341_RED         0xF800       255,   0,   0 
// #define ILI9341_MAGENTA     0xF81F       255,   0, 255 
// #define ILI9341_YELLOW      0xFFE0       255, 255,   0 
// #define ILI9341_WHITE       0xFFFF       255, 255, 255 
// #define ILI9341_ORANGE      0xFD20       255, 165,   0 
// #define ILI9341_GREENYELLOW 0xAFE5       173, 255,  47 
// #define ILI9341_PINK        0xF81F

*/

#define GPS_GREEN        0x07E0
#define GPS_RED          0xF800

void RenderGPS()
{
    int x, y;
    uint16_t color;
    // size of circles
    int r_inner = 8;
    int r_outer = 12;

    // If we hae a GPS fix, the circles are green, otherwise red.
    GPS_FIX ? color = GPS_GREEN : color = GPS_RED;

    switch (currentScreen)
    {
        case 1:
            x = 100;
            y = 100;
//            tft.fillScreen(ILI9340_BLACK);
            tft.fillCircle(x, y, r_inner, color);
            tft.drawCircle(x, y, r_outer, color);
            tft.drawCircle(x, y, r_outer + 1, color);    // We draw two outer circles to give the line double width
            break;
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GPS);

    return;
}



