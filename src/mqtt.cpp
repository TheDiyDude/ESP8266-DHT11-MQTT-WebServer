#include "mqtt.h"

WiFiClient espClient;
PubSubClient client(espClient);

extern String temp;
extern const char *before;
extern const char *after;

extern char payload[];
//extern const char*  mqtt_topics[];
extern String       client_id;
extern char         mqtt_topic_temperature;
extern char         mqtt_topic_humidity;

float poolTemp = 0.0;
float importPower = 0.0;
float exportPower = 0.0;
float L1Power = 0.0;
float L2Power = 0.0;
float L3Power = 0.0;
float Humidity = 0.0;
float oldHumidity = 0.0;
float Temperature = 0.0;
float oldTemperature = 0.0;

String    temp_values;
char      buffer[300];
char      buildTopic[500];
String    MAC,topic_temperature, topic_humidity;
String    mqtt_topics;
extern MQTT mqtt;
extern configData data;
extern util util;

void MQTT::begin() {
    uint16_t port = atoi(data.mqtt_port);
    client.setServer(data.mqtt_broker, port);
    client.setCallback(callback);
    while (!client.connected())
    {
        MAC = String(WiFi.macAddress());
        MAC.replace(":", "");
        MAC.toLowerCase();
        client_id += "-" + MAC;
        debug("The client ");
        debug(client_id.c_str());
        debug(" connects to mqtt broker: ");
        debug(data.mqtt_broker);
        debug(":");
        debugln(data.mqtt_port);
        if (client.connect(client_id.c_str(), data.mqtt_username, data.mqtt_password))
        {
            debugln("mqtt broker connected");
        }
        else
        {
            debug("failed with state ");
            debugln(client.state());
            delay(2000);
        }
  }
  mqtt_topics = "";
  char *topic = util.split(data.mqtt_subscribe, "\r\n");
  while(topic != NULL)
  {
    Serial.printf("MQTT Subscribing to topic: \"%s\"\n", topic);
    client.subscribe(topic);
    mqtt_topics += topic;
    mqtt_topics += "\r\n";
    topic = util.split(NULL, "\r\n");
  }
  strcpy(data.mqtt_subscribe, mqtt_topics.c_str());
  strcpy(buildTopic, "/");
  strcat(buildTopic,data.hostname);
  strcat(buildTopic,&mqtt_topic_temperature);
  topic_temperature = buildTopic;
  Serial.print("Build Topic: ");
  Serial.println(topic_temperature);

  strcpy(buildTopic,"/");
  strcat(buildTopic,data.hostname);
  strcat(buildTopic,&mqtt_topic_humidity);
  topic_humidity = buildTopic;
  Serial.print("Build Topic: ");
  Serial.println(topic_humidity);
}

void MQTT::publish(const char* topic,char* buffer){
    client.publish(topic,buffer);
}

