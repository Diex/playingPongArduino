//#include <Servo.h>
#include <Adafruit_TiCoServo.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define BOB

Adafruit_TiCoServo servo;

#ifdef BOB
#define servoPin 9                // Connect yellow servo wire to digital I/O pin 4
#define ledPin 7
#else
#define servoPin 10                // Connect yellow servo wire to digital I/O pin 4
#define ledPin 6
#endif

int leds[16];
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, ledPin, NEO_GRB + NEO_KHZ800);

boolean newData = false;

void setup() {
  servo.attach(servoPin);      // Attach the servo to the PWM pin

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(115200);
}

int angle;
int pangle;

void loop() {
  recvWithEndMarker();
  showNewData();
  if (angle != pangle) ledsUpdate();
  delay(15);                     // Wait for the servo to get there
}

void ledsUpdate() {

  strip.clear();
  int pos = (int) constrain(map(angle, 180, 0, 0, 16), 0, 16);
  strip.setPixelColor(pos, 0, 255, 0);
  strip.show();

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

    #ifdef BOB
      if (receivedChars[0] == 'B') {
        pangle = angle;
        angle = atoi(next);
        servo.write(angle);           // Set the servo position
      }
    #endif
  
    #ifdef ALICE
      if (receivedChars[0] == 'A') {
        pangle = angle;
        angle = atoi(next);
        servo.write(angle);           // Set the servo position
      }
    #endif

    
      Serial.print("message for: ");
      Serial.print(receivedChars[0]);
      Serial.print('\t');
      Serial.println(atoi(next));
    }
    newData = false;
  }
}
