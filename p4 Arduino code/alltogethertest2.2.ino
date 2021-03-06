#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // raw Analog in 0. updated every 2mS
volatile int Signal;                // raw data
volatile int IBI = 600;             // time intervals between beats
volatile boolean Pulse = false;    
volatile boolean QS = false;        // Heartbeat failsafe boolean
boolean knapTryk = false;
int pulsePin = 0;
int ledPin = 13;
int state = 0;
int flag = 0;
int buttonPin = 8;
int buttonState = 0;

#define BNO055_SAMPLERATE_DELAY_MS (100) // BNO Activation code.
Adafruit_BNO055 bno = Adafruit_BNO055(55); 
void displaySensorDetails(void){
  sensor_t sensor;
  bno.getSensor(&sensor);
  delay(500);
}

void setup() {
  Serial.begin(9600); // Baud-rate for Bluetooth connection.
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(buttonPin, INPUT);
  interruptSetup();
   if(!bno.begin())
  {
    // There was a problem detecting the BNO055 
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  delay(1000);
  displaySensorDetails();
}
void loop() {
  serialOutput() ;
  if (QS == true) {
    serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
    QS = false;                      // reset the Quantified Self flag for next time
  }
  sensors_event_t event;
  bno.getEvent(&event);
  Serial.print("A ");   //Print the heading
  Serial.print((int)event.orientation.x);
  Serial.print(" ");
  delay(BNO055_SAMPLERATE_DELAY_MS);

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && knapTryk == false) {
    digitalWrite(ledPin, HIGH);
   Serial.print("C "); //Print the Click
    knapTryk = true;
  } else if (buttonState == LOW) {
    digitalWrite(ledPin, LOW);
    knapTryk = false;
  }
}
