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

void Webserver::_handleDataGet(Config &config)
{
  Serial.println("Loading config data");
  char message[1280];
  config.load();
  DynamicJsonDocument doc(1600);
  config.configToJSON(doc);
  serializeJson(doc, message);
  _server.send(200, "text/json", message);
}

void Webserver::_handleDataPut(Config &config)
{
  char message[1280];
  strlcpy(message, _server.arg(0).c_str(), sizeof(message));
  DynamicJsonDocument doc(1600);
  deserializeJson(doc, message);
  bool save = config.JSONToConfig(doc);
  if (save == true)
  {
    config.save();
  };

  doc.clear();
  doc.garbageCollect();
  memset(&message[0], 0, sizeof(message));
  config.configToJSON(doc);
  serializeJson(doc, message);
  _server.send(200, "text/json", message);
}

void Webserver::_resetConfig(Config &config)
{
  Serial.println("Resetting config file and rebooting.");
  WiFi.disconnect(true);
#if defined(ESP8266)
  LittleFS.format();
#elif defined(ESP32)
  SPIFFS.format();
#endif
  ESP.restart();
  _server.send(200, "text/plain", "reset");
}

void Webserver::_handleIndex(char *lang)
{
  _server.sendHeader("Content-Encoding", "gzip");
  if (strcmp(lang, "de") == 0)
  {
    _server.send_P(200, "text/html", index_de_html_gz, index_de_html_gz_len);
  }
  else
  {
    _server.send_P(200, "text/html", index_en_html_gz, index_en_html_gz_len);
  }
}

void Webserver::_handleTime()
{
  char buf[16];
  itoa(UTC.now(), buf, 10);
  _server.send(200, "text/plain", buf);
}

void Webserver::setup(Config &config)
{
  _server.on("/", HTTP_GET, [this, &config]() { _handleIndex(config.config.language); });
  _server.on("/index.html", HTTP_GET, [this, &config]() { _handleIndex(config.config.language); });
  _server.on("/styles.css", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "text/css", styles_css_gz, styles_css_gz_len); });
  _server.on("/scripts.js", HTTP_GET, [this]() { _server.sendHeader("Content-Encoding", "gzip");_server.send_P(200, "application/javascript", scripts_js_gz, scripts_js_gz_len); });

  _server.on("/time", HTTP_GET, [this]() { _handleTime(); });
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
