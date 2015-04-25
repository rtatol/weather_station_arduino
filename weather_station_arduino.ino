/*
  LCD display compatible with the Hitachi HD44780 driver + DHT11 temp/humi sensor
  
  HD44780 LCD PINS:
  
    VSS -> GND
    VDD -> 5V
    VD -> 10k ohm potentiometer
    RS -> PIN12
    RW -> GND
    E -> PIN11
    ...
    D4 -> PIN5
    D5 -> PIN4
    D6 -> PIN3
    D7 -> PIN2
    ...
    backlight
    A -> 5V > xxx
    K -> GND
  
  DHT11 PINS:
  
    1 -> 5V
    2 -> PIN7
    3 -> NC
    4 -> GND
*/


#include <LiquidCrystal.h>
#include <dht.h>

#define DHT11_PIN 7

const String MESSAGE_DHTLIB_ERROR_CHECKSUM = "Checksum error";
const String MESSAGE_DHTLIB_ERROR_TIMEOUT = "Time out error";
const String MESSAGE_UNKNOWN_DHTLIB_ERROR = "Unknown error";
unsigned const int DELAY_DEFAULT = 1000 * 120;
unsigned const int DELAY_ERROR = 1000 * 1;

LiquidCrystal LCD(12, 11, 5, 4, 3, 2);
dht DHT;

void setup() {
  LCD.begin(16, 2);
  LCD.print("### hello ###");
  delay(2000);
}

void loop() {
    if(readDht11Value()){
      delay(DELAY_DEFAULT);
    } else {
      delay(DELAY_ERROR);
    }
}

boolean readDht11Value() {
  
  int dht11Status = DHT.read11(DHT11_PIN);
  boolean isSuccess = false;
  
  switch (dht11Status) {
    case DHTLIB_OK:  
        lcdPrint();
        isSuccess = true;
        break;
    case DHTLIB_ERROR_CHECKSUM: 
        Serial.print(MESSAGE_DHTLIB_ERROR_CHECKSUM);
        lcdPrintError(MESSAGE_DHTLIB_ERROR_CHECKSUM);
        break;
    case DHTLIB_ERROR_TIMEOUT: 
        Serial.print(MESSAGE_DHTLIB_ERROR_TIMEOUT); 
        lcdPrintError(MESSAGE_DHTLIB_ERROR_TIMEOUT);
        break;
    default: 
        Serial.print(MESSAGE_UNKNOWN_DHTLIB_ERROR); 
        lcdPrintError(MESSAGE_UNKNOWN_DHTLIB_ERROR);
        break;
  }
  
  return isSuccess;
}

void lcdPrint(){
  LCD.clear();
  
  LCD.print("temp: ");
  LCD.print(DHT.temperature);
  LCD.print(" C");
  
  LCD.setCursor(0,1);
  LCD.print("humi: ");
  LCD.print(DHT.humidity);
  LCD.print(" %");  
}

void lcdPrintError(String message){
  LCD.clear();
  LCD.println(message);  
}
