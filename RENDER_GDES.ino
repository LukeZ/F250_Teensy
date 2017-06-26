
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

#define	ILI9340_BLACK   0x0000
#define	ILI9340_BLUE    0x001F
#define	ILI9340_RED     0xF800
#define	ILI9340_GREEN   0x07E0
#define ILI9340_CYAN    0x07FF
#define ILI9340_MAGENTA 0xF81F
#define ILI9340_YELLOW  0xFFE0  
#define ILI9340_WHITE   0xFFFF

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
//          tft.fillScreen(ILI9340_BLACK);
            tft.fillCircle(x, y, r_inner, color);
            tft.drawCircle(x, y, r_outer, color);
            tft.drawCircle(x, y, r_outer + 1, color);    // We draw two outer circles to give the line double width
            break;
    }
        
    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GPS);

    return;
}



