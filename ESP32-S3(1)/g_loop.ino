
void on_loop(void* pvParameters) {
const TickType_t espera_maxima = pdMS_TO_TICKS(60000); //60 segundos
while(1){  
uint8_t numero_estaciones=0;
EventBits_t uxBits;
xEventGroupWaitBits(xEventGroup, BIT_0, pdFALSE, pdFALSE, espera_maxima);
uxBits = xEventGroupGetBits(xEventGroup);
if ((uxBits & (BIT_0 | BIT_1)) == (BIT_0 | BIT_1)) {
  setLed(0);
  traceln("Mandando 2 estaciones...");
  numero_estaciones = 2;
  xEventGroupClearBits(xEventGroup, BIT_0 | BIT_1);
} else if (((uxBits & BIT_1) != BIT_1) && ((uxBits & BIT_0) == BIT_0)) {
  traceln("Mandando 1 estación...");
  numero_estaciones = 1;
  xEventGroupClearBits(xEventGroup, BIT_0);
} else {
  traceln("Esperando estaciones libres...");
  continue;
}
if(Empty(&Cola) && No_hay_solicitud(solicitud_actual)){
  traceln("Modo automatico:");
  Manejo_automatico(&automatica,numero_estaciones);
}else{
  if(No_hay_solicitud(solicitud_actual)){
    traceln("Popeando de la cola:");
  pop(&Cola,&solicitud_actual);
  }
  traceln("Manejando cola:");
  Manejo_solicitud(&solicitud_actual,numero_estaciones);
}
}


}


