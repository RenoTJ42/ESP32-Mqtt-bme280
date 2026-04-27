#include <stdio.h>
#include "mqtt_client.h"
#include "mqtt.h"

static esp_mqtt_client_handle_t client = NULL;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    if (event_id == MQTT_EVENT_CONNECTED)
    {
        printf("MQTT Connected\n");
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = "mqtt://broker.hivemq.com",
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
}

void mqtt_publish_temperature(float temperature)
{
    if (client == NULL)
        return;

    char msg[50];
    snprintf(msg, sizeof(msg), "%.2f", temperature);

    esp_mqtt_client_publish(client,
                            "esp32/bme280/temperature",
                            msg,
                            0,
                            1,
                            0);
}