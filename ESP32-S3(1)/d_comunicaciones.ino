
void suscribirseATopics() {
  
  mqtt_subscribe(PC_TOPIC);
  mqtt_subscribe(PLACA_TOPIC);

}

void alRecibirMensajePorTopic(char* topic, String incomingMessage) {
  
  if (strcmp(topic, PC_TOPIC) == 0 ) {
    StaticJsonDocument<200> dic;
    DeserializationError errores = deserializeJson(dic, incomingMessage);
    

   if (errores) {
    errorln("Diccionario JSON no ha podido ser creado");
    return;
   }
    setLed(1);
    traceln("<<~~ RECEIVING an MQTT message:");
    traceln(topic);
    traceln(incomingMessage);
    Solicitud aux{
        .id = dic["id_pedido"],
        .num_chasis1 = dic["num_chasis_1"],
        .num_chasis2 = dic["num_chasis_2"],
        .colores_chasis1{.verde=dic["colores_chasis_1"]["verde"],.negro=dic["colores_chasis_1"]["negro"],
        .rojo=dic["colores_chasis_1"]["rojo"],.gris=dic["colores_chasis_1"]["gris"],.blanco=dic["colores_chasis_1"]["blanco"],
        .naranja=dic["colores_chasis_1"]["naranja"]},
        .colores_chasis2{.verde=dic["colores_chasis_2"]["verde"],.negro=dic["colores_chasis_2"]["negro"],
        .rojo=dic["colores_chasis_2"]["rojo"],.gris=dic["colores_chasis_2"]["gris"],.blanco=dic["colores_chasis_2"]["blanco"],
        .naranja=dic["colores_chasis_2"]["naranja"]} 
    }; 
    push(&Cola,aux);
  }

  if (strcmp(topic, PLACA_TOPIC) == 0 ) {
    StaticJsonDocument<50> dic;
    DeserializationError errores = deserializeJson(dic, incomingMessage);
    
   if (errores) {
    errorln("Diccionario JSON no ha podido ser creado");
    return;
   }
   if(dic["id_device"]=="0"){
    return;
   }
   traceln("<<~~ RECEIVING an MQTT message:");
   traceln(topic);
   traceln(incomingMessage);
   bool estacion1 = dic["Estacion1"];
   bool estacion2 = dic["Estacion2"];

    if(!estacion1 && estacion2) {
      traceln("Solo estacion delante");
      xEventGroupSetBits(xEventGroup,BIT_1);
    }
    else if (estacion1 && estacion2) {
      traceln("Las 2 estaciones:");
      xEventGroupSetBits(xEventGroup,BIT_0|BIT_1);
    }
    else if(estacion1 && !estacion2){
      traceln("Solo estacion atras:");
      xEventGroupSetBits(xEventGroup,BIT_0);
    }
    
  }

}

void enviarMensajePorTopic(const char* topic, String outgoingMessage) {

  mqtt_publish(topic, outgoingMessage.c_str());

}





