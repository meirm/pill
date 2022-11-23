#include "config.h"
#include "utils.h"
#include "DHTesp.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <datecalc.h>


RTC_DATA_ATTR int bootCount = 0;

RTC_DATA_ATTR unsigned long cycles = 0;
RTC_DATA_ATTR unsigned long uptime = 0;

int soilMoisture;


DHTesp dht;



void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  pinMode(SOILPIN, INPUT);
  pinMode(DHTPIN, INPUT);

  ++bootCount;
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  while_awake();

}

void while_awake() {
  int trialcount = 0;
  cycles++;
  dht.setup(DHTPIN, DHTesp::DHT11);
  Serial.begin(115200);
  Serial.println("Boot number: " + String(bootCount));
  if (bootCount % FREQ == 0) {
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (trialcount++  > WIFI_TIMEOUT) {
        uptime +=millis() + TIME_TO_SLEEP * 1000;
        esp_deep_sleep_start();
      }
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Spil moisture reading
    soilMoisture = analogRead(SOILPIN);
    //    Serial.print(soilMoisture);
    //    soilMoisture = map(soilMoisture, 4095, 0, 0, 100);
    Serial.print("Moisture: ");
    Serial.println(soilMoisture);


    // DHT readings
    TempAndHumidity newValues = dht.getTempAndHumidity();
    float temp = newValues.temperature;
    float humi = newValues.humidity;
    long rssi = WiFi.RSSI();
    Serial.print("cycles: ");
    Serial.println(cycles);
    char summary[128];
    DateCalc datecalc(0);
    Serial.println("T: " + String(temp) + "  H: " + String(humi));
    String urlFinal = "https://script.google.com/macros/s/" + GOOGLE_SCRIPT_ID + "/exec";
    String uriTemp = "?temp=" + String(temp) + "&humi=" + String(humi) + "&pill_id=" + PILLID + "&moisture=" + String(soilMoisture) + "&rssi=" + String(rssi) + "&cycles=" + String(cycles) + "&uptime=" + urlencode(datecalc.timeDiff(uptime + millis()).time.c_str());
    urlFinal += uriTemp;
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
  } else {
    delay(1000);
  }
  digitalWrite(LED_PIN, LOW);
  uptime +=millis() + TIME_TO_SLEEP * 1000;
  esp_deep_sleep_start();
}

void loop() {

}
