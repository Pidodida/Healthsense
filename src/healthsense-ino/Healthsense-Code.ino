#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Wire.h>
#include "MAX30105.h"
#include <Firebase_ESP_Client.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "tests"
#define WIFI_PASSWORD "12345test"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "iotdb-24e1a-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "AIzaSyBXdVIjqyhHqSb3Nt3pQNSZGqBQKQ5D7sk"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;


//======================================== Millis variable to send/store data to firebase database.
unsigned long sendDataPrevMillis = 0;
const long sendDataIntervalMillis = 10000; //--> Sends/stores data to firebase database every 10 seconds.
//======================================== 

// Boolean variable for sign in status.
bool signupOK = false;

float store_random_Float_Val;
int store_random_Int_Val;
//
//int const PULSE_SENSOR_PIN = 34; // 'S' Signal pin connected to A0

int Signal;                // Store incoming ADC data. Value can range from 0-1024
int Threshold = 2000;       // Determine which Signal to "count as a beat" and which to ignore.
int val;
int tempPin = A0;
PulseOximeter pox;

MAX30105 particleSensor;

#define debug Serial

void setup() {
  Serial.begin(115200);

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;
    
    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);

    // Or use legacy authenticate method
    // Firebase.begin(DATABASE_URL, DATABASE_SECRET);
}


void loop() {
  
//  Signal = analogRead(PULSE_SENSOR_PIN);
//  int pulse = Signal / 30;
  val = analogRead(tempPin);
  float mv = (val / 1024.0)*500;
  float cel = mv / 10;
  Serial.println(cel);
   pox.update();
 
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
  Serial.print("Heart BPM:");
  Serial.print(pox.getHeartRate());
  Serial.print("-----");
  Serial.print("Oxygen Percent:");
  Serial.print(pox.getSpO2());
  Serial.println("\n");
    tsLastReport = millis();
    }
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setInt(&fbdo, "Database/pulserate", pulse)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "/Database/temperature", cel)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setInt(&fbdo, "/Database/temperature", cel)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
//  if Signal
//  Serial.print("Pulse Rate = ");
//  Serial.println(pulse + " BPM");                // Send the signal value to serial plotter
//
//
//


}
