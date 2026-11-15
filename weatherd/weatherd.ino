#include <WiFi.h>
#include "ESP32HTTPClient.h"
#include "DHT.h"
#include <ArduinoJson.h>

// Library setup
// ESP32HTTPClient client("https://c550-105-113-72-222.ngrok-free.app");
ESP32HTTPClient client("https://esp32-weatherapp-api.onrender.com");

// Define GPIO pins
const int DHTPIN = 4;
const int LED = 26;

// Sensor type
#define DHTTYPE DHT11   // DHT 11
// #define DHTTYPE DHT22 // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

// Wi-Fi & API credentials
const char* API_KEY = "change-this-to-something-long";
const char* WIFI_SSID = "OSIARU";
const char* WIFI_PASS = "11111110";

int counter = 0;

void beep(int times = 1, int durationMs = 100) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED, HIGH);
    delay(durationMs);
    digitalWrite(LED, LOW);
    if (i < times - 1) delay(80);
  }
}

// Generate simulated sensor values
float genReadingValue(String kind = "h") {
  if (kind == "h") {
    return random(1, 1000) / 10.0;
  } else if (kind == "t") {
    return random(150, 809) / 10.0;
  }
  return 0.0;
}

// Library setup (use your live Render URL)

void postReadings(float hum, float temp) {
  String message = "";

  // Chain .body() calls — the library builds {"humidity": ..., "temperature": ...} automatically
  client.post("/readings")
    .body("humidity", hum)
    .body("temperature", temp)
    .onError([](int code, const char* msg) {
      Serial.printf("FETCH Error %d: %s\n", code, msg);
    })
    .getBody("message", &message);

  if (client.isSuccess() && message != "") {
    Serial.println(message);
    beep(3);
  } else {
    Serial.println("Failed to POST Readings.");
  }
}


void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  client.setInsecure();  // Bypass SSL certificate check for ngrok

  Serial.println("Starting DHT sensor...");
  dht.begin();
}

void loop() {
  delay(2000);

  // Demo data (uncomment dht lines when hardware is ready)
  // float humidity = dht.readHumidity();
  // float tempC = dht.readTemperature();
  float humidity = genReadingValue("h");
  float tempC = genReadingValue("t");

  if (isnan(humidity) || isnan(tempC)) {
    Serial.println("Failed to read from DHT sensor! Check wiring.");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  |  Temperature: ");
  Serial.print(tempC);
  Serial.println("°C");


  // Increment counter and post every 20 readings (~40 seconds)
  counter++;
  if (counter >= 5) {
    counter = 0;
    postReadings(humidity, tempC);
  }
}
