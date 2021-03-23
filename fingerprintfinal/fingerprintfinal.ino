#include <Adafruit_Fingerprint.h>


#include <SPI.h>
#include <Wire.h>
#include <WiFiClient.h> 
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>  
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//Fingerprint scanner Pins
#define Finger_Tx 14 //D5
#define Finger_Rx 12 //D6

//************************************************************************//************************************************************************
SoftwareSerial mySerial(Finger_Rx, Finger_Tx);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


//************************************************************************
/* WIFI settings*/
const char *ssid = "Orange-4714";  
const char *password = "Salah@2000";
//************************************************************************
String postData ;          // post array that will be send to the website
String link = "http://192.168.1.109:5555/index"; //computer IP or the server domain
int FingerID = 0;         // The Fingerprint ID from the scanner 
uint8_t id;


void setup() {

  Serial.begin(9600);
  Wire.begin(D2, D1);
  lcd.begin(16,2);   // iInit the LCD for 16 chars 2 lines
  lcd.home();
  lcd.print("Hello world");
   /* delay(2000); */
    lcd.clear();
  connectToWiFi();
  
  //---------------------------------------------
  
  // set the data rate for the sensor serial port
  finger.begin(57600);
  Serial.println("\n\nAdafruit finger detect test");

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  //---------------------------------------------
  
  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  
  //------------*test the connection*------------
  
  
}

void loop()                     // run over and over again
{    lcd.clear();
      
     getFingerprintID();
    // delay(250);//don't ned to run this at full speed.            
}
//************send the fingerprint ID to the website*************
void SendFingerprintID( int finger ){
  
  HTTPClient http;    //Declare object of class HTTPClient
  //Post Data
  postData = "{\"FingerID\":\""+String(finger)+"\"}"; // Add the Fingerprint ID to the Post array in order to send it
  // Post methode
   
  http.begin(link); //initiate HTTP request, put your Website URL or Your Computer IP 
  http.addHeader("Content-Type", "application/json");    //Specify content-type header
  int httpCode = http.POST(postData);
  String payload = http.getString();    //Get the response payload
  Serial.print("FingerID=" + String(finger));
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println("testtest");
  Serial.println();
 // Serial.println(payload[2]);    //Print request response payload
  Serial.println(payload.substring(0,12));
  
 // Serial.println(payload[1]);
  Serial.println(postData);   //Post Data
  Serial.println(finger);     //Print fingerprint ID
   lcd.print("welcome "+payload.substring(0,12));
   

  delay(3000);
  
  postData = "";
  http.end();  //Close connection
}
 uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
     // Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    
    lcd.print("see administration ?!");
     delay(1000);
    lcd.clear();
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  //****************************send fingerID to server********************************

  
          SendFingerprintID(finger.fingerID );


  
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
//********************connect to the WiFi******************
void connectToWiFi(){
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
    lcd.clear();
    lcd.print("Connected to wifi");
     /* delay(1000);
    lcd.clear(); */
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    
}