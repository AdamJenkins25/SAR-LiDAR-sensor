//-------------------------------------------------------------------------------------------
// LightWare LW20/SF20 Arduino LWNX serial connection sample.
// https://lightware.co.za
//-------------------------------------------------------------------------------------------
// This sample connects to the LW20/SF20 with the advanced binary protocol.
// The Arduino Uno only has a single hardware serial port, and therefore this sample
// has been modified to use a software serial port that can be used on custom
// defined pins.
//-------------------------------------------------------------------------------------------

#include <SoftwareSerial.h>
SoftwareSerial LW20_SERIAL(0, 1); // RX, TX

#define USER_SERIAL           Serial
#define USER_SERIAL_BAUDRATE  115200
#define LW20_BAUDRATE         38400
#define MOTOR_PIN             9       // Motor control pin
#define TRIGGER_DISTANCE      1.0     // Distance threshold in meters

// LW20 response string data.
char responseData[40];
int responseDataSize = 0;
unsigned long lastReadTime = 0;
const unsigned long READ_INTERVAL = 25; // Read every 25ms

//----------------------------------------------------------------------------------------------
// Application setup.
//----------------------------------------------------------------------------------------------
void setup() {
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW); // Ensure motor is off initially

  USER_SERIAL.begin(USER_SERIAL_BAUDRATE); // Connect to the serial console
  LW20_SERIAL.begin(LW20_BAUDRATE);

  USER_SERIAL.println("LW20 sample sode started");
  lw20Connect();
  lw20PrintProductInfo();
}

//----------------------------------------------------------------------------------------------
// Application main loop.
//----------------------------------------------------------------------------------------------
void loop() {
  if(millis() - lastReadTime >= READ_INTERVAL) {
    lastReadTime = millis();
    float distance = lw20GetDistance();
    float strength = lw20GetStrength();

    if (distance >= 0) {
      USER_SERIAL.print("Distance: ");
      USER_SERIAL.print(distance);
      USER_SERIAL.print(" m Strength: ");
      USER_SERIAL.print(strength);
      USER_SERIAL.println(" %");

      // Motor activation logic
      if (disatnce < TRIGGER_DISTANCE) {
        digitalWrite(MOTOR_PIN, HIGH); // Activate motor
      } else {
        digitalWrite(MOTOR_PIN, LOW); // Deactivate motor
      }
    } else {
      USER_SERIAL.println("Error reading distance");
    }
  }
}

//----------------------------------------------------------------------------------------------
// Conenct to the LW20 and enable serial mode.
//----------------------------------------------------------------------------------------------
void lw20Connect() {
  LW20_SERIAL.print('www\r\n'); // Send some characters to make sure the LW20 switches to serial mode
  delay(500); // Allow time for mode switch
}

//----------------------------------------------------------------------------------------------
// Send a command string and wait for a response. Returns true if a response was received.
// The response string will be held in 'responseData'.
//----------------------------------------------------------------------------------------------
bool lw20SendCommand(const char* CommandStr) {
  // Make sure there is no unwanted data left in the buffer.
  while (LW20_SERIAL.available()) {
    LW20_SERIAL.read();
  }

  LW20_SERIAL.print(CommandStr); // Send the command.

  unsigned long timeoutTime = millis() + 1000; // Only have 1 second timeout.
  responseDataSize = 0;
  
  // Wait for the full response.
  while (millis() < timeoutTime) {
    if (LW20_SERIAL.available()) {
      int c = LW20_SERIAL.read();
      USER_SERIAL.write(c); //Debug: print raw response to Serial Monitor
      
      if (c == '\n') {
        responseData[responseDataSize] = 0;
        USER_SERIAL.print("Received: ");
        USER_SERIAL.println(responseData);
        return true;
      }
      else if (c != '\r') {
        if (responseDataSize == sizeof(responseData) - 1) {
          responseDataSize = 0;
        }
        
        responseData[responseDataSize++] = (char)c;
      }
    }
  }
  USER_SERIAL.println("Command Timeout!");
  return false;
}

//----------------------------------------------------------------------------------------------
// Gets a number from the response string.
// Most commands have a single number that needs to be extracted from the response. That number
// occurs after the ':' character.
//----------------------------------------------------------------------------------------------
float getNumberFromResponse(char* ResponseStr) {
  // Find the ':' character.
  int index = 0;
  
  while (ResponseStr[index] != 0) {    
    if (ResponseStr[index] == ':') {
      return atof(ResponseStr + index + 1);
    }
    ++index;
  }
  return -1.0f; // Indicate failure
}

//----------------------------------------------------------------------------------------------
// Display the product info response.
//----------------------------------------------------------------------------------------------
void lw20PrintProductInfo() {  
  if (lw20SendCommand("?\r\n")) {
    USER_SERIAL.print("Got product response: ");
    USER_SERIAL.println(responseData);
  } else {
    USER_SERIAL.println("Could not get product response");
  }
}

//----------------------------------------------------------------------------------------------
// Get the current distance.
//----------------------------------------------------------------------------------------------
float lw20GetDistance() {
  // NOTE: LDL = median distance to last return.
  if (lw20SendCommand("?ldl\r\n"))
    return getNumberFromResponse(responseData);

  return 0.0f;
}

//----------------------------------------------------------------------------------------------
// Get the signal strength.
//----------------------------------------------------------------------------------------------
float lw20GetStrength() {
  if (lw20SendCommand("?lh\r\n"))
    return getNumberFromResponse(responseData);

  return 0.0f;
}
