/*
 * HTTP Client POST Request
 * Copyright (c) 2018, circuits4you.com
 * All rights reserved.
 * https://circuits4you.com 
 * Connects to WiFi HotSpot. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
/* Set these to your desired credentials. */
const char *ssid = "MEDIDERMAN";  //ENTER YOUR WIFI SETTINGS
const char *password = "medi102030";

//Web/Server address to read/write from 
const char *host = "http://www.abdulsamettutus.com";   //https://circuits4you.com website or IP address of server

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
  HTTPClient http;    //Declare object of class HTTPClient

  String ADCData, station, postData;
  int adcvalue=analogRead(A0);  //Read Analog value of LDR
  adcvalue=5;
  ADCData = String(adcvalue);   //String to interger conversion
  station = "A";

  //Post Data
  postData = "cihaz_veri=" + ADCData + "&station=" + station + "&cihaz_idi=2" + "&aaasss";
  
  http.begin("http://www.abdulsamettutus.com/kontrol/nedmin/netting/islem.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload
   Serial.println(ADCData);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload



 /////////////////////////////////////////////////////////////////////////////////////////////////

  if(httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
  
      const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(capacity);
  
   // Parse JSON object
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
  
    // Decode JSON/Extract values
    Serial.println(F("Response:"));

    Serial.println(root["data"][0].as<char*>());
    Serial.println(root["data"][1].as<char*>());
  }
  else
  {
    Serial.println("Error in response");
  }



/////////////////////////////////////////////////////////////////////////////////////







  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}
//=======================================================================
