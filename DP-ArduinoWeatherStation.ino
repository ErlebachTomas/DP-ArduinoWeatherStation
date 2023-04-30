#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// == BLUETOOTH modul ==
#define RX 11
#define TX 10
SoftwareSerial bluetooth(TX, RX);
//== VLHKOMER  ==
#include <DHT.h>
#define DHTpin 5
#define DHTtype DHT11
DHT sensorDHT(DHTpin, DHTtype);
//== DISPLAY ==
#include <U8glib.h>
U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE);
// == BMP280 ==
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#define BMP280_ADRESS (0x76)
Adafruit_BMP280 bmp;
// == D18B20 ==
#include <OneWire.h>
#include <DallasTemperature.h>
#define D18B20pin 2
OneWire oneWire(D18B20pin);
DallasTemperature snowSensor(&oneWire);

void setup()
{

  bluetooth.begin(9600); // Bluetooth init
  Serial.begin(9600);
  sensorDHT.begin();
  snowSensor.begin();

  if (!bmp.begin(BMP280_ADRESS))
  { // osetreni sbernice
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  display.setFont(u8g_font_unifont);
}

void loop()
{

  float humidity = sensorDHT.readHumidity();
  float tepBMP = bmp.readTemperature();
  snowSensor.requestTemperatures();
  float tepSnow = snowSensor.getTempCByIndex(0);       // prvni čidlo
  float pressure = (bmp.readPressure() / 100.00) + 32; // korekce měření v hPa

  display.firstPage();
  do
  {
    display.setPrintPos(0, 10);
    display.print("Vzduch: ");
    display.print(tepBMP);
    display.print(" C");
    display.setPrintPos(0, 30);
    display.print("Snih: ");
    display.print(tepSnow);
    display.print(" C");
    display.setPrintPos(0, 45);
    display.print("Vlhkost: ");
    display.print(humidity);
    display.print(" %");
    display.setPrintPos(0, 60);
    display.print("Tlak: ");
    display.print(pressure);
    display.print(" hPa");
  } while (display.nextPage());

  if (bluetooth.available() > 0)
  {
    char request = (char) bluetooth.read();    
    if (request == '1') {
      StaticJsonDocument<200> jsonDoc;
      jsonDoc["airTemperature"] = tepBMP;
      jsonDoc["snowTemperature"] = tepSnow;
      jsonDoc["humidity"] = humidity;
      serializeJson(jsonDoc, bluetooth);
      bluetooth.println();
    } else {
      bluetooth.println(request);
    }
  }
  delay(3500);
}
