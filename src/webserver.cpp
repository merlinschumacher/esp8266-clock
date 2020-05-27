#include "webserver.h"
#include "index_html.h"
#include "main_js.h"
#include "timezones_json.h"
#include "water_css.h"

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

void Webserver::_resetConfig(Config &config)
{
  Serial.println("Resetting config file and rebooting.");
  StaticJsonDocument<1> doc;
  deserializeJson(doc, "");
  config.save();

  WiFi.disconnect(true);
  SPIFFS.format();
  ESP.restart();
  _server.send(200, "text/plain", "reset");
}

void Webserver::setup(Config &config)
{
  _server.on("/", HTTP_GET, [this]() {_server.sendHeader("Content-Encoding", "gzip"); _server.send_P(200, "text/html", index_html_gz, index_html_gz_len); });
  _server.on("/index.html", HTTP_GET, [this]() {_server.sendHeader("Content-Encoding", "gzip"); _server.send_P(200, "text/html", index_html_gz, index_html_gz_len); });
  _server.on("/timezones.json", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "text/json", timezones_json_gz, timezones_json_gz_len); });
  _server.on("/water.css", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "text/css", water_css_gz, water_css_gz_len); });
  _server.on("/main.js", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "application/javascript", main_js_gz, main_js_gz_len); });

  _server.on("/time", HTTP_GET, [this]() { _server.send(200, "text/plain", currentTime); });

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
