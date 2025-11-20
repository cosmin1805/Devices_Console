#include "tft_screen_display.h"
#include "tft_screen_touch.h"
#include <ArduinoJson.h>

touch_s touch_s;
display display;
menu menu;
nav_bar nav_bar;

button_c b;

bool active = false;
char *stats[3] = { "CPU:99\367C", "CPU:100%", "RAM:100%" };
char *display_options[3] = { "SHUTDOWN", "RESTART", "MORE" };
char *options[3] = { "shutdown", "reboot", "more" };
char *display_names[5] = { "OG", "PC", "OMV", "I3", "I5" };
char *names[5] = { "OG", "PC", "OMV", "NUC-I3", "NUC-I5" };

//THE LAST BUTTON CLICKED
button last_button;
//IF WE RESETED THE LAST BUTTON TOUCHED
bool reset = true;

void setup(void) {
  Serial.begin(115200);
  display.init();
  nav_bar.draw(display_names, names, 0, 0, 96, 70, 5);
  nav_bar.selected(0, display_names);
  menu.inactive();
  last_button.name = "";
}

long long delay_t = 1500;
unsigned long long curent_millis = millis();

void loop(void) {
  
  int16_t id = touch_s.check(); //GET THE BUTTON ID IF DISPLAY IS TOUCHED
  if (id != -1 && reset == true) {
    curent_millis = millis();
    button button = b.get(id);
    
    if (button.name != last_button.name) { //IF THE BUTTON IS NOT THE SAME AS THE ONE THAT THE USER JUST CLICKED
      last_button = button;
      char buf[40];
      strcpy(buf, button.command);
      strcat(buf, { "," });
      strcat(buf, button.name);
      Serial.write(buf);
      if (button.command == "change") {
        nav_bar.selected(id, display_names); //CHANGE THE NAV_BAR IF A BUTTON OF IT IS PRESED
      } else {
        reset = false;
        display.show_selected_button(button); //IF IT IS NOT A NAV_BAR BUTTON HIGHLIGHT IT
      }
    }
  }
  if (millis() - delay_t >= curent_millis) { //RESET THE LAST BUTTON TO NONE AFTER delay_t TIME
    if (reset == false && last_button.command != "change") { //RESET TO NORMAL STATE IF IT WAS'N RESETED AND IF THE BUTTON PRESSED IS NOT PART OF THE NAV_BAR
      display.reset_selected_button(last_button);
    }
    last_button.name = ""; //RESETING THE BUTTON
    reset = true;
  }

  if (Serial.available()) { //IF THE ESP8266 IS COMMUNICATING GET THE DATA
    String data = Serial.readString();
    //menu.console(data);
    if (data == "{inactive}") { //IF THE DATA TELLS US THAT IT IS INACTIVE SET THE DISPLAY TO BE INACTIVE IF IT HASN'T BEEN DONE BEFORE
      if (active == true) {
        menu.inactive();
        active = false;
      }
    } else {
      if (active == false) { //IF THE DATA TELLS US THAT IT IS ACTIVE SET THE DISPLAY TO BE ACTIVE IF IT HASN'T BEEN DONE BEFORE
        menu.active(stats, display_options,options, 0, 97, 160, 70, 3);
        active = true;
      }
      //GET THE DATA IN A NICE JSON FORMAT
      StaticJsonDocument<96> doc;
      DeserializationError error = deserializeJson(doc, data);
      if (error) {
        return;
      }
      String memory_usage = doc["memory_usage"];  // 100.5
      String cpu_usage = doc["cpu_usage"];        // 100.5
      String cpu_temp = doc["cpu_temp"];          // 48.5
      //MODIFY THE JSON TO FIT ON THE SCREEN
      if(memory_usage.toInt()>=10)
      {
          memory_usage.remove(memory_usage.length()-2,2);
      }
      else
      {
          memory_usage = memory_usage.substring(0,3);
      }
      if(cpu_usage.toInt()>=10)
      {
          cpu_usage.remove(cpu_usage.length()-2,2);
      }
      else
      {
          cpu_usage = cpu_usage.substring(0,3);
      }
      //UPDATE THE STATS ON THE DISPLAY
      menu.stats(cpu_temp, cpu_usage , memory_usage, 0, 97, 160, 70, 3);
    }
  }

}
