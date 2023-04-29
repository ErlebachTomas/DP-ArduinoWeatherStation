#include <SoftwareSerial.h>

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

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);  // LED
  bluetooth.begin(9600); // Bluetooth init 
  Serial.begin(9600);
  sensorDHT.begin();

  display.setFont(u8g_font_unifont);   
  
  if (!bmp.begin(BMP280_ADRESS)) {   // osetreni sbernice 
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}

void loop() {

  float humidity = sensorDHT.readHumidity();
  float tepBMP= bmp.readTemperature();


    display.firstPage();
  do {
   display.setPrintPos(0, 10);
   display.print("Vlhkost: ");
   display.print(humidity);
   display.print(" %");
   display.setPrintPos(0, 30);
   display.print("Vzduch: ");
   display.print(tepBMP);
   display.print(" C");   

   } while (display.nextPage());
  

  if (bluetooth.available() > 0) {    
    byte receivedData = bluetooth.read();  
    

     switch (receivedData) { 
      case '0': 
      digitalWrite(LED_BUILTIN, LOW);
      break; 
      case '1':     
      digitalWrite(LED_BUILTIN, HIGH);
      break;
      default:     
      char stringData = (char) receivedData; 
      Serial.println(stringData); // print
     }
    
    bluetooth.write(receivedData);  // echo  
    
  }
   delay(1000); 
}
