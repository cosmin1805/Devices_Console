#include "tft_screen_display.h"
#include "tft_screen_touch.h"
#include "bitmaps.h"

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

button_c button_c;
char *system_selected = "OG";

void text_center(const char *buf, int x, int y, int w, int h) {
  int16_t x1, y1;
  uint16_t w_text, h_text;
  tft.getTextBounds(buf, w, h, &x1, &y1, &w_text, &h_text);
  tft.setCursor((w - w_text) / 2 + x, (h - h_text) / 2 + y);
  tft.print(buf);
}

void display::init() {
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3) ID = 0x9486;  // write-only shield
  tft.begin(ID);
  tft.invertDisplay(true);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
}

// HIGHLIGHT THE BUTTON
void display::show_selected_button(button b) {
  if (b.name == "wol") {
    tft.fillRect(96, 176, 288, 90, GREEN);
    tft.drawRect(96, 176, 288, 90, WHITE);
    tft.setTextSize(4);
    text_center("WAKE UP", 96, 176, 288, 90);
  } else {
    tft.setTextSize(3);
    tft.fillRect(b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y, GREEN);
    tft.drawRect(b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y, WHITE);
    text_center(b.display_name, b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y);
  }
}
// UNHIGHLIGHT THE BUTTON
void display::reset_selected_button(button b) {

  if (b.name == "wol") {
    tft.fillRect(96, 176, 288, 90, BLACK);
    tft.drawRect(96, 176, 288, 90, WHITE);
    tft.setTextSize(4);
    text_center("WAKE UP", 96, 176, 288, 90);
  } else {
    tft.setTextSize(3);
    tft.fillRect(b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y, BLACK);
    tft.drawRect(b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y, WHITE);
    text_center(b.display_name, b.pos1.x, b.pos1.y, b.pos2.x - b.pos1.x, b.pos2.y - b.pos1.y);
  }
}

void menu::inactive() {
  tft.writeFillRect(0, 70, 480, 250, BLACK);
  button_c.change_index(5);
  tft.setTextSize(3);
  tft.setCursor(105, 103);
  tft.print("Status:");
  tft.setTextColor(RED);
  tft.print("INACTIVE");
  tft.setTextColor(WHITE);
  tft.drawRect(96, 176, 288, 90, WHITE);
  tft.setTextSize(4);
  text_center("WAKE UP", 96, 176, 288, 90);
  button_c.add(96, 176, 384, 266, "wol", "WAKE UP", "command");
}

void menu::active(char *stats[], char *display_options[], char *options[], int16_t x_pos1, int16_t y_pos1, int16_t w, int16_t h, int count) {
  tft.writeFillRect(0, 70, 480, 250, BLACK);
  button_c.change_index(5);
  tft.setTextSize(3);
  int i = 0;
  for (int16_t x = x_pos1; x <= w * (count - 1); x += w) {
    tft.drawRect(x, y_pos1, w, h, WHITE);
    if (system_selected == "PC" && i == 0) {
      text_center("GPU:99\367C", x, y_pos1, w, h);
    } else {
      text_center(stats[i], x, y_pos1, w, h);
    }
    i++;
  };
  int y_difference_from_stats = 126;
  tft.drawBitmap(220, y_pos1 + 70 + 8, icon, 40, 40, WHITE);
  i = 0;
  for (int16_t x = x_pos1; x <= w * (count - 1); x += w) {
    //THE PERFECT FITTING Y ONE CALCULATED IS y_difference_from_stats= 125
    tft.drawRect(x, y_pos1 + y_difference_from_stats, w, h, WHITE);
    text_center(display_options[i], x, y_pos1 + y_difference_from_stats, w, h);
    //ADD THE button_cS
    button_c.add(x, y_pos1 + y_difference_from_stats, x + w, y_pos1 + y_difference_from_stats + h, options[i], display_options[i], "command");
    i++;
  }
}

//MODIFY THE MENU ACTIVE STATS
void menu::stats(String cpu_temp, String cpu_usage, String memory_usage, int16_t x_pos1, int16_t y_pos1, int16_t w, int16_t h, int count) {
  tft.setTextSize(3);
  int i = 0;
  for (int16_t x = x_pos1; x <= w * (count - 1); x += w) {
    tft.setCursor(80 + x, (h - 24) / 2 + y_pos1);
    if (i == 0) {
      tft.writeFillRect(x + 76, y_pos1 + 5, 40, h - 10, BLACK);
      tft.print(cpu_temp);
    } else if (i == 1) {
      tft.writeFillRect(x + 76, y_pos1 + 5, 55, h - 10, BLACK);
      tft.print(cpu_usage);
    } else if (i == 2) {
      tft.writeFillRect(x + 76, y_pos1 + 5, 55, h - 10, BLACK);
      tft.print(memory_usage);
    }
    i++;
  };
}

void nav_bar::draw(char *display_names[], char *names[], int16_t x_pos1, int16_t y_pos1, int16_t w, int16_t h, int count) {
  tft.setTextSize(4);
  int i = 0;
  for (int16_t x = x_pos1; x <= w * (count - 1); x += w) {
    tft.drawRect(x, y_pos1, w, h, WHITE);
    text_center(display_names[i], x, y_pos1, w, h);
    //ADD THE button_cS
    button_c.add(x, y_pos1, x + w, y_pos1 + h, names[i], display_names[i], "change");
    i++;
  }
}

void nav_bar::selected(int16_t id, char *display_names[]) {
  tft.setTextSize(4);
  button b = button_c.get(id);
  for (int i = 0; i <= 4; i++) {
    button temp = button_c.get(i);
    if (temp.name == system_selected) {
      int x = temp.pos1.x;
      int y = temp.pos1.y;
      tft.writeFillRect(x, y, 96, 70, BLACK);
      tft.drawRect(x, y, 96, 70, WHITE);
      text_center(temp.display_name, x, y, 96, 70);
      break;
    }
  }
  system_selected = b.name;
  int x = b.pos1.x;
  int y = b.pos1.y;
  tft.writeFillRect(x, y, 96, 70, GREEN);
  tft.drawRect(x, y, 96, 70, WHITE);
  text_center(b.display_name, x, y, 96, 70);
}