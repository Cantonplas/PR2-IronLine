#pragma once
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <ArduinoJson.h>
#include <iostream>
#include "driver/gpio.h"
#include "Config.h"


#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif
#include <PubSubClient.h>

//Estructuras y funciones:
typedef enum{
    null_chasis=0,
    chasis1=1,
    chasis2=2
}Tipo_chasis_t;

typedef struct{
    Tipo_chasis_t chasis;
    bool libre;
    String color;
    bool usado;
}Estacion;

typedef struct{
    Estacion estacion1;
    Estacion estacion2;
}Estaciones;


static Estaciones estaciones = {
    {null_chasis, false,"",true},
    {null_chasis, false,"",true}
};


static portMUX_TYPE cambio_libre = portMUX_INITIALIZER_UNLOCKED;
static portMUX_TYPE emergencia_cambio =portMUX_INITIALIZER_UNLOCKED;
QueueHandle_t emergenciaQueue;
EventGroupHandle_t xEventGroup;
static bool Emergency = false;


void setup() {
String deviceID = "Placa2_IronLine";
#ifdef LOGGER_ENABLED
  Serial.begin(BAUDS);
  delay(1000);
  Serial.println();
#endif

  wifi_connect();

  mqtt_connect(deviceID);

  suscribirseATopics();

  on_setup();

}

void loop() {

  wifi_loop();
  mqtt_loop();

}


