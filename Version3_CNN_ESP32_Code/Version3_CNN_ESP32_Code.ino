#include <iostream>
#include <string>

/**
 * Firebase Library Citation:
 * Created by K. Suwatchai (Mobizt)
 * Email: k_suwatchai@hotmail.com
 * Github: https://github.com/mobizt/Firebase-ESP32
 * Copyright (c) 2022 mobizt
**/

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

//#if defined(ESP32)
//#include <WiFi.h>
//#include <FirebaseESP32.h>
//#include "soc/soc.h"
//#include "soc/rtc_cntl_reg.h"
//#endif
//#elif defined(ESP8266)
//#include <ESP8266WiFi.h>
//#include <FirebaseESP8266.h>
//#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "KDNETWORK"
//#define WIFI_PASSWORD "jennyLovesAnjali!"
#define WIFI_PASSWORD "TesterNetwork1"

/* 2. Define the API Key */
#define API_KEY "AIzaSyBQ6FfwCksOZZ3U4O-FrI0b1yceY6C-8NY"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://fydp-general-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "karamdanial31@gmail.com"
#define USER_PASSWORD "Cosmicoder"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
const int power_pin_1 = 12;
const int power_pin_2 = 26;

int local_counter = 0;
int firebase_counter = 0;
bool light_on = false;

int counter = 0;
int force_sensor_data[60][3];
bool isChanged = 1;
String result = "";

void setup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

  pinMode(power_pin_1, OUTPUT);
  pinMode(power_pin_2, OUTPUT);
  digitalWrite(power_pin_1, HIGH);
  digitalWrite(power_pin_2, HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
}

void loop()
{
  int a = analogRead(34);
  int b = analogRead(35);
  int c = analogRead(32);
  Serial.print(a);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(", ");
  Serial.print(c);
  Serial.println(" ");
  result += String(a) + ", " + String(b) + ", " + String(c)+ ", ";

  if (counter == 60) {
    counter = 0;
    result = "";
//    for(int i = 0; i < 60; i++){
//      force_sensor_data[i][0] = 0;
//      force_sensor_data[i][1] = 0;
//      force_sensor_data[i][2] = 0;
//    } 
  }

//  force_sensor_data[counter][0] = a;
//  force_sensor_data[counter][1] = b;
//  force_sensor_data[counter][2] = c;

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 30000))
  {
//    for (int i = 0; i < 60; i++) {
//      for (int j = 0; j < 3; j++) {
//        result.concat(String(force_sensor_data[i][j]));
//        Serial.println(force_sensor_data[i][j]);
//        result.concat(", ");
//      }
//    }

    sendDataPrevMillis = millis();
    FirebaseJson json;
    Firebase.getInt(fbdo, F("/users/Karam/step_count"), &firebase_counter);
    firebase_counter = firebase_counter + local_counter;
    isChanged = 1;
    json.set("/Karam/step_count", firebase_counter);
    json.set("/Karam/force_sensor_data", result);
    json.set("/Karam/changed", isChanged);
    Firebase.updateNode(fbdo,"/users",json);
    local_counter = 0;
  }
  counter++;
  delay(600);
}
