

uint8_t ledStatus = 0;

void Solicitud_vacia(Solicitud* solicitud){
  if((solicitud->num_chasis1 == 0) && (solicitud->num_chasis2 == 0)){
    solicitud->id =-1;
  }
}

bool No_hay_solicitud(Solicitud solicitud){
  if(solicitud.id == -1){
    return true;
  }
  return false;
}

String Recibir_colores(Colores* colores){
  if(colores->verde > 0){
    colores->verde--;
    return "Verde";
  }
  if(colores->negro > 0){
    colores->negro--;
    return "Negro";
  }
   if(colores->naranja > 0){
    colores->naranja--;
    return "Naranja";
  }
  if(colores->blanco > 0){
    colores->blanco--;
    return "Blanco";
  }
   if(colores->gris > 0){
    colores->gris--;
    return "Gris";
  }
  if(colores->rojo > 0){
    colores->rojo--;
    return "Rojo";
  }
  return "Error";
}


void Manejo_solicitud(Solicitud* solicitud, uint8_t num_estaciones){
  StaticJsonDocument<200> doc;
  String color= "Error";
  doc["id_device"]="0";
  uint8_t i;
  for(i=0;i<num_estaciones;i++){
  if(solicitud->num_chasis1 > 0){
    color = Recibir_colores(&solicitud->colores_chasis1);
    doc["Envios"][i]["Chasis"]=1;
    solicitud->num_chasis1--;
    
  }else{
  if(solicitud->num_chasis2 > 0){
    color = Recibir_colores(&solicitud->colores_chasis2);
    doc["Envios"][i]["Chasis"]=2;
    solicitud->num_chasis2--;

  }else{
    Solicitud_vacia(solicitud);
    break;
  }
  }

  if((color == "Error")){
    errorln("Error a la hora de mandar solicitud a placa 2");
    return;
  }
  doc["Envios"][i]["Color"]=color;
  }
  doc["numero_envios"]= i;
  if((solicitud->num_chasis1 == 0)&&(solicitud->num_chasis2 == 0)){
    traceln("Solicitud termianada...")
    Solicitud_vacia(solicitud);
  }
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  enviarMensajePorTopic(PLACA_TOPIC,jsonBuffer);
}

String Recibir_colores_automatico(Colores* colores){
  if(colores->negro > 0){
    colores->negro--;
    return "Negro";
  }
   if(colores->naranja > 0){
    colores->naranja--;
    return "Naranja";
  }
  if(colores->blanco > 0){
    colores->blanco--;
    return "Blanco";
  }
   if(colores->gris > 0){
    colores->gris--;
    return "Gris";
  }
  if(colores->rojo > 0){
    colores->rojo--;
    return "Rojo";
  }
  colores->negro++;
  colores->blanco++;
  colores->gris++;
  colores->rojo++;
  colores->naranja++;
  return "Verde";
}



void Manejo_automatico(Solicitud* solicitud, uint8_t num_estaciones){
  StaticJsonDocument<200> doc;
  String color;
  doc["id_device"]="0";
  for(int i=0;i<num_estaciones;i++){
  if(solicitud->num_chasis1 > 0){
    color = Recibir_colores_automatico(&solicitud->colores_chasis1);
    doc["Envios"][i]["Chasis"]=1;
    solicitud->num_chasis1--;
    
  }else{
  if(solicitud->num_chasis2 > 0){
    color = Recibir_colores_automatico(&solicitud->colores_chasis2);
    doc["Envios"][i]["Chasis"]=2;
    solicitud->num_chasis1++;
    

  }
  }

  if(color == "Error"){
    errorln("Error a la hora de mandar solicitud a placa 2");
    return;
  }
  doc["Envios"][i]["Color"]=color;
  }
  doc["numero_envios"]= num_estaciones;
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  enviarMensajePorTopic(PLACA_TOPIC,jsonBuffer);
}


void setLed(uint8_t status) {
  if ( ledStatus == status )
    return;
    
  ledStatus = status;
  if ( status ) {
    digitalWrite(LED, HIGH);
  } else {
    infoln("Led: off");
    digitalWrite(LED, LOW);

  }
}


 


