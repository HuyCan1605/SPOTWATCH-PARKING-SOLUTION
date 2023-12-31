#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"
#include <ArduinoJson.h>


#define FIREBASE_HOST "fir-firebase-4a9b5-default-rtdb.asia-southeast1.firebasedatabase.app"              // the project name address from firebase id

#define FIREBASE_AUTH "JgSwcozm07mDj5hl9yVwextZfT11Lxb2KdLMrKWr"       // the secret key generated from firebase
#define WIFI_SSID "HayTuLapDi"
#define WIFI_PASSWORD "haytulapdi"

FirebaseData firebaseData;
String path = "/";
FirebaseJson json;

//String fireStatus = "";                                                     // led status received from firebase

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

String changeValue(int x) {
  if (x == 1) return "true";
  else return "false";
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
    Firebase.setString(firebaseData, path + "/Zone2_B1", changeValue(slot1Status));
    Firebase.setString(firebaseData, path + "/Zone2_B2", changeValue(slot2Status));
//    Firebase.setInt(firebaseData, path + "/Zone2_B3", slotNumber);
    lastTime = millis();
  }

}
