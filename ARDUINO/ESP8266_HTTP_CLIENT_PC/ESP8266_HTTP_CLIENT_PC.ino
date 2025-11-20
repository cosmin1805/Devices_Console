//VARIABLES FOR WEB_DATA
#include <ArduinoJson.h>

StaticJsonDocument<200> hosts_ip;
StaticJsonDocument<192> hosts_mac;
String system_selected = "OG";

//WIFI THINGS AND HTTP CLIENT

#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#ifndef STASSID
#define STASSID "Asus_Smi"
#define STAPSK "g5cqBh9K"
#endif

ESP8266WiFiMulti WiFiMulti;

HTTPClient http;
WiFiClient client;

//WOL THINGS
#include <WiFiUdp.h>
#include <WakeOnLan.h>
WiFiUDP UDP;
WakeOnLan WOL(UDP);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  hosts_ip["OG"] = "192.168.50.2";
  hosts_ip["PC"] = "192.168.50.3";
  hosts_ip["OMV"] = "192.168.50.4";
  hosts_ip["NUC-I3"] = "192.168.50.5";
  hosts_ip["NUC-I5"] = "192.168.50.6";

  hosts_mac["OG"] = "1c:87:2c:5f:ac:8c";
  hosts_mac["PC"] = "18:c0:4d:ec:eb:1e";
  hosts_mac["OMV"] = "1c:87:2c:5f:ac:8c";
  hosts_mac["NUC-I3"] = "c0:3f:d5:65:8a:2a";
  hosts_mac["NUC-I5"] = "1c:69:7a:05:6c:b6";
  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(STASSID, STAPSK);

  // wait for WiFi connection
  while ((WiFiMulti.run() != WL_CONNECTED)) {
    //Serial.write('.');
    delay(500);
  }
  //Serial.println(" connected to WiFi");

  // allow reuse (if server supports it)
  http.setReuse(false);

}


void http_get_stats() {
  String ip = hosts_ip[system_selected];
  http.begin(client, ip, 2000, "/stats");
  int httpCode = http.GET();
  if (httpCode > 0) {
    //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      http.writeToStream(&Serial);
    }
  } else {
    //Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    // Something went wrong with the connection, try to reconnect
    //http.end();
    //String ip = hosts_ip[system_selected];
    Serial.write("{inactive}");
    //http.begin(client, ip, 2000, "/stats");
  }
  http.end();
}

void arduino_command(String command) {
  if (command == "wol") {
    String MACAddress = hosts_mac[system_selected];
    WOL.sendMagicPacket(MACAddress);
  } else {
    http.end();
    String ip = hosts_ip[system_selected];
    http.begin(client, ip, 2000, "/" + command);
    int httpCode = http.GET();
    http.end();
    //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  }
}

long long delay_t = 1500;
unsigned long long curent_millis = millis();

void loop() {
  if (Serial.available()) {
    String data = Serial.readString();
    int index = data.indexOf(",");
    String action = data.substring(0, index);
    String command = data.substring(index + 1, data.length());
    if (action == "command") {
      arduino_command(command);
    } else if (action == "change") {
      system_selected = command;
      http.end();
      String ip = hosts_ip[system_selected];
      http.begin(client, ip, 2000, "/" + command);
    }
  }
  if (millis() - delay_t >= curent_millis) {
    http_get_stats();
    curent_millis = millis();
  }
}