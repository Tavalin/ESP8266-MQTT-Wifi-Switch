/*
 * mqtt.h
 *
 *  Created on: 11 May 2015
 *      Author: Daniel
 */

#ifndef INCLUDE_MQTT_H_
#define INCLUDE_MQTT_H_

#include <SmingCore/SmingCore.h>

#define MQTT_ADDRESS "danielwalters.duckdns.org"
#define MQTT_PORT 1883
#define MQTT_SUBCRIBE "dev/command"
#define MQTT_PUBLISH "dev/status"

void startMqtt();
void onMessageReceived(String topic, String message);
void publishMessage();
bool isConnected();

#endif /* INCLUDE_MQTT_H_ */
