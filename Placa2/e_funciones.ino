void Mandar_situacion(){
  StaticJsonDocument<50> doc;
  doc["id_device"]="1";
  taskENTER_CRITICAL(&cambio_libre);
  doc["Estacion1"]=estaciones.estacion1.libre;
  doc["Estacion2"]=estaciones.estacion2.libre;
  taskEXIT_CRITICAL(&cambio_libre);
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  enviarMensajePorTopic(PLACA_TOPIC,jsonBuffer);
  

}

String get_color_id(String color){
if(color == "Rojo"){
    return "0";
  }
  if(color == "Negro"){
    return "1";
  }
  if(color == "Gris"){
    return "2";
  }
  if(color == "Blanco"){
    return "3";
  }
  if(color == "Verde"){
    return "4";
  }
  if(color == "Naranja"){
    return "5";

}
  return "Error";
}


void Mandar_estacion_1(){
  StaticJsonDocument<200> doc;
  doc["id_device"]="0";
  doc["id_estacion"]="1";
  doc["topic"] = "Request";
  taskENTER_CRITICAL(&cambio_libre);
  estaciones.estacion1.usado = true;
  if (estaciones.estacion1.color == nullptr || estaciones.estacion1.color == "") {
      errorln("Color de estacion1 no inicializado");
      return;
  }
  String aux = get_color_id(estaciones.estacion1.color);
  if(aux =="Error"){
    errorln("Error mandando informacion estancion 1");
    return;
  }
  doc["id_color"]= aux;
  doc["tipo_chasis"] = estaciones.estacion1.chasis;
  estaciones.estacion1.libre=false;
  taskEXIT_CRITICAL(&cambio_libre);
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  enviarMensajePorTopic(PYTHON_TOPIC,jsonBuffer);
  }




void Mandar_estacion_2(){
  StaticJsonDocument<200> doc;
  doc["id_device"]=0;
  doc["id_estacion"]="2";
  doc["topic"] = "Request";
  taskENTER_CRITICAL(&cambio_libre);
  estaciones.estacion2.usado = true;
  if (estaciones.estacion2.color == nullptr || estaciones.estacion2.color == "") {
      errorln("Color de estacion1 no inicializado");
      return;
  }
  String aux = get_color_id(estaciones.estacion2.color);
  if(aux =="Error"){
    errorln("Error mandando informacion estancion 1");
    return;
  }
  doc["id_color"]= aux;
  doc["tipo_chasis"] = estaciones.estacion2.chasis;
  estaciones.estacion2.libre = false;
  taskEXIT_CRITICAL(&cambio_libre);
  String jsonBuffer;
  serializeJson(doc, jsonBuffer);
  enviarMensajePorTopic(PYTHON_TOPIC,jsonBuffer);

}



