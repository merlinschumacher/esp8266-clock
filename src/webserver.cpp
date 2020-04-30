#include "webserver.h"

ESP8266WebServer _server(80);
ESP8266HTTPUpdateServer _httpUpdater;

Webserver::Webserver()
{
}

void Webserver::handleRequest()
{
    _server.handleClient();
}

String Webserver::_getContentType(String filename)
{
    if (_server.hasArg("download"))
    {
        return "application/octet-stream";
    }
    else if (filename.endsWith(".htm"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".html"))
    {
        return "text/html";
    }
    else if (filename.endsWith(".css"))
    {
        return "text/css";
    }
    else if (filename.endsWith(".js"))
    {
        return "application/javascript";
    }
    else if (filename.endsWith(".png"))
    {
        return "image/png";
    }
    else if (filename.endsWith(".gif"))
    {
        return "image/gif";
    }
    else if (filename.endsWith(".jpg"))
    {
        return "image/jpeg";
    }
    else if (filename.endsWith(".ico"))
    {
        return "image/x-icon";
    }
    else if (filename.endsWith(".xml"))
    {
        return "text/xml";
    }
    else if (filename.endsWith(".pdf"))
    {
        return "application/x-pdf";
    }
    else if (filename.endsWith(".zip"))
    {
        return "application/x-zip";
    }
    else if (filename.endsWith(".gz"))
    {
        return "application/x-gzip";
    }
    return "text/plain";
}

void Webserver::_handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += _server.uri();
    message += "\nMethod: ";
    message += (_server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += _server.args();
    message += "\n";

    for (uint8_t i = 0; i < _server.args(); i++)
    {
        message += " " + _server.argName(i) + ": " + _server.arg(i) + "\n";
    }

    _server.send(404, "text/plain", message);
}

bool Webserver::_handleFileRead(String path)
{
    Serial.println("handleFileRead: " + path);
    if (path.endsWith("/"))
    {
        path += "index.htm";
    }
    String contentType = _getContentType(path);
    String pathWithGz = path + ".gz";
    if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
    {
        if (SPIFFS.exists(pathWithGz))
        {
            path += ".gz";
        }
        File file = SPIFFS.open(path, "r");
        _server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

void Webserver::_handleDataGet(Config &config)
{
    Serial.println("Loading config data");
    String message;
    StaticJsonDocument<512> doc = config.configToJSON();
    serializeJson(doc, message);
    _server.send(200, "text/json", message);
}

void Webserver::_handleDataPut(Config &config)
{
    String message;
    message = _server.arg(0);
    StaticJsonDocument<512> doc;
    Serial.println(message);
    deserializeJson(doc, message);
    serializeJson(doc, Serial);
    config.JSONToConfig(doc);
    config.save();

    doc = config.configToJSON();
    serializeJson(doc, message);
    _server.send(200, "text/json", message);
}

void Webserver::setup(Config &config)
{
    _server.on("/", HTTP_GET, [this]() {
        if (!_handleFileRead("/index.html"))
        {
            _server.send(404, "text/plain", "File not found");
        }
    });
    _server.on("/data.json", HTTP_GET, [this, &config]() { _handleDataGet(config); });
    _server.on("/data.json", HTTP_POST, [this, &config]() { _handleDataPut(config); });

    _server.onNotFound([this]() {
        if (!_handleFileRead(_server.uri()))
        {
            _server.send(404, "text/plain", "File not found");
        }
    });
    _httpUpdater.setup(&_server);
    _server.begin();
}
