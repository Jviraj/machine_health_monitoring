void setup()
{
    Serial.begin(115200);
//    delay(1000);
    xTaskCreate(mq2, "Interfacing MQ2", 10000, NULL, 4, NULL);
    delay(2500);
    xTaskCreate(ultraSonic, "Interfacing HC SR04", 10000, NULL, 3, NULL);
    delay(2500);
    xTaskCreate(temp, "Interfacing Temperature Sensor", 10000, NULL, 2, NULL);
    delay(2500);
    xTaskCreate(vibration, "Interfacing Vibration Sensor", 10000, NULL, 1, NULL);
}

void loop()
{delay(1000);}

void mq2(void *pvParameters)
{
    while(1){
    Serial.println("Inside MQ2");
    delay(10000);
    }
    vTaskDelete( NULL );
}

void ultraSonic(void *pvParameters)
{
    while(1){
    Serial.println("Inside Ultrasonic");
    delay(10000);
    }
    vTaskDelete( NULL );
}

void temp(void *pvParameters)
{
    while(1){
    Serial.println("Inside Temperature");
    delay(10000);
    }
    vTaskDelete( NULL );
}

void vibration(void *pvParameters)
{
    while(1){
    Serial.println("Inside Vibration");
    delay(10000);
    }
    vTaskDelete( NULL );
}
