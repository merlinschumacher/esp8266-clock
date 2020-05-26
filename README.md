# ESP NeoPixel clock 

<img align="right" width="200" src="clock.gif" alt="A NeoPixel clock ticking away" style="margin-left: 1em"/>

This is a firmware for a ESP8266 based NeoPixel / WS281x clock. It shows the hour, minute and second of the day using the LEDs. 

The firmware automatically gets the current time via NTP, supports timezones and will automatically switch to DST.

The colors of the clock are entirely configurable. The "hands" can have different colors in the night. Colors for the quarters, hours and the current hour segment are configurable as well. 

## Hardware

To build a clock like that you'll need an ESP8266 microcontroller. It should have one freely usable GPIO pin. You'll also need a WS2812/WS2813/NeoPixel LED strip with at best 60 LEDs. More or less LEDs will work also, but will not look as good. Connect the strip to the ESP with +5V going to +5V on the ESP, GND to GND and the data line of the strip to the GPIO. Optimally select GPIO3 or GPIO2 or GPIO1. If none of those are available select any other unused. 

## Setup

To use the firmware flash the `firmware.bin` found in the releases list with the [esptool](https://github.com/espressif/esptool).

```plaintext
$ esptool.py --port PORT write_flash 0x0 firmware.bin
```

After the flashing is done connect to the WiFi access point starting with "⏰ESPCLOCK-...". Connect to it and open the URL [http://192.168.4.1](http://192.168.4.1). Configure your WiFi credentials and check the IP address of the clock in the web interface of your router. Open the address of the clock to change the settings according to your needs.

## Settings

### Color and time settings
The first two blocks control the color of the hands, the hour dots, the quarter dots and the hour segment. The segment are the stretch of LEDs that reflect the current hour. The colors can differ between night and day to make a clock dimmed in the night and bright in the day. When the night colors should be used is definded by following section.

<img align="left" width="350" src="screenshot.jpg" alt="A NeoPixel clock ticking away" style="margin-right: 1em"/>

If you want a visual alarm at a certain time, you can enable it in the "Set alarm"-section and set the time. The clock will flash in red for a minute, when the alarm is set.

To have the clock light up in a rainbow color on the hour just check the box.

If you want the colors of two elements to blend when they overlap enable "Blend colors when lights overlap". This way for example a red hour hand and a blue second hand overlapping will create a purple light.

To select your current timezome just click on the dropdown field and look for your contintent and city.

### LED settings

To select the GPIO that is connected to the data line of your LED strip select a pin. This is the raw GPIO pin number so check the datasheet of your ESP-model for the correct number.

If your strip contains more or less than 60 LEDs you can specify them. Note that due to rounding issues there will be skips in the LEDs. 60 LEDs is the optimum. There currently is no interpolation for more or less than 60 LEDs.

If your LED does not start at the top (12 o'clock). You can add an offset by specifing the pixel located at the top. 

### Saving

Some changes, like the colors, are updated immediately but not permamently saved. To make them permanent click the big green button at the top.

Changes to the LED configuration or the timezone may need a restart of the ESP.

### Advanced settings and updates

If you have a NTP server in your local network you can specify it manually. The default "pool.ntp.org" should be sufficient for most uses.

To make the clock easier identifiable you can also change it's hostname. If your local WiFi router resolves local hostnames it will be reachable by that name.

If something is messed or you just want to reset the clock click the "Reset all settings" button. It will completely remove all settings from the ESP.

You can update the firmware version using the web interface by uploading a file in the "Software update" section. 

## Notes and Links

The firmware has been build with [PlatformIO](https://platformio.org/). It uses the libraries [WifiManager](https://github.com/tzapu/WiFiManager), [ArduinoJSON](https://arduinojson.org/), [NeoPixelBus](https://github.com/Makuna/NeoPixelBus/) and [ezTime](https://github.com/ropg/ezTime). The web interface uses [Water.css](https://watercss.netlify.app/) for styling.

The software was inspired by the [esp8266-NeoPixel-Clock](https://github.com/radimkeseg/esp8266-NeoPixel-Clock) by Radim Keseg. The rainbow was taken from an [example by ACROBOTIC](https://github.com/acrobotic/Ai_Demos_NeoPixelBus/blob/master/Rainbow/Rainbow.ino). The string splitting function was taken from [here](https://github.com/BenTommyE/Arduino_getStringPartByNr/blob/master/getStringPartByNr.ino).

Due to the fact, that the firmware uses the NeoPixelBus bitbanging method there might be issues with longer LED strips. Strips up to 60 LEDs worked fine for me. If you have issues with the LEDs not lighting up correctly change the method in the code and build your own version. See the [NeoPixelBus wiki](https://github.com/Makuna/NeoPixelBus/wiki/ESP8266-NeoMethods#neoesp8266dma800kbpsmethod) for further details. 

If your strip uses a different color order than GRB you also have to modify the firmware, to have proper color reproduction. The [NeoPixelBus wiki](https://github.com/Makuna/NeoPixelBus/wiki/NeoPixelBus-object#neo-features) is also helpful for that.

The firmware should work with the ESP32, but you'll have to compile it yourself and I haven't tested it. 