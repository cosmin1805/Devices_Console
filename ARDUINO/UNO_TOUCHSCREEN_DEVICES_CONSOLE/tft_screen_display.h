#ifndef tft_screen_display_h
#define tft_screen_display_h
#include "tft_screen_touch.h"
#include <Adafruit_GFX.h>

class display {
public:
  void init();
  void show_selected_button(button b);
  void reset_selected_button(button b);
};

class menu {
public:
  void inactive();
  void active(char *stats[], char *display_options[], char *options[], int16_t x_p, int16_t y_p, int16_t w, int16_t h, int count);
  void stats(String cpu_temp, String cpu_usage, String memory_usage, int16_t x_f, int16_t y_f, int16_t w, int16_t h, int count);
};

class nav_bar {
public:
  void draw(char *display_names[], char *names[], int16_t x_f, int16_t y_f, int16_t w, int16_t h, int16_t count);
  void selected(int16_t id, char *display_names[]);
};

#endif