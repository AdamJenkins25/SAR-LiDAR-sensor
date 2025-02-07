#define USER_SERIAL           Serial
#define USER_SERIAL_BAUDRATE  115200

#define LW20_SERIAL           Serial1
#define LW20_BAUDRATE         115200

char responseData[40];
int responseDataSize = 0;

void setup() 
{
  USER_SERIAL.begin(115200);
}

void lw20Connect() {
  LW20_SERIAL.begin(LW20_BAUDRATE);

  // Send some characters to make sure the LW20 switches to serial mode.
  LW20_SERIAL.print('www\r\n');
}
void loop() {
  USER_SERIAL.println("LW20 sample code started");
  
  // Connect to the LW20.
  lw20Connect();

  // Show product information to show a valid connection is established.
  lw20PrintProductInfo();

  // Run a loop that asks for distance every 25ms.
  while (true) {
    float distance = lw20GetDistance();
    float strength = lw20GetStrength();
    
    USER_SERIAL.print("Distance: ");
    USER_SERIAL.print(distance);
    USER_SERIAL.print(" m Strength: ");
    USER_SERIAL.print(strength);
    USER_SERIAL.println(" %");
    
    delay(25);
  }
}

}
float lw20GetDistance() {
  // NOTE: LDL = median distance to last return.
  if (lw20SendCommand("?ldl\r\n"))
    return getNumberFromResponse(responseData);

  return 0.0f;
}
