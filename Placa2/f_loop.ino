void on_loop(void* pvParameters) {
const TickType_t espera_maxima = pdMS_TO_TICKS(6000); //6 Segundos
while(1){
  taskENTER_CRITICAL(&emergencia_cambio);
  vTaskDelay(pdMS_TO_TICKS(10));
  if(Emergency == true){
    traceln("Emergencia activada...");
    vTaskDelay(pdMS_TO_TICKS(1000));
    taskEXIT_CRITICAL(&emergencia_cambio);
    continue;
  }
  taskEXIT_CRITICAL(&emergencia_cambio);

  EventBits_t uxBits;
  Mandar_situacion();
  xEventGroupWaitBits(xEventGroup, BIT_0, pdFALSE, pdFALSE, espera_maxima);
  uxBits = xEventGroupGetBits(xEventGroup);  
  
  if((uxBits & BIT_0) != BIT_0){
    traceln("Esperando estaciones libres...");
    continue;
  }
  bool libre2 = false;
  bool libre1 = false;
  bool usado1 = true;
  bool usado2 = true;
  taskENTER_CRITICAL(&cambio_libre);
  libre2 =estaciones.estacion2.libre;
  libre1 = estaciones.estacion1.libre;
  usado2 = estaciones.estacion2.usado;
  usado1 = estaciones.estacion1.usado;
  taskEXIT_CRITICAL(&cambio_libre);
  if(libre2 && (!usado2)){
      Mandar_estacion_2();
      xEventGroupClearBits(xEventGroup,BIT_0);
    }
  delay(10);
  if(libre1 && (!usado1)){
      Mandar_estacion_1();
      xEventGroupClearBits(xEventGroup,BIT_0);
    }
  
  vTaskDelay(pdMS_TO_TICKS(200));
}
}

