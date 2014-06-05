// PassThrough
// Pass data between soft and hard serial ports
// Mike Grusin, SparkFun Electronics
//
// Hardware connections:
//
// Use GPS shield with switch set to DLINE,
// or connect GPS RX to pin 2, GPS TX to pin 3
//
// Open serial monitor (or serial port) at 9600 baud

#include <SoftwareSerial.h>

// Set up software serial port for GPS receiver

SoftwareSerial gps_ss(2,3); // RX, TX

void setup()
{
  // Initialize serial ports:
  
  gps_ss.begin(4800);
  Serial.begin(9600);
}

void loop()
{
  char data;

  // From GPS to Serial

  while (gps_ss.available())
  {
    data = gps_ss.read();
    Serial.print(data);
  }

  // From Serial to GPS

  while (Serial.available())
  {
    data = Serial.read();
    gps_ss.print(data);
  }
}

