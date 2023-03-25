//mq2 gas sensor
int LED = 32;            /*LED pin defined*/
int Sensor_input = 33;    /*Digital pin 5 for sensor input*/
//

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

//For Vibration sensor
int vib = 13;
int count = 0;
int prev_count = 0;
//

void setup()
{
    Serial.begin(115200);
    delay(1000);
    
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

void loop()
{delay(1000);}

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
    }
    else {
//      Serial.println("No Gas");
      digitalWrite (LED, LOW) ;  /*LED set LOW if NO Gas detected */
    }
    delay(10000);
    }
    vTaskDelete( NULL );
}

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
    delay(10000);
    }
    vTaskDelete( NULL );
}

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
    delay(10000);
    }
    vTaskDelete( NULL );
}

void vibration(void *pvParameters)
{
    while(1){
    //Serial.println("Inside Vibration");
    bool value = digitalRead(vib);
    if (value == 1) {
      count = count + 1;
      delay(1000);
    }else if(value == 0){
      if(count != prev_count){
        prev_count = count;
        Serial.println(prev_count);
      }
    }
    }
    vTaskDelete( NULL );
}
