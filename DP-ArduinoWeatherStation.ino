#include <SoftwareSerial.h>
#include <SoftwareSerial.h>

// Bluetooth
#define RX 11
#define TX 10
SoftwareSerial bluetooth(TX, RX);

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);  // LED
  bluetooth.begin(9600); // Bluetooth init 
  Serial.begin(9600);

}

void loop() {
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