#include "wifi.h"
#include "theDiyDude.h"
#include "webserver.h"

extern void blink_led(int pin, unsigned long time);
extern void updateMillisStart();
extern util util;
extern const char *myHostname;
extern String wifiNetworksListObjects;
bool apmode;
int numNetworksFound=0;
extern configData data;

void wifi_setup()
{
    util.readConfigurationFromEEPROM();
    if (strlen(data.ssid) == 0)
    { 
      ap_mode();
    }
    else
    {
      if (testWifi())
      { 
        debugln("WiFi Connected!!!");
        apmode = false;
        debugln("Ready");
        debug("IP address: ");
        debugln(WiFi.localIP());
        debug("data.hostname = ");
        debugln(data.hostname);
        if (MDNS.begin(data.hostname))
        {
          debugln("mDNS gestartet.");
        }
        launchWeb(1);
      }
      else
      {
        ap_mode();
      }
    }
}
void ap_mode() {                                  
  debugln("AP Mode. Please connect to http://192.168.4.1 to configure");
  const char* ssidap = "Setup";
  const char* passap = "";
  scanNetworks();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssidap, passap);
  debugln(WiFi.softAPIP());
  apmode = true;
  launchWeb(0);                                   
}
boolean testWifi() {                            
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  scanNetworks();
  WiFi.mode(WIFI_STA);
  WiFi.begin(data.ssid, data.password);
  int c = 0;
  while (c < 30) {
    if (WiFi.status() == WL_CONNECTED) {
      WiFi.setAutoReconnect(true);
      WiFi.persistent(true);
      debugln(WiFi.status());
      debugln(WiFi.localIP());
      delay(100);
      return true;
    }
    debug(".");
    delay(900);
    c++;
  }
  debugln("Connection time out...");
  return false;
}
void scanNetworks(){
    numNetworksFound = WiFi.scanNetworks(false, true);
    if (numNetworksFound == 0) {
        debugln("No Wifi Networks found!");
    } else {
        wifiNetworksListObjects = "<ol>";
        for (int i = 0; i < numNetworksFound; ++i)
        {
            wifiNetworksListObjects += "<li>";
            wifiNetworksListObjects += WiFi.SSID(i);
            wifiNetworksListObjects += " (";
            wifiNetworksListObjects += WiFi.RSSI(i);
            wifiNetworksListObjects += ")";
            wifiNetworksListObjects += (WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*";
            wifiNetworksListObjects += "</li>";
        }
        wifiNetworksListObjects += "</ol>";
    }
}
