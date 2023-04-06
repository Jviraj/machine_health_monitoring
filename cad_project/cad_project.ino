//--------------------------------------------------------------------
//mq2 gas sensor
int LED = 32;            /*LED pin defined*/
int Sensor_input = 33;    /*Digital pin 5 for sensor input*/
//
//--------------------------------------------------------------------
//For Ultrasonic sensor
const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;
//
//--------------------------------------------------------------------
//For temperature sensor
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
//
//--------------------------------------------------------------------
//For Vibration sensor
int vib = 13;
int count = 0;
int prev_count = 0;
//
//--------------------------------------------------------------------
// Webserver
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Arduino.h>
const char* ssid = "SSID";
const char* password = "PASSWORD";
JSONVar reading;
AsyncWebServer server(80);
AsyncEventSource events("/events");
//--------------------------------------------------------------------
// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
//    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
//    return readDHTHumidity();
  }
  return String();
}

//--------------------------------------------------------------------
void setup()
{
    Serial.begin(115200);
    delay(1000);

    //Initialize SPIFFS
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }

    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
  
    // Print ESP32 Local IP Address
    Serial.println(WiFi.localIP());
  
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/index.html", "text/html");
    });

      server.serveStatic("/", SPIFFS, "/");

//    server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request){
//      request->send(SPIFFS, "/index.html", String(), false, processor);
//    });

  // Handle Web Server Events
    events.onConnect([](AsyncEventSourceClient *client){
      if(client->lastId()){
        Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
      }
      // send event with message "hello!", id current millis
      // and set reconnect delay to 1 second
      client->send("hello!", NULL, millis(), 10000);
    });
    server.addHandler(&events);

    // Start server
    server.begin();
    
    //MQ2
    pinMode(LED, OUTPUT);  /*LED set as Output*/

    //Ultrasonic
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

    //Temp
    // Start the DS18B20 sensor
    sensors.begin();

    //Vibration
    pinMode(vib, INPUT);

    xTaskCreate(mq2, "Interfacing MQ2", 10000, NULL, 3, NULL);
    delay(2500);
    xTaskCreate(ultraSonic, "Interfacing HC SR04", 10000, NULL, 2, NULL);
    delay(2500);
    xTaskCreate(temp, "Interfacing Temperature Sensor", 10000, NULL, 1, NULL);
    delay(2500);
    xTaskCreate(vibration, "Interfacing Vibration Sensor", 10000, NULL, tskIDLE_PRIORITY, NULL);
}
//--------------------------------------------------------------------
void loop()
{delay(1000);}
//--------------------------------------------------------------------
void mq2(void *pvParameters)
{
    while(1){
    //Serial.println("Inside MQ2");
    int sensor_Aout = analogRead(Sensor_input);  /*Analog value read function*/
    if(sensor_Aout<500) break;
    Serial.print("Gas Sensor: ");   
    Serial.println(sensor_Aout);   /*Read value printed*/
//    Serial.print("\t");
//    Serial.print("\t");
    if (sensor_Aout > 1800) {    /*if condition with threshold 1800*/
//      Serial.println("Gas");  
      digitalWrite (LED, HIGH) ; /*LED set HIGH if Gas detected */
      reading["mq2"] = "1";
    }
    else {
//      Serial.println("No Gas");
      digitalWrite (LED, LOW) ;  /*LED set LOW if NO Gas detected */
      reading["mq2"] = "0";
    }
    String accString = JSON.stringify (reading);
    events.send(accString.c_str(),"gas_reading",NULL);
    delay(10000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void ultraSonic(void *pvParameters)
{
    while(1){
    //Serial.println("Inside Ultrasonic");
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    
    // Calculate the distance
    distanceCm = duration * SOUND_SPEED/2;
    
    if(distanceCm==0) break;
    
    // Convert to inches
    distanceInch = distanceCm * CM_TO_INCH;
    
    // Prints the distance in the Serial Monitor
    Serial.print("Distance (cm): ");
    Serial.println(distanceCm);
    Serial.print("Distance (inch): ");
    Serial.println(distanceInch);
    reading["distance"] = String(distanceCm);
    String accString = JSON.stringify (reading);
    events.send(accString.c_str(),"ultrasonic_reading",NULL);
    delay(10000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void temp(void *pvParameters)
{
    while(1){
    //Serial.println("Inside Temperature");
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    if(temperatureC<0) break;
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    reading["temp"] = String(temperatureC);
    String accString = JSON.stringify (reading);
    events.send(accString.c_str(),"temperature_reading",NULL);
    delay(10000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void vibration(void *pvParameters)
{
    while(1){
    //Serial.println("Inside Vibration");
    bool value = digitalRead(vib);
    if (value == 1) {
      count = count + 1;
      delay(1000);
      reading["vibration"] = String(prev_count);
      String accString = JSON.stringify (reading);
      events.send(accString.c_str(),"chatter_reading",NULL);
    }else if(value == 0){
      if(count != prev_count){
        prev_count = count;
        Serial.println(prev_count);
     }
    }
    }
    vTaskDelete( NULL );
}
