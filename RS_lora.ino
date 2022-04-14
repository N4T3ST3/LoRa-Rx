#include <U8x8lib.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h>


// WIFI_LoRa_32 ports
// GPIO5  -- SX1278's SCK
// GPIO19 -- SX1278's MISO
// GPIO27 -- SX1278's MOSI
// GPIO18 -- SX1278's CS
// GPIO14 -- SX1278's RESET
// GPIO26 -- SX1278's IRQ(Interrupt Request)

#define SS      18
#define RST     14
#define DI0     26
#define BAND    915E6 // North America 

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

String LoRa_Data;

void setup() {

  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(9600);
  while (!Serial); 
  delay(1000);

  u8x8.begin();
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  Serial.println("LoRa Receiver");
  u8x8.drawString(0, 4, "LoRa Receiver");

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    u8x8.drawString(0, 1, "Starting LoRa failed!");
    while (1);
  }
}

void loop() {

  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("RX PACK '");
    
    // read packet
    while (LoRa.available()) {
      LoRa_Data = LoRa.readString();
      Serial.print(LoRa_Data);
    }

    // print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.println(rssi);

    // Display information
   u8x8.clearDisplay();
   u8x8.setCursor(0,0);
   u8x8.print("LORA RECEIVER");
   u8x8.setCursor(0,20);
   u8x8.print(LoRa_Data);
   u8x8.setCursor(60,40);
   u8x8.print(rssi);
   u8x8.display();   
  }

}
