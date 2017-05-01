/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/
#include <arduino.h>
#include <main.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include "secrets.h"

#define PIN D3

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);




// Update these with values suitable for your network.

#define BUILTIN_LED 4



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Serial.flush();
  setup_wifi();

  client.setServer(mqtt_server, 1883);
//  client.setServer(mqtt_server, 9001);
  client.setCallback(callback);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  colorWipe(strip.Color(255, 0, 0), 25); // Red
  colorWipe(strip.Color(0, 0, 0), 25); // Clear

  colorWipe(strip.Color(0, 255, 0), 25); // Green
  colorWipe(strip.Color(0, 0, 0), 25); // Clear

}

void setup_wifi() {

  WiFi.disconnect(true);
  delay(3000);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf("connecting %s:%s \r\n", ssid, password);
    delay(1000);
    // Serial.print(WiFi.status());
    // Serial.print(";");
    // Serial.println(WL_CONNECTED);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}




void callback(char* topic, byte* payload, unsigned int length) {
//  colorWipe(strip.Color(0, 0, 255), 25); // Blue
//  colorWipe(strip.Color(0, 0, 0), 25); // Clear

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();


  // Convert topic to string
  String topicStr(topic);

  if ((topicStr == "qliksense/connection/open") || (topicStr == "qliksense/session/start")) {
    // Spinning green ring
    colorWipe(strip.Color(0, 255, 0), 40); // Green
    colorWipe(strip.Color(0, 0, 0), 40); // Clear
  } else if ((topicStr == "qliksense/connection/close") || (topicStr == "qliksense/session/stop")) {
    // Spinning green ring
    colorWipe(strip.Color(255, 0, 0), 40); // Green
    colorWipe(strip.Color(0, 0, 0), 40); // Clear
  } else {

    // Convert payload to string
    char cmdString[length + 1];
    memcpy(cmdString, payload, length);
    cmdString[length] = 0;

    String cmd(cmdString);

    //client.publish("qliksense/ghostlamp/1/cmd-received", cmdString);

    // Switch on the LED if an 1 was received as first character
    //  if ((char)payload[0] == '1') {
    if (cmd == "1") {
      //digitalWrite(BUILTIN_LED, HIGH);   // Turn the LED on (Note that LOW is the voltage level

      colorWipe(strip.Color(255, 0, 0), 25); // Red
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "2") {
      colorWipe(strip.Color(0, 255, 0), 25); // Green
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "3") {
      theaterChase(strip.Color(127, 127, 127), 50); // White
      colorWipe(strip.Color(0, 0, 0), 25); // Clear
    } else if (cmd == "4") {
      theaterChase(strip.Color(127, 0, 0), 50);     // Red
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "5") {
      theaterChase(strip.Color(0, 127, 0), 50);     // Green
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "6") {
      theaterChase(strip.Color(0, 0, 127), 50);     // Blue
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "7") {
      rainbow(20);
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "8") {
      rainbowCycle(20);
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "9") {
      theaterChaseRainbow(50);
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "10") {
      theaterChase(strip.Color(0, 0, 127), 50);     // Blue
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "11") {
      theaterChase(strip.Color(0, 0, 127), 50);     // Blue
      colorWipe(strip.Color(0, 0, 0), 25); // Clear

    } else if (cmd == "12") {
      rainbow(20);

    } else {
      colorWipe(strip.Color(0, 0, 0), 25); // Clear
    }
  }


}

void reconnect() {
  // Loop until we're reconnected
//  digitalWrite(BUILTIN_LED, HIGH);
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("GhostLamp")) {
      Serial.println("connected");
      // Once connected, publish an announcement.
      // Use a topic of .../ghostlamp/n/... where n is the number of the current lamp.
      // There will probably never be more than one Pacma Ghost lamp in any given network, but still...
      client.publish("qliksense/ghostlamp/1/status", "connected");
      // ... and resubscribe
      client.subscribe("qliksense/connection/open");
      client.subscribe("qliksense/connection/close");
      client.subscribe("qliksense/session/start");
      client.subscribe("qliksense/session/stop");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
  }


}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
