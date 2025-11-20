#include <UNO_CONSOLE_DISPLAY.h>
#include <buttons_list.h>

display::display(String sys)
{
  selectedSystem = sys;
}

void display::init()
{
  uint16_t ID = tft.readID();
  if (ID == 0xD3D3)
  {
    ID = 0x9486; // write-only shield
  }
  tft.begin(ID);
  tft.invertDisplay(true);
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
}

void display::centerText(const char *displayText, int x, int y, int w, int h)
{
  //GET THE TEXT SIZE 
  int16_t x1, y1;
  uint16_t w_text=0, h_text=0;
  tft.getTextBounds(displayText, w, h, &x1, &y1, &w_text, &h_text);
  //SET THE CURSOR TO THE CENTER OF THE RECT TO PRINT TEXT
  tft.setCursor((w - w_text) / 2 + x, (h - h_text) / 2 + y);
  tft.print(displayText);
}
  
void display::navBarDraw()
{

}

void display::menuInactive()
{
  tft.writeFillRect(0, 70, 480, 250, BLACK);
  tft.setTextSize(3);
  tft.setCursor(105, 103);
  tft.print("Status:");
  tft.setTextColor(RED);
  tft.print("INACTIVE");
  tft.setTextColor(WHITE);
  tft.drawRect(96, 176, 288, 90, WHITE);
  tft.setTextSize(4);
  centerText("WAKE UP", 96, 176, 288, 90);
}

