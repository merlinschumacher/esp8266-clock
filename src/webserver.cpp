#include "webserver.hpp"
#include "index_en_html.hpp"
#include "index_de_html.hpp"
#include "scripts_js.hpp"
#include "styles_css.hpp"

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

void Webserver::_handleDataGet(Config &config)
{
  Serial.println("Loading config data");
  char message[2048];
  config.load();
  StaticJsonDocument<2048> doc = config.configToJSON();
  serializeJson(doc, message);
  _server.send(200, "text/json", message);
}

void Webserver::_handleDataPut(Config &config)
{
  char message[2048];
  strlcpy(message, _server.arg(0).c_str(), sizeof(message));
  StaticJsonDocument<2048> doc;
  deserializeJson(doc, message);
  bool save = config.JSONToConfig(doc);
  if (save == true)
  {
    config.save();
  };

  doc.clear();
  memset(&message[0], 0, sizeof(message));
  doc = config.configToJSON();
  serializeJson(doc, message);
  _server.send(200, "text/json", message);
}

void Webserver::_resetConfig(Config &config)
{
  Serial.println("Resetting config file and rebooting.");
  StaticJsonDocument<1> doc;
  deserializeJson(doc, "");
  config.save();

  WiFi.disconnect(true);
#if defined(ESP8266)
  LittleFS.format();
#elif defined(ESP32)
  SPIFFS.format();
#endif
  ESP.restart();
  _server.send(200, "text/plain", "reset");
}

void Webserver::_handleIndex(Config &config)
{
  _server.sendHeader("Content-Encoding", "gzip");
  if (strcmp(config.config.language, "de") == 0)
  {
    _server.send_P(200, "text/html", index_de_html_gz, index_de_html_gz_len);
  }
  else
  {
    _server.send_P(200, "text/html", index_en_html_gz, index_en_html_gz_len);
  }
}

void Webserver::setup(Config &config)
{
  _server.on("/", HTTP_GET, [this, &config]() { _handleIndex(config); });
  _server.on("/index.html", HTTP_GET, [this, &config]() { _handleIndex(config); });
  _server.on("/styles.css", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "text/css", styles_css_gz, styles_css_gz_len); });
  _server.on("/scripts.js", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "application/javascript", scripts_js_gz, scripts_js_gz_len); });

  _server.on("/time", HTTP_GET, [this]() { _server.send(200, "text/plain", currentTime); });
  _server.on("/version", HTTP_GET, [this]() { _server.send(200, "text/plain", VERSION); });

  _server.on("/reset", HTTP_GET, [this, &config]() { _resetConfig(config); _server.send(200, "text/plain", ""); });

  _server.on("/data.json", HTTP_GET, [this, &config]() { _handleDataGet(config); });
  _server.on("/data.json", HTTP_POST, [this, &config]() { _handleDataPut(config); });

  _server.onNotFound([this]() {
    _server.send(404, "text/plain", "File not found");
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
