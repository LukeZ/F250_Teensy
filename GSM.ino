
//=============================================================================================================================================== >> 
// GSM
//=============================================================================================================================================== >> 

void RenderGSM()
{
int x = 0;
int y = 0;  
#define GSM_BAR             0x0679          // 0, 204, 204
#define GSM_NOT_BAR         COLOR_DESELECT
#define BAR_SPACE           8
#define BAR_WIDTH           5
uint16_t bar_color;
uint16_t final_color;

    switch (currentScreen)
    {
        case SCREEN_AUTO:
        case SCREEN_ALTITUDE:
        case SCREEN_TEMPERATURE:
            x = OX + 270;
            y = OY + 11;
            break;

        default:        // Do nothing
            // Now we've displayed the info, we can clear this element
            displayElement.clearDataFlag(gde_GSM);        
            return;
            break;  
    }

    nightTime ? bar_color = NightColor : bar_color = GSM_BAR;
    GSM_Bars > 0 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x, y, BAR_WIDTH, BAR_WIDTH, final_color); 
    GSM_Bars > 1 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+BAR_SPACE, y-BAR_WIDTH, BAR_WIDTH, BAR_WIDTH*2, final_color);
    GSM_Bars > 2 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+(BAR_SPACE*2), y-(BAR_WIDTH*2), BAR_WIDTH, BAR_WIDTH*3, final_color);
    GSM_Bars > 3 ? final_color = bar_color : final_color = GSM_NOT_BAR; 
        tft.fillRect(x+(BAR_SPACE*3), y-(BAR_WIDTH*3), BAR_WIDTH, BAR_WIDTH*4, final_color);

    // Now we've displayed the info, we can clear this element
    displayElement.clearDataFlag(gde_GSM);

}


