#include <Arduino_JSON.h>
JSONVar reading;
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
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";
String str = "";

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // Send a GET request to localhost
  sendPostRequest("greeting", "a");

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
   delay(500);
   xTaskCreate(ultraSonic, "Interfacing HC SR04", 10000, NULL, 2, NULL);
   delay(500);
   xTaskCreate(temp, "Interfacing Temperature Sensor", 10000, NULL, 1, NULL);
   delay(500);
   xTaskCreate(vibration, "Interfacing Vibration Sensor", 10000, NULL, tskIDLE_PRIORITY, NULL);
}

void loop() {
//   String accString = JSON.stringify ("00");
//   delay(1000);
//   str = "update-mq2";
//   sendPostRequest(str, accString);
//   accString = JSON.stringify ("01");
//   delay(1000);
//   str = "update-dist";
//   sendPostRequest(str, accString);
//   accString = JSON.stringify ("02");
//   delay(1000);
//   str = "update-temp";
//   sendPostRequest(str, accString);
//   accString = JSON.stringify ("03");
//   delay(1000);
//   str = "update-vib";
//   sendPostRequest(str, accString);
}

void sendPostRequest(String s, String d) {
  WiFiClient client;
  HTTPClient http;
  String serverPath = "http://192.168.29.129:8080/";
  serverPath = serverPath + s;

  http.begin(client, serverPath); // Change to the correct address/port if needed
  http.addHeader("Content-Type", "application/json");
  // Send the GET request
  int httpCode = http.POST(d);

  if (httpCode > 0) {
    Serial.printf("HTTP Code: %d\n", httpCode);
  } else {
    Serial.println("HTTP GET request failed");
    Serial.printf("HTTP Code: %d\n", httpCode);
  }

  http.end();
}

void mq2(void *pvParameters)
{
    while(1){
    // Inside MQ2 sensor
    int sensor_Aout = analogRead(Sensor_input);  /*Analog value read function*/
    if(sensor_Aout<500) break;
    Serial.print("Gas Sensor: ");   
    Serial.println(sensor_Aout);   /*Read value printed*/
    
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
    str = "update-mq2";
    sendPostRequest(str, reading);
    delay(2000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void ultraSonic(void *pvParameters)
{
    while(1){
    // Inside Ultrasonic Sensor
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
    str = "update-dist";
    sendPostRequest(str, reading);
    delay(2000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void temp(void *pvParameters)
{
    while(1){
    // Inside Temperature Sensor
    sensors.requestTemperatures(); 
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    if(temperatureC<0) break;
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    reading["temp"] = String(temperatureC);
    str = "update-temp";
    sendPostRequest(str, reading);
    delay(2000);
    }
    vTaskDelete( NULL );
}
//--------------------------------------------------------------------
void vibration(void *pvParameters)
{
    while(1){
    // Inside Vibration Vibration Sensor
    bool value = digitalRead(vib);
    if (value == 1) {
      count = count + 1;
      delay(1000);
      reading["vibration"] = String(prev_count);
      str = "update-vib";
      sendPostRequest(str, reading);
    }else if(value == 0){
      if(count != prev_count){
        prev_count = count;
        Serial.println(prev_count);
     }
    }
    }
    vTaskDelete( NULL );
}
