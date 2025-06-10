// COMM BAUDS
#define BAUDS 115200


#define LOGGER_ENABLED            // Comentar para deshabilitar el logger por consola serie

#define LOG_LEVEL TRACE           // nivells en c_logger: TRACE, DEBUG, INFO, WARN, ERROR, FATAL, NONE


// WIFI
#define NET_SSID                  "UPV-PSK"
#define NET_PASSWD                "giirob-pr2-2023"

// MQTT
#define MQTT_SERVER_IP            "mqtt.dsic.upv.es"
#define MQTT_SERVER_PORT          1883
#define MQTT_USERNAME             "giirob"    
#define MQTT_PASSWORD             "UPV2024"

#define PLACA_TOPIC                 "giirob/pr2/ironline/placa"
#define PYTHON_TOPIC                "giirob/pr2/ironline/python"


//Macros:
#define BIT_0 (1<<0)
#define GPIO_EMERGENCY gpio_num_t(4)





