#include "mqtt.hpp"
Mqtt::Mqtt()
{
}

void Mqtt::_handleRequest(char *topic, byte *payload, unsigned int length, Config &config)
{
    payload[length] = '\0';
    if (strncmp(topic, commandTopic, sizeof(commandTopic)) == 0)
    {
        if (strncmp((char *)payload, "alarm", length) == 0)
        {
            strlcpy(currentCommand, "alarm", 32);
        }
        else if (strncmp((char *)payload, "rainbow", length) == 0)
        {
            strlcpy(currentCommand, "rainbow", 32);
        }
        else if (strncmp((char *)payload, "off", length) == 0)
        {
            strlcpy(currentCommand, "off", 32);
        }
        else if (strncmp((char *)payload, "getConfig", length) == 0)
        {
            strlcpy(currentCommand, "time", 32);
            publishConfig(config);
        }
        else
        {
            strlcpy(currentCommand, "time", 32);
        }
    }
    else if (strncmp(topic, setConfigTopic, sizeof(setConfigTopic)) == 0)
    {

        DynamicJsonDocument doc(2048);
        deserializeJson(doc, (char *)payload, length);
        bool save = config.JSONToConfig(doc);
        config.tainted = true;
        if (save)
        {
            config.save();
        }
        publishConfig(config);
    }
}

bool Mqtt::connect(Config &config)
{
    if (_mqttClient.connected())
    {
        return true;
    }
    else
    {
        bool connected = _mqttClient.connect(config.config.hostname, config.config.mqttUser, config.config.mqttPassword, statusTopic, 0, true, "shutdown");
        if (connected)
        {
            publishStatus("online");
            _mqttClient.subscribe(setConfigTopic);
            _mqttClient.subscribe(commandTopic);
            _mqttClient.setCallback([this, &config](char *topic, byte *payload, unsigned int length)
                                    { _handleRequest(topic, payload, length, config); });
        }
        return connected;
    };
}

bool Mqtt::setup(Config &config)
{
    _mqttClient.setClient(_wifiClient);
    _mqttClient.setServer(config.config.mqttServer, config.config.mqttPort);
    _mqttClient.setBufferSize(2560);
    snprintf(configTopic, sizeof(configTopic), "%s/config", config.config.mqttBaseTopic);
    snprintf(setConfigTopic, sizeof(setConfigTopic), "%s/setConfig", config.config.mqttBaseTopic);
    snprintf(statusTopic, sizeof(statusTopic), "%s/status", config.config.mqttBaseTopic);
    snprintf(commandTopic, sizeof(commandTopic), "%s/command", config.config.mqttBaseTopic);
#ifdef DEBUG_BUILD
    snprintf(debugTopic, sizeof(debugTopic), "%s/debug", config.config.mqttBaseTopic);
#endif

    if (connect(config))
    {
        return true;
    }
    else
    {
        return false;
    };
}

void Mqtt::publish(const char *message, const char *topic, bool retain)
{
    _mqttClient.publish(topic, message, retain);
}

void Mqtt::publishConfig(Config &config)
{
    DynamicJsonDocument doc(2048);
    config.configToJSON(doc, true);
    doc.shrinkToFit();
    char response[measureJson(doc) + 1];
    serializeJson(doc, response, sizeof(response));
    publish(response, configTopic, false);
}

#ifdef DEBUG_BUILD
void Mqtt::publishUptime()
{
    char s[128];
    snprintf(s, sizeof(s), "{\"uptime\": %lu}", millis() / 1000);
    _mqttClient.publish(debugTopic, s, false);
}
#endif

void Mqtt::publishStatus(const char *status)
{
    if (strncmp(status, _lastStatus, sizeof(_lastStatus)) != 0)
    {
        publish(status, statusTopic, true);
        strlcpy(_lastStatus, status, sizeof(_lastStatus));
    }
}

void Mqtt::loop()
{
    _mqttClient.loop();
}