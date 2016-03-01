//Seawolf Pneumatics

#define DEBUG //uncomment this line while testing w/ Arduino serial monitor

//byte code definitions
const byte OPEN_GRABBER   = 1;
const byte CLOSE_GRABBER  = 2;
const byte DROPPER_1      = 3;
const byte DROPPER_2      = 4;
const byte TORPEDO_1      = 5;
const byte TORPEDO_2      = 6;
const byte EXTRA          = 7; //reserved for future use for something else

//callback function declarations
void openGrabber();
void closeGrabber();
void dropper1();
void dropper2();
void torpedo1();
void torpedo2();

//pins
const int dropperPin1=A2;
const int dropperPin2=A3;
const int torpedoPin1=A4;
const int torpedoPin2=A5;
const int grabberPin1=A6;
const int grabberPin2=A7;

void firePin(int pin, unsigned long ms)
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW); 
}

int handshakeSerial(){
    int incomingByte;
    int i = 0;
    char incomingString[64];

    /* Wait for established message */
    while(1){
        incomingByte = Serial.read();
        if(incomingByte == -1 || (incomingByte != '{' && i == 0)) {
            Serial.println("{ID|Pneumatics}");
            delay(250);
            continue;
        }

        incomingString[i] = incomingByte;
        incomingString[++i] = '\0';
        if(incomingByte == '}') {
            break;
        }
    }

    Serial.println(incomingString);

    if(strcmp(incomingString, "{ESTABLISHED|NULL}") != 0) {
        Serial.println("Invalid message from client!");
        return 0;
     }

    /* Wait for READY message */
    while(1) {
        incomingByte = Serial.read();
        if(incomingByte == -1) {
            delay(100);
        } else if(incomingByte == '}') {
            break;
        }
    }

    return 1;
}

void setup() 
{
  Serial.begin(9600);
  #ifndef DEBUG
    handshakeSerial();
  #endif

  pinMode(dropperPin1,OUTPUT);
  pinMode(dropperPin2,OUTPUT);
  pinMode(torpedoPin1,OUTPUT);
  pinMode(torpedoPin2,OUTPUT);
  pinMode(grabberPin1,OUTPUT);
  pinMode(grabberPin2,OUTPUT);
}

void loop() 
{
  if(Serial.available()>0)
  {
    byte code;

    //only use this method when manually debugging with arduino serial monitor
    #ifdef DEBUG
      code=Serial.parseInt();
    #else
      code=Serial.read();
    #endif

    switch(code)
    {
      case OPEN_GRABBER:
        openGrabber();
        break;
      case CLOSE_GRABBER:
        closeGrabber();
        break;
      case DROPPER_1:
        dropper1();
        break;
      case DROPPER_2:
        dropper2();
        break;
      case TORPEDO_1:
        torpedo1();
        break;
      case TORPEDO_2:
        torpedo2();
        break;
      default:
        Serial.print("Unknown Code: ");
        Serial.println(code);
    }
  }
}

//function definitions

void openGrabber()
{
  #ifdef DEBUG
  Serial.println("Open Grabber");
  #endif
  
  firePin(grabberPin1, 1000);
}

void closeGrabber()
{
  #ifdef DEBUG
  Serial.println("Close Grabber");
  #endif
  
  firePin(grabberPin2, 1000);

}

void dropper1()
{
  #ifdef DEBUG
  Serial.println("Dropper 1");
  #endif
  firePin(dropperPin1, 1000);
}

void dropper2()
{
  #ifdef DEBUG
  Serial.println("Dropper 2");
  #endif
  firePin(dropperPin2, 1000);
}

void torpedo1()
{
  #ifdef DEBUG
  Serial.println("Torpedo 1");
  #endif
  firePin(torpedoPin1, 1000);
}

void torpedo2()
{
  #ifdef DEBUG
  Serial.println("Torpedo 2");
  #endif
  firePin(torpedoPin2, 1000);
}

