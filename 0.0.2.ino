//////////////////////////////////
///  Noah Polzer               ///
///  pono1012@hs-karlsruhe.de  ///
///  kleine Wetterstation      ///
///  26.06.2020                ///
///                            ///
///                            ///
//////////////////////////////////


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 

#define OLED_RESET     4 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude, lux;

int lightPin = 32;
int light = 0;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000);
  display.clearDisplay();
  
   bme.begin(0x76); 
   
}

void loop() {
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  
  light = analogRead(lightPin);
  
  lightolux();
  
  output();
}

void output() {
  display.clearDisplay();

  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(10, 0); 
  display.println(F("Mini-Wetterstation"));

  display.setCursor(10, 16); 
  display.print("Temp:");
  display.setCursor(70, 16);
  display.println(temperature);
  
  display.setCursor(10, 26); 
  display.println("Lftfchtg:");
  display.setCursor(70, 26);
  display.println(humidity);
  
  display.setCursor(10, 36); 
  display.println("Lux:");
  display.setCursor(70, 36);
  display.println(lux);
  
  display.setCursor(10, 46); 
  display.println("Hoehe:");
  display.setCursor(70, 46);
  display.println(altitude);

  
  display.display();
  delay(2000);
}

void lightolux()  //Näherungsweise
{
  float xplus=47.6190476;
  float voltage = light * (3.3 / 1023.0);
  float ma = ( voltage / 250 ) * 100000;
 
  lux = (0.21) * (ma-xplus) + 10;
  
  if(ma <= 10)
  {
    lux = 1/10 * ma;
  } 
  
}
