#include <ESP8266WiFi.h>

// card initialization ESP-01
void initESP (void);
bool loadConfig(String *ssid, String *pass, String *name);
bool saveConfig(String *ssid, String *pass, String *name);
