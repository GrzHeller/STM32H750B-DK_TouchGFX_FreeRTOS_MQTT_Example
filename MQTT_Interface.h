#ifndef _MQTT_INTERFACE_H_
#define _MQTT_INTERFACE_H_
#include <mqtt.h>

void interface_mqtt_connect(mqtt_client_t *client, const struct mqtt_connect_client_info_t *ci, void *arg);
void interface_mqtt_publish(mqtt_client_t *client, void *arg);

#endif // _MQTT_INTERFACE_H_
