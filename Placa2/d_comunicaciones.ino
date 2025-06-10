void suscribirseATopics() {
  
  mqtt_subscribe(PLACA_TOPIC);
  mqtt_subscribe(PYTHON_TOPIC);

}


void alRecibirMensajePorTopic(char* topic, String incomingMessage) {
  if (strcmp(topic, PLACA_TOPIC) == 0 ) {
    StaticJsonDocument<200> dic;
    DeserializationError error = deserializeJson(dic, incomingMessage);
   if (error) {
    errorln("Diccionario JSON no ha podido ser creado");
   }
   if(dic["id_device"]== "1"){
    return;
   }
   traceln("<<~~ RECEIVING an MQTT message:");
   traceln(topic);
   traceln(incomingMessage);
   
    bool robot2_primero = true;
   
    taskENTER_CRITICAL(&cambio_libre);
    for(int i =0;i<dic["numero_envios"];i++){
      if(robot2_primero && estaciones.estacion2.libre){
        estaciones.estacion2.chasis = dic["Envios"][i]["Chasis"];
        estaciones.estacion2.color = dic["Envios"][i]["Color"].as<String>();
        estaciones.estacion2.usado = false;
        traceln(estaciones.estacion2.color);
        robot2_primero = false;
      }else{
          estaciones.estacion1.chasis = dic["Envios"][i]["Chasis"];
          estaciones.estacion1.color = dic["Envios"][i]["Color"].as<String>();
          estaciones.estacion1.usado = false;
          traceln(estaciones.estacion1.color);
          }
      }
    taskEXIT_CRITICAL(&cambio_libre);
    xEventGroupSetBits(xEventGroup,BIT_0);
    return;
  }
  if(strcmp(topic, PYTHON_TOPIC)==0){
    StaticJsonDocument<200> dic;
    DeserializationError error = deserializeJson(dic, incomingMessage);
   if (error) {
    errorln("Diccionario JSON no ha podido ser creado");
   }
   if(dic["id_device"]=="0"){
    return;
   }
   traceln("<<~~ RECEIVING an MQTT message:");
   traceln(topic);
   traceln(incomingMessage);
   
    taskENTER_CRITICAL(&cambio_libre);
    estaciones.estacion1.libre=dic["Robot1"];
    estaciones.estacion2.libre=dic["Robot2"];
    taskEXIT_CRITICAL(&cambio_libre);
    return;
  }
  errorln("Mensaje de Topic no válido");
   

}

void enviarMensajePorTopic(const char* topic, String outgoingMessage) {

  mqtt_publish(topic, outgoingMessage.c_str());

}





