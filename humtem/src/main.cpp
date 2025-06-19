#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN D4      // GPIO2
#define DHTTYPE DHT11  // Or DHT22

const char* ssid = "SREEHARI";
const char* password = "447643899";

const char* host = "api.thingspeak.com";
const char* writeAPIKey = "PLCAZ0TUXMD6RWE3";

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT");
    return;
  }

  if (client.connect(host, 80)) {
    String url = "/update?api_key=";
    url += writeAPIKey;
    url += "&field1=" + String(temp);
    url += "&field2=" + String(hum);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("Data sent to ThingSpeak!");
  }

  delay(15000);  // ThingSpeak allows 1 update every 15 seconds
}
