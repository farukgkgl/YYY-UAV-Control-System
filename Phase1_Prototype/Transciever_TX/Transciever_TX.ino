#include <SPI.h>
#include "RF24.h"

// --- DONANIM AYARLARI ---
// nRF24L01 Bağlantıları: CE -> D7, CSN -> D8
RF24 radio(7, 8); 

const byte address[6] = "00001";

// Veri Paketi Yapısı
struct JoystickData {
  byte throttle_value; // Gaz
  byte y_aux_value;    // Yardımcı
};

// İşte 'data' değişkenini burada global olarak tanımlıyoruz:
JoystickData data;

void setup() {
  Serial.begin(9600);
  
  if (!radio.begin()) {
    Serial.println(F("Radyo baslatilamadi! Kablolari kontrol et."));
    while (1); 
  }
  
  // Adaptörsüz kullanım için güvenli ayarlar
  radio.setPALevel(RF24_PA_MIN); 
  radio.setDataRate(RF24_1MBPS); 
  
  radio.openWritingPipe(address);
  radio.stopListening();
  
  Serial.println(F("Sistem Hazir. Joystick Ortada=0, Ileri=Gaz."));
}

void loop() {
  // Joystick okumaları
  int raw_throttle = analogRead(A1); 
  int raw_aux = analogRead(A0);

  // --- YENİ GAZ MANTIĞI ---
  // Joystick Yönü Doğru (İleri itince sayı artıyor) varsayımıyla:
  // Merkez (512) ve Geri (<512) -> MOTOR DURUR (0)
  // İleri (>520) -> MOTOR HIZLANIR (0-255)

  if (raw_throttle < 520) {
    // Joystick ortada veya geriye çekili: Gazı Kes
    data.throttle_value = 0; 
  } 
  else {
    // Joystick ileri itiliyor:
    // 520 ile 1023 arasını -> 0 ile 255 arasına çevir
    data.throttle_value = map(raw_throttle, 520, 1023, 0, 255);
  }

  // Yardımcı eksen (olduğu gibi kalsın)
  data.y_aux_value = map(raw_aux, 0, 1023, 0, 255);

  // --- DEBUG EKRANI ---
  // Buradan kontrol et: Bırakınca "Giden Gaz: 0" yazmalı.
  Serial.print("Joystick Ham: ");
  Serial.print(raw_throttle);
  Serial.print(" | Giden Gaz: ");
  Serial.println(data.throttle_value);

  // Gönder
  radio.write(&data, sizeof(data));
  
  delay(20); 
}