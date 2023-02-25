#include <Arduino_FreeRTOS.h>

void setup()
{
    xTaskcreate(MQ2, "Interfacing MQ2", 100, NULL, 1, NULL);
    vTaskDelay(2500);
    xTaskcreate(UltraSonic, "Interfacing HC SR04", 100, NULL, 1, NULL);
    vTaskDelay(2500);
    xTaskcreate(Temp, "Interfacing Temperature Sensor", 100, NULL, 1, NULL);
    vTaskDelay(2500);
    xTaskcreate(Vibration, "Interfacing Vibration Sensor", 100, NULL, 1, NULL);
    vTaskDelay(2500);
}

void loop()
{}

void MQ2(void *pvParameters)
{
    Serial.println("Inside MQ2");
    vTaskDelay(10000);
}

void UltraSonic(void *pvParameters)
{
    Serial.println("Inside Ultrasonic");
    vTaskDelay(10000);
}

void Temp(void *pvParameters)
{
    Serial.println("Inside Temperature");
    vTaskDelay(10000);
}

void Vibration(void *pvParameters)
{
    Serial.println("Inside Vibration");
    vTaskDelay(10000);
}