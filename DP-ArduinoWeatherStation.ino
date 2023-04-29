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


void setup() {

  pinMode(LED_BUILTIN, OUTPUT);  // LED
  bluetooth.begin(9600); // Bluetooth init 
  Serial.begin(9600);

  sensorDHT.begin();
  display.setFont(u8g_font_unifont);   

}

void loop() {

  float humidity = sensorDHT.readHumidity();
  Serial.print("DHT: ");
  Serial.println(humidity);

  display.firstPage();
 do {
   display.setPrintPos(0, 10);
   display.print("DHT: ");
   display.print(humidity);
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
