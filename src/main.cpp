#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>

ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
FS *filesystem = &SPIFFS;

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
}

String getContentType(String filename)
{
  if (server.hasArg("download"))
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

bool handleFileRead(String path)
{
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
  {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (filesystem->exists(pathWithGz) || filesystem->exists(path))
  {
    if (filesystem->exists(pathWithGz))
    {
      path += ".gz";
    }
    File file = filesystem->open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void handleDataGet()
{
  String message = "{}";
  server.send(200, "text/json", message);
}
void handleForm()
{
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
  }
  else
  {
    String message = "POST form was:\n";
    for (uint8_t i = 0; i < server.args(); i++)
    {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(200, "text/plain", message);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset saved settings
  //wifiManager.resetSettings();

  //fetches ssid and pass from eeprom and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration

  String chipid = "ESP_CLOCK-" + String(ESP.getChipId()); // The chip ID is essentially its MAC address(length: 6 bytes).
  wifiManager.autoConnect(chipid.c_str());
  //or use this for auto generated name ESP + ChipID
  //if you get here you have connected to the WiFi
  Serial.println("Connected to WiFi");
  timeClient.begin();
  timeClient.update();
  Serial.println("Updated time via NTP");
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  server.on("/", HTTP_GET, []() {
    if (!handleFileRead("/index.html"))
    {
      server.send(404, "text/plain", "File not found");
    }
  });
  server.on("/data.json", HTTP_GET, handleDataGet);

  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
    {
      server.send(404, "text/plain", "File not found");
    }
  });

  server.begin();
  ArduinoOTA.setHostname(chipid.c_str());
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();
  MDNS.update();
  // put your main code here, to run repeatedly:
}