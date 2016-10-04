
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
/**
 * Moisture sensor pin.
 */
#define MOISTURE_SENSOR A0
/**
 * OLED reset PIN
 */
#define OLED_RESET 4
/**
 * 128x64 OLED screen.
 */
Adafruit_SSD1306 display(OLED_RESET);
/**
 * OLED display address
 */
#define DISPLAY_ADDRESS 0x3C
/**
 * Number of measurements averaged.
 */
#define NUMBER_OF_MEASUREMENTS 5
/**
 * Delay between soil measurements.
 */
#define MEASUREMENT_DELAY 100
/**
 * Very dry max value
 */
#define VERY_DRY_VALUE 100
/**
 * Dry max value
 */
#define DRY_VALUE 200
/**
 * Humid max value
 */
#define HUMID_VALUE 600
/**
 * Wet max value
 */
#define WET_VALUE 800
/**
 * Icon width.
 */
#define ICON_WIDTH 32 
/**
 * Icon height.
 */
#define ICON_HEIGHT  16 
/**
 * Icon X position.
 */
#define ICON_X_POS 0
/**
 * Icon Y position.
 */
#define ICON_Y_POS 10
/**
 * Drop bitmap very dry.
 */
static const unsigned char PROGMEM BMP_VERY_DRY[] =
{
  B00000000, B00000000, B10000000, B00000000, 
  B00000000, B00000001, B10000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000100, B00100000, B00000000, 
  B00000000, B00001000, B00010000, B00000000, 
  B00000000, B00010000, B00001000, B00000000, 
  B00000000, B00100000, B00000100, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B01000000, B00000010, B00000000,
  B00000000, B10000000, B00000001, B00000000, 
  B00000000, B10000000, B00000001, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B00100000, B00000100, B00000000, 
  B00000000, B00001111, B11110000, B00000000
};
/**
 * Drop bitmap  dry.
 */
static const unsigned char PROGMEM BMP_DRY[] =
{
  B00000000, B00000000, B10000000, B00000000, 
  B00000000, B00000001, B10000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000100, B00100000, B00000000, 
  B00000000, B00001000, B00010000, B00000000, 
  B00000000, B00010000, B00001000, B00000000, 
  B00000000, B00100000, B00000100, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B01000000, B00000010, B00000000,
  B00000000, B10000000, B00000001, B00000000, 
  B00000000, B10000000, B00000001, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B00001111, B11110000, B00000000
};
/**
 * Drop bitmap humid.
 */
static const unsigned char PROGMEM BMP_HUMID[] =
{
  B00000000, B00000000, B10000000, B00000000, 
  B00000000, B00000001, B10000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000100, B00100000, B00000000, 
  B00000000, B00001000, B00010000, B00000000, 
  B00000000, B00010000, B00001000, B00000000, 
  B00000000, B00100000, B00000100, B00000000, 
  B00000000, B01000000, B00000010, B00000000, 
  B00000000, B01111111, B11111110, B00000000,
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B00001111, B11110000, B00000000
};
/**
 * Drop bitmap wet.
 */
static const unsigned char PROGMEM BMP_WET[] =
{
  B00000000, B00000000, B10000000, B00000000, 
  B00000000, B00000001, B10000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000010, B01000000, B00000000, 
  B00000000, B00000100, B00100000, B00000000, 
  B00000000, B00001000, B00010000, B00000000, 
  B00000000, B00011111, B11111000, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B01111111, B11111110, B00000000,
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B00001111, B11110000, B00000000
};
/**
 * Drop bitmap very wet.
 */
static const unsigned char PROGMEM BMP_VERY_WET[] =
{
  B00000000, B00000000, B10000000, B00000000, 
  B00000000, B00000001, B10000000, B00000000, 
  B00000000, B00000011, B11000000, B00000000, 
  B00000000, B00000011, B11000000, B00000000, 
  B00000000, B00000111, B11100000, B00000000, 
  B00000000, B00001111, B11110000, B00000000, 
  B00000000, B00011111, B11111000, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B01111111, B11111110, B00000000,
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B11111111, B11111111, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B01111111, B11111110, B00000000, 
  B00000000, B00111111, B11111100, B00000000, 
  B00000000, B00001111, B11110000, B00000000
};
/**
 * Setup Serial and Display.
 */
void setup()   {                
  Serial.begin(9600);  
  display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS);   
}

/**
 * Loop. Each iteration updates the screen.
 */
void loop() {
  /**
   * Get analog moisture level.
   */
  float moistureLevel = getMoistureLevel();
  /**  
   *   Set display text.
   */
  setDisplayText(moistureLevel);
  /**
   * Set display icon.
   */
  setDisplayIcon(moistureLevel);
  /**
   * Refresh screen.
   */
  display.display();
  /**
   * Wait two seconds for next iteration.
   */
  delay(2000);
}

/**
 * Get displayed text based on moisture level.
 */
char* getMoistureText(float moistureLevel) {
  if (moistureLevel <= VERY_DRY_VALUE) {
    return "Very dry";  
  } else if (moistureLevel <= DRY_VALUE) {
    return "Dry";  
  } else if (moistureLevel <= HUMID_VALUE) {
    return "Moist";  
  } else if (moistureLevel <= WET_VALUE) {
    return "Wet";  
  }else {
    return "Very wet";
  }
}

/**
 * Set display icon based on moisture level.
 */
void setDisplayIcon(float moistureLevel) {
  if (moistureLevel <= VERY_DRY_VALUE) {
    display.drawBitmap(ICON_X_POS, ICON_Y_POS, BMP_VERY_DRY, ICON_WIDTH, ICON_HEIGHT, 1);
  } else if (moistureLevel <= DRY_VALUE) {
    display.drawBitmap(ICON_X_POS, ICON_Y_POS, BMP_DRY, ICON_WIDTH, ICON_HEIGHT, 1);
  } else if (moistureLevel <= HUMID_VALUE) {
    display.drawBitmap(ICON_X_POS, ICON_Y_POS, BMP_HUMID, ICON_WIDTH, ICON_HEIGHT, 1);
  } else if (moistureLevel <= WET_VALUE) {
    display.drawBitmap(ICON_X_POS, ICON_Y_POS, BMP_WET, ICON_WIDTH, ICON_HEIGHT, 1);
  }else {
    display.drawBitmap(ICON_X_POS, ICON_Y_POS, BMP_VERY_WET, ICON_WIDTH, ICON_HEIGHT, 1);
  }
}

/**
 * Set display text based on moisture level.
 */
void setDisplayText(float moistureLevel) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(25,0);
  display.print("Zamioculcas");  
  display.setTextSize(0);
  display.setTextColor(WHITE);
  display.setCursor(30,10);
  display.print("Soil: ");  
  display.println(getMoistureText(moistureLevel));    
}

/**
 * Get moisture level.
 */
float getMoistureLevel() {
   int val = 0;
  for (int i = 0;i < NUMBER_OF_MEASUREMENTS; i++) {
    val = analogRead(MOISTURE_SENSOR) + val;
    delay(MEASUREMENT_DELAY);
  }
  float valAvg = val / NUMBER_OF_MEASUREMENTS;
  return valAvg;
}

