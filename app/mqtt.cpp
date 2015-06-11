/*
 * mqtt.cpp
 *
 *  Created on: 11 May 2015
 *      Author: Daniel
 */

#include <mqtt.h>
#include <AppSettings.h>


#define ON "1"
#define OFF "0"
#define ON_STR "ON"
#define OFF_STR "OFF"

MqttClient mqtt(MQTT_ADDRESS, MQTT_PORT, onMessageReceived);


void startMqtt()
{
	if (isConnected())
	{
		return;
	}
	else
	{
		mqtt.connect(DEVICE_NAME);
		mqtt.subscribe(MQTT_SUBCRIBE);
		publishMessage();
	}
}

void publishMessage() {
	if (isConnected()) {
		mqtt.publish(MQTT_PUBLISH, digitalRead(PIN)? ON_STR: OFF_STR,true);
	}
}


// Callback for messages, arrived from MQTT server
void onMessageReceived(String topic, String message)
{
	debugf("%s",topic.c_str());
	debugf(":\r\n\t"); // Prettify alignment for printing
	debugf("%s",message.c_str());
	if (strcmp(message.c_str(), ON) == 0)
	{
		debugf(ON_STR);
		digitalWrite(PIN, HIGH);  // Pull GPIO HIGH TO BREAK CIRCUIT
		publishMessage();

	}
	else if (strcmp(message.c_str(), OFF) == 0)
	{
		debugf(OFF_STR);
		digitalWrite(PIN, LOW); // Pull GPIO LOW TO COMPLETE CIRCUIT
		publishMessage();
	}

}

bool isConnected() {
	return (WifiStation.isConnected() && mqtt.isProcessing());
}



