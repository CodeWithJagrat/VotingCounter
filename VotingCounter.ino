#include <Arduino.h>
// #include <WiFi.h>               //we are using the ESP32
#include <ESP8266WiFi.h>      // uncomment this line if you are using esp8266 and comment the line above
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "ZTE-cz9JjE"
#define WIFI_PASSWORD "pgh3pt6r"

// Insert Firebase project API Key
#define API_KEY "AIzaSyBAppkpiENR7RyqfSVW5ZNLhHKwg8zGwfs"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-cam-python-default-rtdb.firebaseio.com/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;                     //since we are doing an anonymous sign in 

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){

  //temperature and humidity measured should be stored in variables so the user
  //can use it later in the database

  int N1 = 0;
  int N2 = 0;
  int N3 = 0;
  int N4 = 0;
  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    //since we want the data to be updated every second
    sendDataPrevMillis = millis();
    // Enter Temperature in to the DHT_11 Table
    if (Firebase.RTDB.setInt(&fbdo, "Vote/N1", N1)){
      // This command will be executed even if you dont serial print but we will make sure its working
      Serial.print("N1 : ");
      Serial.println(N1);
      Serial.print(" ");
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    
    // Enter Humidity in to the DHT_11 Table
    if (Firebase.RTDB.setInt(&fbdo, "Vote/N2", N2)){
      Serial.print("N2 : ");
      Serial.print(N2);
      Serial.print(" ");
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "Vote/N3", N3)){
      Serial.print("N3 : ");
      Serial.print(N3);
      Serial.print(" ");
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setInt(&fbdo, "Vote/N4", N4)){
      Serial.print("N4 : ");
      Serial.print(N4);
      Serial.print(" ");
    }
    else {
      Serial.println("Failed to Read from the Sensor");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  }
}
