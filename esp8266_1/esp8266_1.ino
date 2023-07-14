//#include <ESP8266WiFi.h>
//#include "FirebaseESP8266.h"
//#include <ArduinoJson.h>
//#include <string.h>
//
//#define FIREBASE_HOST "fir-firebase-4a9b5-default-rtdb.asia-southeast1.firebasedatabase.app"              // the project name address from firebase id
//
//#define FIREBASE_AUTH "JgSwcozm07mDj5hl9yVwextZfT11Lxb2KdLMrKWr"       // the secret key generated from firebase
//#define WIFI_SSID "HayTuLapDi"
//#define WIFI_PASSWORD "haytulapdi"
//
//FirebaseData firebaseData;
//String path = "/";
//FirebaseJson json;
//
////String fireStatus = "";                                                     // led status received from firebase
//
//int slotNumber = 2;
//String slot1Status;
//String slot2Status;
//String lastSlot1Status = "1";
//String lastSlot2Status = "1";
//String massage = "";
//char getID;
//void setup()
//{
//  Serial.begin(9600);
//  //  delay(1000);
//  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
//  Serial.print("Connecting to ");
//  Serial.print(WIFI_SSID);
//  while (WiFi.status() != WL_CONNECTED)
//  {
//    Serial.print(".");
//    delay(500);
//  }
//  Serial.println();
//  Serial.print("Connected to ");
//  Serial.println(WIFI_SSID);
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
//  Firebase.reconnectWiFi(true);
//  if (!Firebase.beginStream(firebaseData, path)) {
//    Serial.println("Reason: " + firebaseData.errorReason());
//    Serial.println();
//  }
//}
//
//unsigned long lastTime = 0;
//void loop()
//{
//  if (Serial.available()) {
//    massage = String(Serial.readStringUntil('*'));
//    getID = massage.charAt(0);
//    switch (getID) {
//      case '1':
//        massage.remove(0, 2);
//        slot1Status = massage;
//        if (!slot1Status.equals(lastSlot1Status)) {
//          if (slot1Status.equals("1")) {
//            slotNumber++;
//          } else {
//            slotNumber--;
//          }
//          lastSlot1Status = slot1Status;
//        }
//        break;
//      case '2':
//         massage.remove(0, 2);
//         slot2Status = massage;
//        if (!slot2Status.equals(lastSlot2Status)) {
//          if (slot2Status.equals("1")) {
//            slotNumber++;
//          } else {
//            slotNumber--;
//          }
//          lastSlot2Status = slot2Status;
//        }
//        break;
//    }
//  }
//  if (millis() - lastTime > 1000) {
//    Serial.print("Slot 1 status: ");
//    Serial.println(slot1Status);
//    Serial.print("Slot 2 status: ");
//    Serial.println(slot2Status);
//    Serial.print("Number of slot: ");
//    Serial.println(slotNumber);
//    
//    Firebase.setInt(firebaseData, path + "/Slot 1 status of parking 2", slot1Status.toInt());
//    Firebase.setInt(firebaseData, path + "/Slot 2 status of parking 2", slot2Status.toInt());
//    Firebase.setInt(firebaseData, path + "/Slot car left of parking 2", slotNumber);
//    lastTime = millis();
//  }
//}

#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <ArduinoJson.h>


#define FIREBASE_HOST "fir-firebase-4a9b5-default-rtdb.asia-southeast1.firebasedatabase.app"              // the project name address from firebase id

#define FIREBASE_AUTH "JgSwcozm07mDj5hl9yVwextZfT11Lxb2KdLMrKWr"       // the secret key generated from firebase
#define WIFI_SSID "Truong_thanh_len"
#define WIFI_PASSWORD "muoinghin"

FirebaseData firebaseData;
String path = "/";
FirebaseJson json;

String fireStatus = "";                                                     // led status received from firebase

int slotNumber = 2;
const short iRsensor = 4;
const short iRsensor2 = 0;
short slot1Status;
short lastSlot1Status = 1;
short slot2Status;
short lastSlot2Status = 1;
void setup()
{
  Serial.begin(9600);
  pinMode(iRsensor, INPUT);
  pinMode(iRsensor2, INPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
  Firebase.reconnectWiFi(true);
  if (!Firebase.beginStream(firebaseData, path)) {
    Serial.println("Reason: " + firebaseData.errorReason());
    Serial.println();
  }
}

unsigned long lastTime = 0;
void loop()
{
  slot1Status = digitalRead(iRsensor);
  slot2Status = digitalRead(iRsensor2);
  if (slot1Status != lastSlot1Status) {
    if (slot1Status == 0) {
      slotNumber--;
    } else {
      slotNumber++;
    }
    lastSlot1Status = slot1Status;
  }


  if (slot2Status != lastSlot2Status) {
    if (slot2Status == 0) {
      slotNumber--;
    } else {
      slotNumber++;
    }
    lastSlot2Status = slot2Status;
  }

  if (millis() - lastTime > 1000) {
    Serial.print("Slot 1 status: ");
    Serial.println(slot1Status);
    Serial.print("Slot 2 status: ");
    Serial.println(slot2Status);
    Serial.print("Number of slot: ");
    Serial.println(slotNumber);
    Firebase.setInt(firebaseData, path + "/Slot 1 status of parking 2", slot1Status);
    Firebase.setInt(firebaseData, path + "/Slot 2 status of parking 2", slot2Status);
    Firebase.setInt(firebaseData, path + "/Slot car left of parking 2", slotNumber);
    lastTime = millis();
  }
    fireStatus = Firebase.getInt("LED_STATUS");                                      // get ld status input from firebase
    if (fireStatus == "ON")
    {                                                          // compare the input of led status received from firebase
      Serial.println("Led Turned ON");
      digitalWrite(led, HIGH);                                                         // make external led ON
    }
    else if (fireStatus == "OFF")
    {                                                  // compare the input of led status received from firebase
      Serial.println("Led Turned OFF");
      digitalWrite(led, LOW);                                                         // make external led OFF
    }
    else
    {
      Serial.println("Command Error! Please send ON/OFF");
    }
}