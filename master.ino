//master
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
}

 uint8_t mac[] = {0x5C, 0xCF, 0x7F, 0x4C, 0x82, 0x5C}; //AP MAC SLAVE'S ADDRESS

  #define WIFI_CHANNEL 4
  int prevstate_1 = LOW;
  int prevstate_2 = LOW;



struct __attribute__((packed))DataStruct {
  char text[32];
};
    DataStruct button_1;
    DataStruct button_2;

void setup() {
  pinMode(14, INPUT_PULLUP); //14
  pinMode(16, INPUT_PULLUP); //16
  pinMode(2, OUTPUT); //2
  pinMode(13, OUTPUT); //13

  Serial.begin(115200); Serial.println();
  Serial.println("Starting EspnowController.ino");
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();
  Serial.printf("This mac: %s, ", WiFi.macAddress().c_str());
  Serial.printf("slave mac: %02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.printf(", channel: %i\n", WIFI_CHANNEL);
  if (esp_now_init() != 0)
  {
    Serial.println("*** ESP initialization failed");
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
  strcpy(button_1.text, "Button 01 pressed");
  strcpy(button_2.text, "Button 02 pressed");
  Serial.println("Setup finished");
}


void loop() {
  sendData();
}


void sendData() {

  int currentstate_1 = digitalRead(16);
  if (prevstate_1 != currentstate_1) {
    if (currentstate_1 == LOW) {
      uint8_t bs[sizeof(button_1)];
      memcpy(bs, &button_1, sizeof(button_1));
      esp_now_send(mac, bs, sizeof(button_1));
      Serial.println(button_1.text);
      digitalWrite(2, !digitalRead(2));
    }
  } prevstate_1 = currentstate_1;

  int currentstate_2 = digitalRead(14);
  if (prevstate_2 != currentstate_2) {
    if (currentstate_2 == LOW) {
      uint8_t bs[sizeof(button_2)];
      memcpy(bs, &button_2, sizeof(button_2));
      esp_now_send(mac, bs, sizeof(button_2));
      Serial.println(button_2.text);
      digitalWrite(13, !digitalRead(13));
    }
  } prevstate_2 = currentstate_2;
}
