
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "Arduino.h"



int led1 = 2;                                                             // led pin yellow to indicate thet the fan is turned off 
int led2 = 15;                                                            // led pin blue to indicate thet the fan is turned off 


int bright = 0;                                                           // to control the speed of the fan
int fade = 5;

const int motor = 19;                                                     // motor pin

                                                                          // WiFi Parameters
const char* ssid = "CenturyLink3541";
const char* password = "August17";

void setup() {
  
  pinMode( motor , OUTPUT);
 
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);                                    
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {                                  // checks the wifi connection status
    delay(500);
    Serial.print(".");
  }
}

void loop() {
  ledcwrite( 0, bright), ;
                                                                          // Check WiFi Status
  int speed = Serial.parseInt();
  
  if (WiFi.status() == WL_CONNECTED) {
    
    HTTPClient http;                                                      //Object of class HTTPClient
    
    http.begin("http://api.openweathermap.org/data/2.5/weather?q=Arvada&units=imperial&APPID=2766afd92bbcd1446ef143934c241f39");
    
    int httpCode = http.GET();
                                                                          //Check the returning code
    if (httpCode > 0) {
                                                                          
      String payload = http.getString();                                  // Get the request response payload
      Serial.println(payload);

                                                                         // Parsing
      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 280;
      DynamicJsonDocument doc(capacity);

     

      JsonObject main = doc["main"];
      float main_temp = main["temp"]; 

      JsonObject sys = doc["sys"];


      Serial.print("current temp is ");
                                                                          // prints out the temperature
      Serial.println(main_temp);

      
      if( main_temp < 60 ){
         digitalWrite(led1,HIGH);                                         // if the temparature is less then 60 yeelow light is on indicating that the fan is turned off
      }

      if(main_temp < 80 && main_temp > 60){                               // if the temparture is more than 60 and less tahn 80 turns the fan on at medium speed
                                         
         bright = bright + fade;                                          // controlling the total speed 

         if (bright <= 0 || bright >= 255) {
            fade = -fade; 
        analogWrite( motor, bright); 
            
        digitalWrite(led2, HIGH); }
        delay(30);                                                      
      }
      
      if(main_temp > 80){                                               // if the temparture is greater than 80 turns the fan on at max speed
         digitalWrite(led2, HIGH); 
         digitalWrite(motor ,HIGH);
      }
        
    }

    http.end();   //Close connection
  }
  // Delay
  delay(60000);
}
