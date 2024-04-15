#include <SPI.h>
#include <WiFiNINA.h>
#include "ThingSpeak.h"

#include "Secret.h"

//Secret has the following
// #define SECRET_SSID "WIFI NAME"
// #define SECRET_PASS "WIFI PASSWORD"

// #define SECRET_CH_ID 0000000                      // replace 0000000 with your channel number
// #define SECRET_WRITE_APIKEY "XYZXYZXYZXYZXYZX"    // replace XYZ with your channel write API Key


// Please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // Your network SSID (name)
char pass[] = SECRET_PASS;        // Your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;      // The WiFi radio's status
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
String myString;
int MotionVal;



#define PIR_PIN 13

void setup() {
  Serial.begin(9600);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection:
    
    delay(10000);
  }
  Serial.println("Connected to WiFi");
  Serial.println("PIR Motion Sensor Test");

  // Set the PIR pin as input
  pinMode(PIR_PIN, INPUT);
}

void loop() {
  // Check WiFi connection
  // (existing code to check and reconnect if necessary)

  int motionState = digitalRead(PIR_PIN);
  
  // Check if motion is detected
  if (motionState == HIGH) {
    myString = "Motion";
    MotionVal = 1;
    Serial.println("Motion detected!");
  } else {
    myString = "No Motion";
    MotionVal = 0;
    Serial.println("No motion detected.");
  }

  // Update ThingSpeak fields with the actual values
  ThingSpeak.setField(1, myString);
  ThingSpeak.setField(2, MotionVal);


  // Write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Delay before the next read
  delay(20000); // Delay adjusted to 20 seconds to avoid hitting ThingSpeak's rate limit
}

