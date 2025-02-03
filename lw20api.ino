#include <LW20.h> // includes library to control LiDAR sensor

LW20 lw20(Serial1, 115200); // initializes lw20 object (hardware serial port, baud rate)

void setup() 
{
  // Start serial monitor port.
  Serial.begin(115200); // begins at 115200 baud

  // Setup LW20.
  lw20.init();
  lw20.setLaserParams(LW20_MODE_388); // configures laser parameters to LW20_MODE_388 operating mode
  lw20.setServoParams(1000, 2000, 10); // (min servo pulse width (1ms), max servo pulse width (2ms), servo update interval (0.01ms))
  lw20.setScanParams(-45, 45, 8, 3.5); // (min scan angle (left limit), max scan angle (right limit), number of steps per scan, scan speed in degrees per step)
  lw20.setAlarmAParams(0.5, -45, -20); // triggers if an object is closer than 0.5m between -45 and -20 degrees
  lw20.setAlarmBParams(0.5, 20, 45); // triggers if an object is closer than 0.5m between 20 and 45 degrees
  lw20.startScan(); // starts the scanning process
}

void loop() 
{
  // Check alarm status.
  bool a, b;
  lw20.checkAlarmStatus(&a, &b); // check status and store results in a (Alarm A) and b (Alarm B)

  // print alarm statuses (0 = No alarm, 1 = Alarm triggered) to serial monitor
  Serial.print("Alarm A:");
  Serial.print(a);
  Serial.print(" Alarm B:");
  Serial.println(b);
  
  delay(100); // waits 100ms before checking again to avoid flooding serial output
}
