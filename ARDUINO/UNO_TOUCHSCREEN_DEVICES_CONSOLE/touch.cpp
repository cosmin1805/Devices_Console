#include "tft_screen_touch.h"

#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int16_t XP = 9, XM = A3, YP = A2, YM = 8;  //320x480 ID=0x0099
const int16_t TS_LEFT = 188, TS_RT = 928, TS_TOP = 963, TS_BOT = 201;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

button buttons[9];
int16_t index = 0;
//ADDING A TOUCH BUTTON TO THE ARRAY
void button_c::add(int16_t x1, int16_t y1, int16_t x2, int16_t y2, char *name,char *display_name, char *command) {
  buttons[index].pos1.x = x1;
  buttons[index].pos1.y = y1;

  buttons[index].pos2.x = x2;
  buttons[index].pos2.y = y2;

  buttons[index].name = name;
  buttons[index].display_name = display_name;
  buttons[index].command = command;
  index++;
};
//GET BUTTON BY INDEX
button button_c::get(int16_t id) {
  return buttons[id];
};
//CHANGE THE INDEX SO IT EXCLUDES THE BUTTONS FROM THE OTHER MENU (THIS IS DONE TO PREVENT "UNSEEN BUTTONS" FROM BEING CLICKED)
void button_c::change_index(int16_t n_index) {
  index = n_index;
};

//CHECK IF A BUTTON IS PRESSED AND RETURN ITS INDEX IF THE TOUCHED AREA IS A BUTTON
int button_check(int x, int y) {
  for (int i = 0; i <= index-1; i++) {
    if ((x > buttons[i].pos1.x && x < buttons[i].pos2.x) && (y > buttons[i].pos1.y && y < buttons[i].pos2.y)) {
      return i;
    }
  }
  return -1;
}
//CHECK IF THE TOUCH IS PRESSED AND IF THE AREA IS A BUTTON
int16_t touch_s::check()
{
  TSPoint p = ts.getPoint();
  pinMode(YP, OUTPUT);  //restore shared pins
  pinMode(XM, OUTPUT);
  digitalWrite(YP, HIGH);  //because TFT control pins
  digitalWrite(XM, HIGH);
  bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
  int id = -1;
  if(pressed) {
    int pixel_x = map(p.y, 963, 201, 0, 480);
    int pixel_y = map(p.x, 928, 188, 0, 320);
    id = button_check(pixel_x, pixel_y);
  }
  return id;
}
