
void on_setup() {

    pinMode(LED, OUTPUT);
    setLed(0);
    
    inicializar(&Cola);
    xEventGroup = xEventGroupCreate();
    if(xEventGroup == NULL){
        fatalln("Error creando los Group Events");
    }
    xEventGroupClearBits(xEventGroup,BIT_0|BIT_1);
    solicitud_actual.id =-1;
    xTaskCreate(
    on_loop,         
   "on_loop",       
    2048,             
    NULL,           
    1,                
    NULL              
    );

}

