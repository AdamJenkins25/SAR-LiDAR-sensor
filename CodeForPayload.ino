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
void loop() 
{

 float lw20GetDistance() {
  // NOTE: LDL = median distance to last return.
  if (lw20SendCommand("?ldl\r\n"))
    return getNumberFromResponse(responseData);

  return 0.0f;
}

}
