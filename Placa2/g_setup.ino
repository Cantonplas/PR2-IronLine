void Mandar_emergencia_task(void* arg) {
    uint8_t dummy;
    while (true) {
        esp_task_wdt_reset();
        if (xQueueReceive(emergenciaQueue, &dummy, portMAX_DELAY)) {
            taskENTER_CRITICAL(&emergencia_cambio);
            Emergency = !Emergency;
            taskEXIT_CRITICAL(&emergencia_cambio);
            vTaskDelay(pdMS_TO_TICKS(10));
            StaticJsonDocument<200> doc;
            doc["id_device"] = "0";
            doc["topic"] = "Emergency";
            doc["Emergency_stop"] = Emergency;
            String jsonBuffer;
            serializeJson(doc, jsonBuffer);
            vTaskDelay(pdMS_TO_TICKS(10));
            enviarMensajePorTopic(PYTHON_TOPIC, jsonBuffer);
        }
    }
}

static void IRAM_ATTR gpio_isr_handler(void* arg){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t dummy = 0; 
    xQueueSendFromISR(emergenciaQueue, &dummy, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
    
}

void on_setup() {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE; // Flanco de subida
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << GPIO_EMERGENCY);
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

    emergenciaQueue = xQueueCreate(10, sizeof(uint8_t));
    if (emergenciaQueue == NULL) {
        fatalln("Error creando la cola...");
        return;
    }

    xTaskCreate(
    Mandar_emergencia_task, 
    "Mandar_emergencia_task", 
    4096, 
    NULL, 
    1, 
    NULL);

    gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1);
    gpio_isr_handler_add(GPIO_EMERGENCY, gpio_isr_handler, NULL);


    xEventGroup = xEventGroupCreate();
    if(xEventGroup == NULL){
        fatalln("Error creando los Group Events");
        return;
    }
    xEventGroupClearBits(xEventGroup,BIT_0);
    xTaskCreate(
        on_loop,
        "on_loop",
        3048,
        NULL,
        1,
        NULL
    );
}
