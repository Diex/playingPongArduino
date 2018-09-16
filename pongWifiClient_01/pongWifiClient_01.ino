
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOSC.h>

WiFiUDP Udp;

ArduinoOSCWiFi osc;
const char* ssid = "suckmykiss";
const char* pass = "frutigran";
const int recv_port = 8888;


#include <Servo.h>

Servo myservo;  // create servo object to control a servo
                // twelve servo objects can be created on most boards


// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      16

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, D3, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second


void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);    // turn *on* led


    Serial.begin(115200);

    WiFi.disconnect(true);
    WiFi.begin(ssid, pass);


  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(recv_port);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());


    osc.begin(Udp, recv_port);
    osc.addCallback("/led", &callback);

     myservo.attach(D2);  //
      pixels.begin(); // This initializes the NeoPixel library.
}

void loop()
{
  for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).

  }

  
    osc.parse();
}

void callback(OSCMessage& m)
{
  int state = m.getArgAsInt32(0);
  Serial.println(m.getOSCAddress());
  Serial.println(state);

  digitalWrite(BUILTIN_LED, state);
  
}
