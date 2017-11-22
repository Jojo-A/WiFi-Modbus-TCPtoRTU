#include ".\h\init.h"
#include <ESP8266WiFi.h>
#include ".\h\mbTCPslave.h"
#include ".\h\trace.h"
#include "FS.h"

const char* ap_default_psk = "password"; ///< Default PSK.
const char* ssid = "YOUR_SSID";
const char* pass = "YOUR_WIFI_PASSWORD";

IPAddress apIP(192, 168, 1, 1);


void WiFiEvent(WiFiEvent_t event);

int statusWifi = 0;
String default_node_name;
String ap_default_ssid;
//String node_name;

/******************************************************************************/
/* Developments     */
void initESP (void)
{
  default_node_name = "esp8266-" + String(ESP.getChipId()); ///< Default name.
  ap_default_ssid  = default_node_name + "-ap"; ///< Default SSID.
  
  Serial.println("Default node name: " + default_node_name);
  
  String station_ssid = "";
  String station_psk = "";
  String station_name = "";
  delay(100);

  SPIFFS.begin();
  if (! loadConfig(&station_ssid, &station_psk, &station_name))
    {
      station_ssid = "";
      station_psk = "";
      station_name = default_node_name;

      Serial.println("No WiFi connection information available.");
    }

    if (WiFi.getMode() != WIFI_STA)
    {
      WiFi.mode(WIFI_STA);
      delay(10);
    }

    // Compare file config with sdk config.
    if (WiFi.SSID() != station_ssid || WiFi.psk() != station_psk)
    {
      // Try to connect to WiFi station.
      WiFi.hostname(station_name.c_str());
      WiFi.begin(station_ssid.c_str(), station_psk.c_str());

      // ... Uncomment this for debugging output.
      //WiFi.printDiag(Serial);
    }
    else
    {
      // Begin with sdk config.
      WiFi.hostname(default_node_name.c_str());
      WiFi.begin();

      //WiFi.printDiag(Serial);
    }

    Serial.println("Wait for WiFi connection.");

    // ... Give ESP 10 seconds to connect to station.
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
    {
      Serial.write('.');
      //Serial.print(WiFi.status());
      delay(500);
    }
    Serial.println();

    // Check connection
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Successfully connected to network: \"" + station_ssid + "\"");
      Serial.println("My IP address:                     " + WiFi.localIP().toString());
      Serial.println("My name:                           " + station_name);
    }
    else
    {
      Serial.println("Can not connect to WiFi station. Go into AP mode.");

      // Go into software AP mode.
      WiFi.mode(WIFI_AP);

      delay(10);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ap_default_ssid.c_str(), ap_default_psk);

      Serial.println("My AP IP address: " + WiFi.softAPIP().toString());
      Serial.print  ("My AP SSID:       "); Serial.println(ap_default_ssid);
      Serial.print  ("My AP password:   "); Serial.println(ap_default_psk);
      Serial.println("My name:          " + station_name);
    }
}
/******************************************************************************/
/* Developments     */
void WiFiEvent(WiFiEvent_t event) {

    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            statusWifi = 1;
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            ESP.wdtDisable();
            delay(1000);
            ESP.restart();
            break;
    }
}
/******************************************************************************/
/* Reading sidewalk     */

bool loadConfig(String *ssid, String *pass, String *name)
{
  // open file for reading.
  File configFile = SPIFFS.open("/cl_conf.txt", "r");
  if (!configFile)
  {
    Serial.println("Failed to open cl_conf.txt.");

    return false;
  }

  // Read content from config file.
  String content = configFile.readString();
  
  configFile.close();
  content.trim();
  
  String new_line = "\r\n";
  uint8_t le = 2;
  
  // Check if there is a second line available.
  int8_t pos = content.indexOf(new_line);
  if (pos == -1)
  {
    le = 1;
    // check for linux and mac line ending.
    new_line = "\n";
    pos = content.indexOf(new_line);
    if (pos == -1)
    {
      new_line = "\r";
      pos = content.indexOf(new_line);
      // If there IS a second line, look if there is a third line:
      // Check if there is a third line available.
      if (pos != -1)
      {
        pos = content.indexOf(new_line, pos);
      }
    }
  }
  // If there is no second or third line: Some information is missing.
  if (pos == -1)
  {
    Serial.println("Invalid content.");
    Serial.println(content);

    return false;
  }

  // Store SSID string
  *ssid = content.substring(0, content.indexOf(new_line));
  pos = content.indexOf(new_line) + le;
  // Store PSK in string
  *pass = content.substring(pos, content.indexOf(new_line, pos));
  pos = content.indexOf(new_line, pos) + le;
  // Store name in string
  *name = content.substring(pos, content.indexOf(new_line, pos));

  ssid->trim();
  pass->trim();
  name->trim();

  return true;
} // loadConfig

/******************************************************************************/
/* Preserving the sidewalk     */
bool saveConfig(String *ssid, String *pass, String *name)
{
  // Open config file for writing.
  File configFile = SPIFFS.open("/cl_conf.txt", "w");
  if (!configFile)
  {
    Serial.println("Failed to open cl_conf.txt for writing");

    return false;
  }

  // Save SSID, PSK and node name
  configFile.println(*ssid);
  configFile.println(*pass);
  configFile.println(*name);

  configFile.close();

  return true;
} // saveConfig
