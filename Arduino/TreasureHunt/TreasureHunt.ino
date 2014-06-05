// Treasure Hunt!
// Display range and bearing to target coordinates
// Mike Grusin, SparkFun Electronics
//
// Hardware connections:
//
// Use GPS shield with switch set to DLINE,
// or connect GPS RX to pin 2, GPS TX to pin 3
//
// Connect serial LCD RX to pin 5
//
// Set TARGET_LAT and TARGET_LON to target coordinates
//
// LCD display:
//
// sat 5 rng 24m
// 138d SE -24d <
//
// sat 5:   Number of satellites in view
// rng 24m: Range to target coordinates
// 138d SE: Absolute bearing to target
// -24d <:  Relative bearing and direction to target
//          positive to right, negative to left
//          (Only valid when you are moving)

// Target coordinates

//double TARGET_LAT = 40.065156, TARGET_LON = -105.210142; // Center of SparkFun HQ
double TARGET_LAT = 40.064594, TARGET_LON = -105.209897;

#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;

// Set up two software serial ports
// one for GPS receiver, one for LCD

SoftwareSerial lcd_ss(4,5); // RX, TX
SoftwareSerial gps_ss(2,3); // RX, TX

void setup()
{
  // Initialize software serial ports:
  
  lcd_ss.begin(9600);
  gps_ss.begin(4800);
  
  // Display an opening message on LCD

  lcd_ss.listen(); // Connect to LCD port
  
  LCDclear();
  LCDline1();
  lcd_ss.print("Treasure Hunt!");
  LCDline2();
  lcd_ss.print("Waiting for GPS");
}

void loop()
{
  double range, abscourse;
  int relcourse;
  char data;
  
  gps_ss.listen(); // Connect to GPS port
  
  // Feed TinyGPS library characters from the GPS
  
  while (gps_ss.available())
  {
    data = gps_ss.read();
    gps.encode(data);
  }

  // If there is an updated location,
  // calculate and print new information

  if (gps.location.isUpdated())
  {
    // Get range and bearing to target:

    range = TinyGPSPlus::distanceBetween(gps.location.lat(),gps.location.lng(),TARGET_LAT,TARGET_LON);

    abscourse = TinyGPSPlus::courseTo(gps.location.lat(),gps.location.lng(),TARGET_LAT,TARGET_LON);

    // Compute relative course to target
    // (This is only valid when you're moving,
    // otherwise GPS receiver can't compute course)

    relcourse = (int)(gps.course.deg() - abscourse);
    if (relcourse < 0) relcourse += 360;
    if (relcourse > 180) relcourse -= 360;
    
    // Print out information

    lcd_ss.listen(); // Connect to LCD
    
    LCDclear();
    LCDline1();

    // Number of satellites:

    lcd_ss.print("sat ");
    lcd_ss.print(gps.satellites.value());

    // Range to target in meters:

    lcd_ss.print(" rng ");
    lcd_ss.print(range,0);
    lcd_ss.print("m");

    LCDline2();

    // Absolute course to target

    lcd_ss.print(abscourse,0);
    lcd_ss.print("d ");
    lcd_ss.print(TinyGPSPlus::cardinal(abscourse));
    lcd_ss.print("  ");

    // Relative course and direction to target

    lcd_ss.print(abs(relcourse));
    lcd_ss.print("d ");
    if (abs(relcourse) < 22)
      lcd_ss.print("^");
    else
      if (abs(relcourse) > 112)
        lcd_ss.print("v");
      else
        if (relcourse < 0)
          lcd_ss.print(">");
        else
          lcd_ss.print("<");
  }
}

void LCDclear()
// Clear the screen of a SparkFun serial-enabled LCD
{
  LCDline1();
  lcd_ss.print(F("                "));
  LCDline2();
  lcd_ss.print(F("                "));
}

void LCDline1()
// Move cursor to start of line 1 on a SparkFun serial-enabled LCD
{
  lcd_ss.write(254);
  lcd_ss.write(128);
}

void LCDline2()
// Move cursor to start of line 2 on a SparkFun serial-enabled LCD
{
  lcd_ss.write(254);
  lcd_ss.write(192);
}

