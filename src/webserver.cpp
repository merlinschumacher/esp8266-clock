#include "webserver.h"

#if defined(ESP8266)
ESP8266WebServer _server(80);
ESP8266HTTPUpdateServer _httpUpdater;
#elif defined(ESP32)
WebServer _server(80);
#endif

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
#if defined(ESP8266)
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
#elif defined(ESP32)
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
#endif
    return false;
}

void Webserver::_handleDataGet(Config &config)
{
    Serial.println("Loading config data");
    String message;
    config.load();
    StaticJsonDocument<1024> doc = config.configToJSON();
    serializeJson(doc, message);
    _server.send(200, "text/json", message);
}

void Webserver::_handleDataPut(Config &config)
{
    String message = _server.arg(0);
    StaticJsonDocument<1024> doc;
    deserializeJson(doc, message);
    bool save = config.JSONToConfig(doc);
    if (save == true)
    {
        config.save();
    };

    doc.clear();
    message = "";
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
    _server.on("/time", HTTP_GET, [this]() { _server.send(200, "text/plain", currentTime); });
    _server.on("/timezones.json", HTTP_GET, [this]() { _server.send_P(200, "application/x-gzip", timezones_json_gz, sizeof(timezones_json_gz)); });
    _server.on("/water.css", HTTP_GET, [this]() { _server.send_P(200, "application/x-gzip", water_css_gz, sizeof(water_css_gz)); });
    _server.on("/data.json", HTTP_GET, [this, &config]() { _handleDataGet(config); });
    _server.on("/data.json", HTTP_POST, [this, &config]() { _handleDataPut(config); });

    _server.onNotFound([this]() {
        if (!_handleFileRead(_server.uri()))
        {
            _server.send(404, "text/plain", "File not found");
        }
    });
#if defined(ESP8266)
    _httpUpdater.setup(&_server);
#elif defined(ESP32)
    _server.on(
        "/update", HTTP_POST, []() {
      _server.sendHeader("Connection", "close");
      _server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart(); }, []() {
      HTTPUpload& upload = _server.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) { //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update Failed Unexpectedly (likely broken connection): status=%d\n", upload.status);
      } });
#endif
    _server.begin();
}
