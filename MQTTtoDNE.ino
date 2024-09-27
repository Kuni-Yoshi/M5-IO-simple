/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack Equipped with M5Core sample source code
* Doog Navigation Engine MQTT interface from M5 Stack.
* Date: 2024-09-24
*******************************************************************************
*/
#include <PubSubClient.h>
#include <WiFi.h>

#include "M5Stack.h"
#include <driver/adc.h>

//#define DNE_SERIAL RMS-10C2-AAH46

WiFiClient espClient;
PubSubClient client(espClient);

// Configure the name and password of the connected wifi and your MQTT Serve
const char* ssid = "RMS-10C2-AAH46-53485427";
const char* password = "53485427";

const char* mqtt_server = "192.168.212.1";
const int16_t mqttPort = 1883;
const char* mqtt_user = "mqtt";
const char* mqtt_pass = "sI7G@DijuY";

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (64)
char msg[MSG_BUFFER_SIZE];
int value = 0;  //回数

uint16_t colorvalue = 0;

void setupWifi();
void callback(char* topic, byte* payload, unsigned int length);
void reConnect();

void setup() {
  M5.begin(true, false, false, false);
  M5.Power.begin();
  M5.Lcd.setTextSize(2);
  adc_power_acquire();

  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);

  dacWrite(25, 0);  // Speaker OFF

  setupWifi();
  client.setServer(mqtt_server, mqttPort);  // Sets the server details.  配置所连接的服务器
  client.setCallback(callback);             // Sets the message callback function.  设置消息回调函数
}

void loop() {
  M5.update();
  if (!client.connected()) {
    reConnect();
  }
  client.loop();  // This function is called periodically to allow clients to
                  // process incoming messages and maintain connections to the
                  // server.
  if (M5.BtnA.wasPressed()) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Speaker.beep();  //beep
  } else if (M5.BtnA.wasReleased()) {
    //
    M5.Lcd.println("BtnAwasPressed Publish message:");
    snprintf(msg, MSG_BUFFER_SIZE, "{\"app\":\"memory-start-save\",\"params\":\"--map IO76\"}");
    M5.Lcd.println(msg);
    client.publish("0/THOUZER_HW/RMS-10C2-AAH46/exec/cmd", msg);  // Publishes a message to the specified
  }

  if (M5.BtnB.wasPressed()) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Speaker.beep();  //beep
    digitalWrite(5, HIGH);
    digitalWrite(2, HIGH);
  } else if (M5.BtnB.wasReleased()) {
    //M5.Lcd.clear(BLACK);
    M5.Lcd.println("BtnBwasPressed Publish message:");
    snprintf(msg, MSG_BUFFER_SIZE, "{\"app\":\"app-memorytrace\",\"params\":\"--map IO76\"}");
    M5.Lcd.println(msg);

    client.publish("0/THOUZER_HW/RMS-10C2-AAH46/exec/cmd", msg);  // Publishes a message to the specified
  }

  if (M5.BtnC.wasPressed()) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Speaker.beep();  //beep
  } else if (M5.BtnC.wasReleased()) {
    //M5.Lcd.clear(BLACK);
    M5.Lcd.println("BtnCwasPressed Publish message:");
    snprintf(msg, MSG_BUFFER_SIZE, "{\"app\":\"app-memorytrace\",\"params\":\"--map IO76 --loop\"}");
    M5.Lcd.println(msg);
    client.publish("0/THOUZER_HW/RMS-10C2-AAH46/exec/cmd", msg);  // Publishes a message to the specified
  }
  delay(2);
}

void setupWifi() {
  delay(10);
  M5.Lcd.printf("Connecting to %s", ssid);
  WiFi.mode(WIFI_STA);         // Set the mode to WiFi station mode.  设置模式为WIFI站模式
  WiFi.begin(ssid, password);  // Start Wifi connection.  开始wifi连接

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.printf("\nSuccess\n");
}

void callback(char* topic, byte* payload, unsigned int length) {
  char submsg[MSG_BUFFER_SIZE];
  //snprintf(submsg, MSG_BUFFER_SIZE, "%d",length);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Message arrived []");
  M5.Lcd.println();
  //M5.Lcd.println(submsg);


  if (payload[length - 7] == '1') {

    M5.Lcd.print("IO 3 =");  //Green
    M5.Lcd.print((char)payload[length - 7]);
    M5.Lcd.println();
    digitalWrite(5, LOW);
  } else {
    digitalWrite(5, HIGH);
  };


  if (payload[length - 9] == '1') {
    M5.Lcd.print("IO 5 =");  //Red
    M5.Lcd.print((char)payload[length - 9]);
    M5.Lcd.println();

    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  };

  M5.Lcd.println();

  for (int i = 0; i < length; i++) { M5.Lcd.print((char)payload[i]); }

  M5.Lcd.println();
  //colorvalue= M5.Lcd.color565(0,255,0);
}

void reConnect() {
  while (!client.connected()) {
    M5.Lcd.clear();
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Attempting MQTT connection...");
    // Create a random client ID.  创建一个随机的客户端ID
    String clientId = "M5Stack-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect.  尝试重新连接
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass)) {
      M5.Lcd.printf("\nSuccess\n");
      // Once connected, publish an announcement to the topic.
      client.publish("0/THOUZER_HW/RMS-10C2-AAH46/exec/cmd", "hello world from M5Stack");
      // ... and resubscribe.
      client.subscribe("0/WHISPERER/RMS-10C2-AAH46/io12");
    } else {
      M5.Lcd.print("failed, rc=");
      M5.Lcd.print(client.state());
      M5.Lcd.println("try again in 5 seconds");
      delay(5000);
      M5.Lcd.println("try again...");
      delay(1000);
    }
  }
}
