#include <WiFi.h>
#ifdef SSL_ROOT_CA
  #include <WiFiClientSecure.h>
#endif
#include <PubSubClient.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <ArduinoJson.h>
#include <iostream>
#include "freertos/timers.h"


#include "Config.h"

//Estructuras y funciones:

typedef struct {
  uint32_t verde;
  uint32_t negro; 
  uint32_t rojo;
  uint32_t gris;
  uint32_t blanco;
  uint32_t naranja;
}Colores;

typedef struct{
    int32_t id;
    uint32_t num_chasis1;
    uint32_t num_chasis2;
    Colores colores_chasis1;
    Colores colores_chasis2;
}Solicitud;

typedef struct {
    Solicitud elementos[MAX_STACK_SIZE];
    int top;
}Queue;


void inicializar(Queue* cola) {
    cola->top = -1;
}

bool Empty(Queue* cola) {
    return cola->top == -1;
}

bool Full(Queue* cola) {
    return cola->top == MAX_STACK_SIZE - 1;
}

bool push(Queue* cola, Solicitud nueva_solicitud) {
    if (Full(cola)) {
        return 0;
    }
    cola->elementos[++(cola->top)] = nueva_solicitud;
    return 1;
}

bool pop(Queue* cola, Solicitud* solicitud_extraida) {
    if (Empty(cola)) {
        return 0;
    }
    *solicitud_extraida = cola->elementos[0];
    for(int i=0;i<(cola->top);i++){
        int j = i+1;
        cola->elementos[i]=cola->elementos[j];

    }
    cola->top--;
    return 1;
}



static Queue Cola;
EventGroupHandle_t xEventGroup;
static Solicitud solicitud_actual;
static Solicitud automatica{
        .id = 1,
        .num_chasis1 = 1,
        .num_chasis2 = 1,
        .colores_chasis1{.verde=1,.negro=1,.rojo=1,.gris=1,.blanco=1,.naranja=1},
        .colores_chasis2{.verde=1,.negro=1,.rojo=1,.gris=1,.blanco=1,.naranja=1}
  };


void setup() {
String deviceID = "Placa1_IronLine";
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


