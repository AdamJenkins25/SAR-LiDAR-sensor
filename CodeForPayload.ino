#include <LW20.h>
#include <lw20api.h>
LW20 lw20(Serial1, 115200);               // Instantiate the LW20 using the hardware Serial Port1 at 115200 baud
const int Motoroutput = 9;  
const int MotorReverse = 10;
void setup() 
{
  pinMode(Motoroutput, OUTPUT);
  pinMode(MotorReverse, OUTPUT);
  // Start serial monitor port.
  Serial.begin(115200);                 // Prepare the USB Terminal to use as a monitor

  // Setup LW20.
  lw20.init();                          // Get the LW20 ready for commands
  lw20.setLaserParams(LWMS_48);
}

void loop() 
{
  // Get the first pulse distance with no filter.
  float distance = 1;
  float temperature = 1;

  Serial.print("Distance: "); 
  Serial.print(distance);
  if (distance = 1 ){
    digitalWrite(Motoroutput , HIGH);
    delay (25);
    digitalWrite(Motoroutput, LOW);
    digitalWrite(MotorReverse, HIGH);
    delay (25);
    digitalWrite(MotorReverse, LOW);
  }
  delay(25);

}
