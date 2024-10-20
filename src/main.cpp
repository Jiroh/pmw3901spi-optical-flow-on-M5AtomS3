#include <M5Unified.h>
#include <Wire.h>
#include <SPI.h>
#include <Bitcraze_PMW3901.h>
#include <Arduino.h> // Include Arduino GPIO library
#include <math.h>

// Using digital pin 5 for chip select
Bitcraze_PMW3901 flow(5);

// Constants
float h = 10.00;     // Height of sensor from the ground (adjust accordingly)
float fov = 42.0;   // Field of view of the sensor in radians (adjust accordingly)
int res = 30.0;     // Resolution of the sensor in pixels (adjust accordingly)
float scaler = 5.0;// Scaling factor (adjust accordingly)
float dt = 0.01;    // Time period of loop (adjust accordingly)

// Variables
float abs_dist_x = 0;
float abs_dist_y = 0;

// Function to compute conversion factor
float conversion_factor = h * 2 * tan(fov / 2) / (res * scaler);

void setup()
{
  M5.begin();
  M5.Power.begin();
  //SPI.begin(sck, miso, mosi, ss);
  SPI.begin(6, 7, 8, 5);

  //DW(SS, LOW); // 
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);  // 2Mhz

   M5.Lcd.setTextSize(2);
   M5.Lcd.println("pmw3901");
 
  if (!flow.begin())
  {
    M5.Lcd.println("fail init");
    while (1); { }
  }
  flow.begin();
  delay(100);//[msec]
}
int16_t deltaX,deltaY;
 
void loop()
{
  
  // sensor data (replace with actual data acquisition)
  flow.readMotionCount(&deltaX, &deltaY);
  
  // Convert data for PMW3901
  float dist_x, dist_y;
  dist_x = conversion_factor * deltaX;
  dist_y = conversion_factor * deltaY;
  
  // Update cumulative distances
  abs_dist_x += dist_x;
  abs_dist_y += dist_y;

  // Calculate velocity
  float vel_x = dist_x / dt;
  float vel_y = dist_y / dt;

  // Output results
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(GREEN, BLACK); 
  M5.Lcd.setCursor(5, 10);  
  M5.Lcd.print("Dist X: ");
  M5.Lcd.println(dist_x);
  M5.Lcd.setCursor(5, 40);
  M5.Lcd.print("Dist Y: ");
  M5.Lcd.println(dist_y);
  M5.Lcd.setCursor(5, 70);
  M5.Lcd.print("Velo X: ");
  M5.Lcd.println(vel_x);
  M5.Lcd.setCursor(5, 100);
  M5.Lcd.print("Velo Y: ");
  M5.Lcd.println(vel_y);

  delay(50);//[msec]
  M5.Lcd.fillScreen(0);       // clear display
}