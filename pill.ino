#include "config.h"

#include "DHTesp.h"
#include <WiFi.h>
#include <HTTPClient.h>


RTC_DATA_ATTR int bootCount = 0;

int soilMoisture;

WiFiServer server(80);


DHTesp dht;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  pinMode(SOILPIN, INPUT);
  pinMode(DHTPIN, INPUT);
  dht.setup(DHTPIN, DHTesp::DHT11);

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  
  ++bootCount;
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    Serial.begin(115200);
    Serial.println("Boot number: " + String(bootCount));
  if (bootCount % 4 == 0) {
    // Spil moisture reading
    soilMoisture = analogRead(SOILPIN);
    Serial.print(soilMoisture);
    soilMoisture = map(soilMoisture, 4095, 0, 0, 100);
    Serial.print("Moisture: ");
    Serial.println(soilMoisture);


    // DHT readings
    TempAndHumidity newValues = dht.getTempAndHumidity();
    float temp = newValues.temperature;
    float humi = newValues.humidity;
    Serial.println("T: " + String(temp) + "  H: " + String(humi));
    String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec?sheet=rfidlog" + "&count=" + temp + "&tagid=" + humi;
    Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    Serial.print("HTTP Status Code: ");
    Serial.println(httpCode);
    String payload;
    if (httpCode > 0) {
      payload = http.getString();
      Serial.println("Payload: " + payload);
    }
    http.end();
    urlFinal = "";

    
    Serial.println("Going to sleep now");
    Serial.flush();
  }else{
    delay(1000);
  }
  digitalWrite(LED_PIN, LOW);
  esp_deep_sleep_start();
}

void loop() {

}
