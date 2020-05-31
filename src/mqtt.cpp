#include "mqtt.h"
#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Mqtt::Mqtt()
{
}

void Mqtt::handleRequest(char *topic, byte *payload, unsigned int length)
{
}

void Mqtt::connect()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        return;
    }
    if (mqttClient.connected())
    {
        return;
    }
    mqttClient.setServer(server, port);
    mqttClient.connect(clientID, username, password);
    mqttClient.setCallback(handleRequest);
    mqttClient.publish(_statusTopic, "{state:\"online\"");
    mqttClient.subscribe(_statusTopic);
    mqttClient.subscribe(_configTopic);
    mqttClient.subscribe(_effectTopic);
}

void Mqtt::publish(String message, uint8_t type = 0)
{
    mqttClient.publish(_statusTopic, message);
}