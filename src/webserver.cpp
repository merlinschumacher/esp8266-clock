#include "webserver.h"

ESP8266WebServer _server(80);

Webserver::Webserver(Config *config)
{
    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    _config = config;
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
    if (_filesystem->exists(pathWithGz) || _filesystem->exists(path))
    {
        if (_filesystem->exists(pathWithGz))
        {
            path += ".gz";
        }
        File file = _filesystem->open(path, "r");
        _server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

bool Webserver::_handleDataGet()
{
    Serial.println("Loading config data");
    String message;
    StaticJsonDocument<512> doc = Webserver::_config->configToJSON();
    serializeJson(doc, Serial);
    serializeJson(doc, message);
    Serial.println("====");
    Serial.println(message);
    Serial.println("====");
    _server.send(200, "text/json", message);
    return true;
}

void Webserver::_handleForm()
{
    if (_server.method() != HTTP_POST)
    {
        _server.send(405, "text/plain", "Method Not Allowed");
    }
    else
    {
        String message = "POST form was:\n";
        for (uint8_t i = 0; i < _server.args(); i++)
        {
            message += " " + _server.argName(i) + ": " + _server.arg(i) + "\n";
        }
        _server.send(200, "text/plain", message);
    }
}

void Webserver::setup()
{
    _server.on("/", HTTP_GET, [this]() {
        if (!_handleFileRead("/index.html"))
        {
            _server.send(404, "text/plain", "File not found");
        }
    });
    _server.on("/data.json", HTTP_GET, [this]() { _handleDataGet(); });

    _server.onNotFound([this]() {
        if (!_handleFileRead(_server.uri()))
        {
            _server.send(404, "text/plain", "File not found");
        }
    });

    _server.begin();
}
