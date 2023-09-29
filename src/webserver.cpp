#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "theDiyDude.h"
#include "webserver.h"

extern configData data;
extern void blink_led(int pin, unsigned long time);
extern void updateMillisStart();
extern void scanNetworks();
extern util util;

ESP8266WebServer server(80);

const char* PARAM_MESSAGE = "message";
String temp,content,wifiNetworksListObjects;


void launchWeb(int webtype) {
  createWebServer(webtype);
  server.begin();
}

void createWebServer(int webtype) {
  if (webtype == 0) {
    server.on("/", []() {
      scanNetworks();
      String ssid = data.ssid;
      String password = data.password;
      String mqtt_broker = data.mqtt_broker;
      String mqtt_username = data.mqtt_username;
      String mqtt_password = data.mqtt_password;
      String mqtt_port = data.mqtt_port;
      String hostname = data.hostname;
      String mqtt_subscribe = data.mqtt_subscribe;
      content = "<html><head><style>.button {background-color: #3CBC8D;";
      content += "color: white;padding: 5px 10px;text-align: center;text-decoration: none;";
      content += "display: inline-block;font-size: 14px;margin: 4px 2px;cursor: pointer;}";
      content += "input[type=text],[type=password]{width: 100%;padding: 5px 10px;";
      content += "margin: 5px 0;box-sizing: border-box;border: none;";
      content += "background-color: #3CBC8D;color: white;}";
      content += ".landingPage {margin: 0 auto 0 auto;width: 100%;min-height: 100vh;display: flex;align-items: center;justify-content: center;}";
      content += "</style></head><body><div class='landingPage'><div>";
      content += "<table><tr>";
      content += "<td><form action='/'><input type='submit' value='Home' /></form></td>";
      content += "<td><form action='/settings'><input type='submit' value='Settings' /></form></td>";
      content += "<td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>";
      content += "</tr></table>";      
      content += "<h1>Setup</h1>";
      content += "<h3>Current Settings</h3>";
      content += "<table><tr><td><label> WiFi SSID </label></td><td><p style='color:blue;'>" + ssid + "</p></td></tr>";
      content += "<tr><td><label> WiFi Pasword </label></td><td><p style='color:blue;'>*****</p></td></tr></table>";
      content += "<h4>Available Networks:</h4>"+wifiNetworksListObjects;
      content += "<form method='get' action='setting'>";
      content += "<h3>New Settings</h3>";
      content += "<table><tr><td><label>WiFi SSID</label></td><td><input type='text' name = 'ssid' lenght=32 value='"+ssid+"'></td></tr>";
      content += "<tr><td><label> WiFi Password</label></td><td><input type='password' name = 'password' lenght=32 value='"+password+"'></td></tr>";
      content += "<tr><td><label>Enable MQTT</label></td><td><input type='checkbox' name='mqtt' checked='on'></td></tr>";
      content += "<tr><td><label> My Hostname</label></td><td><input type='text' name = 'hostname' lenght=32 value='"+hostname+"'></td></tr>";
      content += "<tr><td><label> MQTT Broker</label></td><td><input type='text' name = 'mqtt_broker' lenght=32 value='"+mqtt_broker+"'></td></tr>";
      content += "<tr><td><label> MQTT Port</label></td><td><input type='text' name = 'mqtt_port' lenght=32 value='"+mqtt_port+"'></td></tr>";
      content += "<tr><td><label> MQTT Username</label></td><td><input type='text' name = 'mqtt_username' lenght=32 value='"+mqtt_username+"'></td></tr>";
      content += "<tr><td><label> MQTT Password</label></td><td><input type='password' name = 'mqtt_password' lenght=32 value='"+mqtt_password+"'></td></tr>";
      content += "<tr><td><label> MQTT Subscribe</label></td><td><textarea name='mqtt_subscribe' rows=5 cols=40>"+mqtt_subscribe+"</textarea></td></tr>";
      
      content += "<tr><td></td><td><input class=button type='submit'></td></tr></table></form>";
      content += "<table><tr>";
      content += "<td><form action='/'><input type='submit' value='Home' /></form></td>";
      content += "<td><form action='/settings'><input type='submit' value='Settings' /></form></td>";
      content += "<td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>";
      content += "</tr></table>";
      content += "</div></div></body></html>";
      server.send(200, "text/html", content);
    }); 
    server.on("/setting", []() { 
      strcpy(data.ssid           ,server.arg("ssid").c_str());
      strcpy(data.password       ,server.arg("password").c_str());  
      strcpy(data.hostname       ,server.arg("hostname").c_str()); 
      strcpy(data.mqtt_broker    ,server.arg("mqtt_broker").c_str()); 
      strcpy(data.mqtt_username  ,server.arg("mqtt_username").c_str()); 
      strcpy(data.mqtt_password  ,server.arg("mqtt_password").c_str()); 
      strcpy(data.mqtt_port      ,server.arg("mqtt_port").c_str()); 
      strcpy(data.mqtt_subscribe ,server.arg("mqtt_subscribe").c_str());
      print_configuration();
      util.setEepAdr(sizeof(data));
      util.writeConfigurationToEEPROM();
      content = "Success...rebooting NOW!";
      Serial.printf("Writing bytes: %d\n", util.getEepAdr());
      server.send(200, "text/html", content);
      delay(2000);
      ESP.restart();
    });
    server.on("/clear", []() {                      
      util.clearEEPROM();
      delay(2000);
      ESP.restart();
    });   
  }
  if (webtype == 1) {
    server.on("/", []() {
        content = temp;
        server.send(200, "text/html", content);
    });
    server.on("/clear", []() {                      
      util.clearEEPROM();
      delay(2000);
      ESP.restart();
    });
    server.on("/tester", []() {                  
      String ssidw = server.arg("ssid");
      String passw = server.arg("password");
      String mqtt = server.arg("mqtt");
      String mqtt_broker = server.arg("mqtt_broker");
      String mqtt_port = server.arg("mqtt_port");
      String mqtt_username = server.arg("mqtt_username");
      String mqtt_password = server.arg("mqtt_password");
      String sub = server.arg("mqtt_subscribe");
      debug("SSID: "); debugln(ssidw);
      debug("Password: "); debugln(passw);
      debug("MQTT: "); debugln(mqtt);
      debug("MQTT Broker: "); debugln(mqtt_broker);
      debug("MQTT Port: "); debugln(mqtt_port);
      debug("MQTT Username: "); debugln(mqtt_username);
      debug("MQTT Password: "); debugln(mqtt_password);
      debug("MQTT Sub: "); debugln(sub);
      content = "Success...rebooting NOW!";
      server.send(200, "text/html", content);
    }); 
    server.on("/settings", []() {
      scanNetworks();
      String ssid = data.ssid;
      String password = data.password;
      String mqtt_broker = data.mqtt_broker;
      String mqtt_username = data.mqtt_username;
      String mqtt_password = data.mqtt_password;
      String mqtt_port = data.mqtt_port;
      String hostname = data.hostname;
      String mqtt_subscribe = data.mqtt_subscribe;
      content = "<html><head><style>.button {background-color: #3CBC8D;";
      content += "color: white;padding: 5px 10px;text-align: center;text-decoration: none;";
      content += "display: inline-block;font-size: 14px;margin: 4px 2px;cursor: pointer;}";
      content += "input[type=text],[type=password]{width: 100%;padding: 5px 10px;";
      content += "margin: 5px 0;box-sizing: border-box;border: none;";
      content += "background-color: #3CBC8D;color: white;}";
      content += ".landingPage {margin: 0 auto 0 auto;width: 100%;min-height: 100vh;display: flex;align-items: center;justify-content: center;}";
      content += "</style></head><body><div class='landingPage'><div>";
      content += "<table><tr>";
      content += "<td><form action='/'><input type='submit' value='Home' /></form></td>";
      content += "<td><form action='/settings'><input type='submit' value='Settings' /></form></td>";
      content += "<td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>";
      content += "</tr></table>";      
      content += "<h1>Setup</h1>";
      content += "<h3>Current Settings</h3>";
      content += "<table><tr><td><label> WiFi SSID </label></td><td><p style='color:blue;'>" + ssid + "</p></td></tr>";
      content += "<tr><td><label> WiFi Pasword </label></td><td><p style='color:blue;'>*****</p></td></tr></table>";
      content += "<h4>Available Networks:</h4>"+wifiNetworksListObjects;
      content += "<form method='get' action='setting'>";
      content += "<h3>New Settings</h3>";
      content += "<table><tr><td><label>WiFi SSID</label></td><td><input type='text' name = 'ssid' lenght=32 value='"+ssid+"'></td></tr>";
      content += "<tr><td><label> WiFi Password</label></td><td><input type='password' name = 'password' lenght=32 value='"+password+"'></td></tr>";
      content += "<tr><td><label>Enable MQTT</label></td><td><input type='checkbox' name='mqtt' checked='on'></td></tr>";
      content += "<tr><td><label> My Hostname</label></td><td><input type='text' name = 'hostname' lenght=32 value='"+hostname+"'></td></tr>";
      content += "<tr><td><label> MQTT Broker</label></td><td><input type='text' name = 'mqtt_broker' lenght=32 value='"+mqtt_broker+"'></td></tr>";
      content += "<tr><td><label> MQTT Port</label></td><td><input type='text' name = 'mqtt_port' lenght=32 value='"+mqtt_port+"'></td></tr>";
      content += "<tr><td><label> MQTT Username</label></td><td><input type='text' name = 'mqtt_username' lenght=32 value='"+mqtt_username+"'></td></tr>";
      content += "<tr><td><label> MQTT Password</label></td><td><input type='password' name = 'mqtt_password' lenght=32 value='"+mqtt_password+"'></td></tr>";
      content += "<tr><td><label> MQTT Subscribe</label></td><td><textarea name='mqtt_subscribe' rows=5 cols=40>"+mqtt_subscribe+"</textarea></td></tr>";
      
      content += "<tr><td></td><td><input class=button type='submit'></td></tr></table></form>";
      content += "<table><tr>";
      content += "<td><form action='/'><input type='submit' value='Home' /></form></td>";
      content += "<td><form action='/settings'><input type='submit' value='Settings' /></form></td>";
      content += "<td><form action='/clear'><input type='submit' value='Clear EEPROM' /></form></td>";
      content += "</tr></table>";
      content += "</div></div></body></html>";
      server.send(200, "text/html", content);
    });
    server.on("/setting", []() {   
      strcpy(data.ssid           ,server.arg("ssid").c_str());
      strcpy(data.password       ,server.arg("password").c_str());  
      strcpy(data.hostname       ,server.arg("hostname").c_str()); 
      strcpy(data.mqtt_broker    ,server.arg("mqtt_broker").c_str()); 
      strcpy(data.mqtt_username  ,server.arg("mqtt_username").c_str()); 
      strcpy(data.mqtt_password  ,server.arg("mqtt_password").c_str()); 
      strcpy(data.mqtt_port      ,server.arg("mqtt_port").c_str()); 
      strcpy(data.mqtt_subscribe ,server.arg("mqtt_subscribe").c_str());
      print_configuration();
      util.setEepAdr(sizeof(data));
      util.writeConfigurationToEEPROM();
      content = "Success...rebooting NOW!";
      Serial.printf("Writing bytes: %d\n", util.getEepAdr());
      server.send(200, "text/html", content);
      delay(2000);
      ESP.restart();
    });
  }
}