#include <SPI.h>
#include "RF24.h"
#include <Servo.h> 

// --- DÜZELTME BURADA ---
// Senin bağlantıların: CE -> 9, CSN -> 10
RF24 radio(9, 10); 

Servo esc;        
const byte address[6] = "00001";

struct JoystickData {
  byte throttle_value; 
  byte y_aux_value;    
};

JoystickData data;
unsigned long lastRecvTime = 0;

void setup() {
  Serial.begin(9600);
  
  esc.attach(3, 700, 2000); // ESC Sinyal D3
  
  Serial.println("ESC Arming (Hazirlaniyor)...");
  esc.write(0); 
  delay(3000);  
  
  if (!radio.begin()) {
    Serial.println(F("Radyo HATASI! Baglantilari kontrol et."));
    while (1);
  }
  
  radio.setPALevel(RF24_PA_MIN); 
  radio.setDataRate(RF24_1MBPS); 
  radio.openReadingPipe(0, address);
  radio.startListening(); 
  
  Serial.println(F("Sistem Hazir. Veri Bekleniyor..."));
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(data)); 
    lastRecvTime = millis(); 
    
    int motor_power = map(data.throttle_value, 0, 255, 0, 180);
    esc.write(motor_power);
    
    // Debug
    Serial.print("Radyo Gelen: ");
    Serial.print(data.throttle_value);
    Serial.print(" | Aci: ");
    Serial.println(motor_power);
  }
  
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    esc.write(0); 
  }
}