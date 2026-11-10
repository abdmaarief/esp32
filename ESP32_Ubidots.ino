
#include <WiFi.h>
#include "UbidotsEsp32Mqtt.h"
const char *TOKEN = "ISI_TOKEN_UBIDOTS";
const char *WIFI_SSID = "ISI_NAMA_WIFI";
const char *WIFI_PASS = "ISI_PASSWORD_WIFI";
const char *DEVICE_LABEL = "esp32-led";
const char *VARIABLE_LABEL = "led";
const int PIN_LED = 23;
Ubidots ubidots(TOKEN);

void callback(char *topic, byte *payload, unsigned int length) {
  String pesan = "";
  for (unsigned int i = 0; i < length; i++) {
    pesan += (char)payload[i];
  }
  Serial.print("Nilai diterima: ");
  Serial.println(pesan);
  int nilai = pesan.toInt();
  if (nilai == 1) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("LED MENYALA");
  } else if (nilai == 0 && (pesan == "0" || pesan == "0.0")) {
    digitalWrite(PIN_LED, LOW);
    Serial.println("LED MATI");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) { 
    ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  }
  if (!ubidots.connected()) {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL);
  }
  ubidots.loop();
}
