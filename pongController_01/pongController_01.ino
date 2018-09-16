#include <Servo.h>

// Read data from the serial port and set the position of a servomotor
// according to the value

//Servo alice;                   // Create servo object to control a servo
//int aliceServoPin = 10;                // Connect yellow servo wire to digital I/O pin 4

Servo bob;                   // Create servo object to control a servo
int bobServoPin = 9;                // Connect yellow servo wire to digital I/O pin 4

int leds[16];
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data

boolean newData = false;

void setup() {
//  alice.attach(aliceServoPin);      // Attach the servo to the PWM pin
  bob.attach(bobServoPin);      // Attach the servo to the PWM pin
  Serial.begin(115200);
}

int angle;

void loop() {
  recvWithEndMarker();
  showNewData();
//  Serial.println(alice.read());
  delay(15);                     // Wait for the servo to get there
}


void recvWithEndMarker() {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    }
    else {
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
    char * next = strtok(receivedChars, ":");
    if (next != NULL) {
      next = strtok(NULL, ":");
      
//      if (receivedChars[0] == 'A') {        
//        angle = atoi(next);                 
//        alice.write(angle);           // Set the servo position
//
//      }
      
      if (receivedChars[0] == 'B') {
        angle = atoi(next);
        bob.write(angle);           // Set the servo position
      }

      Serial.print("message for: ");
      Serial.print(receivedChars[0]);
      Serial.print('\t');
      Serial.println(atoi(next));
    }
    newData = false;
  }
}
