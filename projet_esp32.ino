#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#define CONNECTION_TIMEOUT 10
#define LED 18

AsyncWebServer server(80);
const char* ssid = "WiFiESP32";
const char* password = "motdepasse";

void setup(){
    Serial.begin(115200);
    delay(1000);
    // Set pin mode
    pinMode(LED, OUTPUT);
    digitalWrite(LED, LOW);
    
    // Set WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    int timeout_counter = 0;
    
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(250);
        
        //Reset ESP32
        timeout_counter++;
        if(timeout_counter >= CONNECTION_TIMEOUT*5){
        ESP.restart();
        }
    }
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    //serveur web
    server.on("/LedOn",HTTP_GET, [](AsyncWebServerRequest * request) {
      Serial.println("Led On : ");
      Serial.println (request-> client () -> remoteIP ());
      digitalWrite(LED, HIGH);
      request->send(200, "text/plain", "Allumée\n");
    });
    server.on("/LedOff",HTTP_GET, [](AsyncWebServerRequest * request) {
      Serial.println("Led Off : ");
      Serial.println (request-> client () -> remoteIP ());
      digitalWrite(LED, LOW);
      request->send(200, "text/plain", "Eteinte\n");
    });
    server.begin();
    Serial.println("Serveur actif");
}

void loop() {}
