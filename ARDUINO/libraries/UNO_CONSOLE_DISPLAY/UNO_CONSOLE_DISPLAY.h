#ifndef UNO_CONSOLE_DISPLAY_h
#define UNO_CONSOLE_DISPLAY_h

#include <buttons_list.h>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

class display
{
    private:
        MCUFRIEND_kbv tft;
        String selectedSystem;

        buttons_list nav_bar[5];
        buttons_list menu_inactive[1]={"wol","WAKE UP",96, 176, 384, 266,false,"command"};  
        buttons_list menu_active[5];
        
    public:
        display(String sys);
        //display
        void init();
        void centerText(const char *displayText, int x, int y, int w, int h);
        //menus
        void menuInactive();
        //nav bar
        void navBarDraw();
        //buttons
        void buttonsInit();
};
#endif