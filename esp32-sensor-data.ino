#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define AOUT_PIN 36  // ESP32 pin GPIO36 (ADC0) that connects to AOUT pin of moisture sensor
const char* ssid = "";
const char* password = "";
const char* host = "babimjvuxtxesunjiagw.supabase.co";
const char* resource = "/rest/v1/sensor";
WiFiClientSecure client;

void setup() {
  Serial.begin(115200);  // Initialize serial for debugging
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("?");
  }
  Serial.println("\nConnected to WiFi");
  client.setInsecure();  // Not recommended for production, set up proper CA certificate validation
}

void sendMoistureData(int sensorValue) {
  HTTPClient http;
  String url = String("https://") + String(host) + String(resource);
  http.begin(client, url);  // Start HTTPS connection
  http.addHeader("Content-Type", "application/json");
  http.addHeader("apikey", "<key here>");  // Replace with your Supabase API key

  // JSON object for the payload
  String httpRequestData = "{\"sensorValue\":" + String(sensorValue) + "}";

  // Send POST request and get response code
  int httpResponseCode = http.POST(httpRequestData);

  // Check and print HTTP response code
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0) {
    if (httpResponseCode >= 200 && httpResponseCode < 300) {
      Serial.println("Request successful.");
    } else {
      Serial.println("Server responded with error.");
    }
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(http.errorToString(httpResponseCode));  // Print the error message
  }

  http.end();  // Close HTTP connection
}


void loop() {

  int sensorValue = analogRead(AOUT_PIN);  // read the analog value from sensor

  Serial.print("Moisture valuee: ");
  Serial.println(sensorValue);

  sendMoistureData(sensorValue);
  delay(500);
}