void MQTT::loop(){
    client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
  //digitalWrite(LED_BUILTIN, LOW);
  String value="";
  Temperature=getTemp();
  if (Temperature != oldTemperature) { 
    oldTemperature = Temperature; 
    sprintf(buffer,"%3.2f",Temperature);
    mqtt.publish(topic_temperature.c_str(),buffer);
  }
  Humidity=getHumidity();
  if (Humidity != oldHumidity) { 
    oldHumidity = Humidity; 
    sprintf(buffer,"%3.2f",Humidity);
    mqtt.publish(topic_humidity.c_str(),buffer);
  }
  for (unsigned int i = 0; i < length; i++) {
      value += String((char)payload[i]);
  }
  sprintf(buffer, "Message arrived in topic: %s\nMessage: %s", (char*)topic, value.c_str());
  debug(buffer);

  if (strcmp(topic, "PoolSteuerung/PoolTemperatur") == 0)
  {
      poolTemp = value.toFloat();
  }
  if ( strcmp(topic,"SmartMeter/EASTRON/SDM72DV2/ImportPower") == 0 ) {
      importPower = value.toFloat();
  }
  if ( strcmp(topic,"SmartMeter/EASTRON/SDM72DV2/ExportPower") == 0 ) {
      exportPower = value.toFloat();
  }
  if ( strcmp(topic,"SmartMeter/EASTRON/SDM72DV2/L1_Power") == 0 ) {
      L1Power = value.toFloat();
  }
  if ( strcmp(topic,"SmartMeter/EASTRON/SDM72DV2/L2_Power") == 0 ) {
      L2Power = value.toFloat();
  }
  if ( strcmp(topic,"SmartMeter/EASTRON/SDM72DV2/L3_Power") == 0 ) {
      L3Power = value.toFloat();
  }
  temp_values = "<td>";
  temp_values += "Pool Temperatur";
  temp_values += "</td><td>";
  temp_values += poolTemp;
  temp_values += "</td><td>&#8451;</td></tr>";
  temp_values += "<tr><td>";
  temp_values += "Temperatur Keller";
  temp_values += "</td><td>";
  temp_values += Temperature;
  temp_values += "</td><td>&#8451;</td>";
  temp_values += "<tr><td>";
  temp_values += "Luftfeuchte Keller";
  temp_values += "</td><td>";
  temp_values += Humidity;
  temp_values += "</td><td>&#37;</td>";
  temp_values += "<tr><td>";
  temp_values += "Import Power";
  temp_values += "</td><td>";
  temp_values += importPower;
  temp_values += "</td><td>Watt</td>";
  temp_values += "<tr><td>";
  temp_values += "Export Power";
  temp_values += "</td><td>";
  temp_values += exportPower;
  temp_values += "</td><td>Watt</td>";
  temp_values += "<tr><td>";
  temp_values += "L1 Power";
  temp_values += "</td><td>";
  temp_values += L1Power;
  temp_values += "</td><td>Watt</td>";
  temp_values += "<tr><td>";
  temp_values += "L2 Power";
  temp_values += "</td><td>";
  temp_values += L2Power;
  temp_values += "</td><td>Watt</td>";
  temp_values += "<tr><td>";
  temp_values += "L3 Power";
  temp_values += "</td><td>";
  temp_values += L3Power;
  temp_values += "</td><td>Watt</td>";
  temp = before + temp_values + after;
  //digitalWrite(LED_BUILTIN, HIGH);
}

void MQTT::checkConnection() {
    if (!client.connected())
    {
      uint16_t port = atoi(data.mqtt_port);
      client.setServer(data.mqtt_broker, port);
      client.setCallback(callback);
      while (!client.connected())
      {
          MAC = String(WiFi.macAddress());
          MAC.replace(":", "");
          MAC.toLowerCase();
          client_id += "-" + MAC;
          debug("The client ");
          debug(client_id.c_str());
          debug(" connects to mqtt broker: ");
          debug(data.mqtt_broker);
          debug(":");
          debugln(data.mqtt_port);
          if (client.connect(client_id.c_str(), data.mqtt_username, data.mqtt_password))
          {
              debugln("mqtt broker connected");
          }
          else
          {
              debug("failed with state ");
              debugln(client.state());
              delay(2000);
          }
        }
        uint8_t timeout = 8;
        while (timeout && (!client.connected())){
            timeout--;
            delay(1000);
        }
        if(client.connected()) {
            mqtt_topics = "";
            MQTT::connected = true;
            char *topic=util.split(data.mqtt_subscribe,"\r\n");
            while(topic != NULL)
            {
                Serial.printf("MQTT Subscribing to topic: \"%s\"\n", topic);
                client.subscribe(topic);
                mqtt_topics += topic;
                mqtt_topics += "\r\n";
                topic = util.split(NULL, "\r\n");
            }
            strcpy(data.mqtt_subscribe, mqtt_topics.c_str());
        } else {
            MQTT::connected = false;
         }
    }
}