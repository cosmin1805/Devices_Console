#ifndef tft_screen_touch_h
#define tft_screen_touch_h
#include <Adafruit_GFX.h>

struct pos1 {
  int x;
  int y;
};
struct pos2 {
  int x;
  int y;
};
struct button {
  pos1 pos1;
  pos2 pos2;
  char *name;
  char *display_name;
  char *command;
};

class button_c
{
  public:
    void add(int x1, int y1, int x2, int y2, char *name,char *display_name, char *command);
    button get(int id);
    void change_index(int n_index);
};

class touch_s
{
  public:
    int check();

};
#endif