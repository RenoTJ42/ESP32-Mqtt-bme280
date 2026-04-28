#ifndef MQTT_H
#define MQTT_H

void mqtt_app_start(void);
void mqtt_publish_temperature(float temperature);
void mqtt_subscribe(void);

#endif