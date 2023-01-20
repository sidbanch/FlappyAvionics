/*
FLAPPY CODE VERSION 1
The flappy bird firmware samples four analog ports and bundles them in CSV output to a text file.
Every time the device is powered, a new text file is created.
Typically, fin sampling wires and the raven pyro (set to fire on launch) will go to these analog inputs.
Wiring
CS - D10
SCK - D13
MOSI - D11
MOSO - D12
*/
//LIBRARIES
#include <SPI.h>
#include <SD.h>
//CONFIGURABLES
const int csPin = 10;
const byte pinsCableEnd[]  = {A1,A3,A5,A7};
const int numCables=sizeof(pinsCableEnd);
const int baudRate = 9600;
//LOGGING VARIABLES
int logNumber = 0;
String fileName;
File myFile;
int status;
const int ledPin = 3;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
const long longBlinkInterval = 4000;
const long fastBlinkInterval = 1000;
int ledState = LOW;
int innerInterval = 100;
void setup() {
  pinMode(ledPin,OUTPUT);
  //Initialize SD and serial communications
  Serial.begin(baudRate);
  while (!SD.begin()) {
    Serial.println("SD Initialization failed!");
    status = 2;
    Serial.println(status);
    Serial.println("should be slow blinking");
    delay(1000);
  }
  Serial.println("SD Initialization success!");
  status = 1;
  Serial.println(status);
  Serial.println("should be stagnant light");
  //Figure out the current log number on the SD card
  File rootDir = SD.open("/");
  if (rootDir) {
    File entry = rootDir.openNextFile();
    while (entry) {
      logNumber++;
      entry = rootDir.openNextFile();
    }
    fileName = "LOG" + String(logNumber) + ".txt";
    writeLog("Bootup complete. Log file number " + String(logNumber));
  } else {
    while (!SD.open("/")) {
      Serial.println("Unable to open sd card root directory!");
      delay(100);
    }
  }
  delay(1000);
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  // myFile = SD.open("data.txt", FILE_WRITE);
  // // if the file opened okay, write to it:
  // if (myFile) {
  // Serial.print("Writing to test.txt...");
  // myFile.println("This is a test file :)");
  // myFile.println("testing 1, 2, 3.");
  // } else {
  // // if the file didn't open, print an error:
  // Serial.println("error opening test.txt");
  //}
}
void loop() {
  unsigned long currentMillis = millis(); // keeps track of time
  //Log CSV
  /*
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.print(analogRead(A3));
  Serial.print(",");
  Serial.print(analogRead(A5));
  Serial.print(",");
  Serial.println(analogRead(A7));
  */
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    String s = String(millis()) + ", ";
    for(int i = 0;i<numCables;i++)
    {
        // myFile.print(analogRead(pinsCableEnd[i]));
        // myFile.print(",");
        // Serial.println(1);
        s += String(analogRead(pinsCableEnd[i])) + ", ";
    }
    // myFile.println(analogRead(pinsCableEnd[numCables-1]));
    // myFile.flush();
    s = s.substring(0, s.length() - 2);
    writeLog(s);
  }

//digitalWrite(ledPin, HIGH);
  Serial.println("Inner Interval is Running");
  Serial.println(currentMillis - previousMillis2);
if (currentMillis - previousMillis2 >= 1000) {
  previousMillis2 = currentMillis;
  Serial.println("Inner Interval is Running");
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    //set the LED with the ledState of the variable
    digitalWrite(ledPin, ledState);
  } 

  /*if (status == 1) {
    digitalWrite(ledPin, HIGH);
  } else if (status == 2) {
    if (currentMillis - previousMillis >= longBlinkInterval) {
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    //set the LED with the ledState of the variable
      digitalWrite(ledPin, ledState);
      previousMillis = currentMillis;
    }
  } else if (status == 3) {
    if (currentMillis - previousMillis >= fastBlinkInterval) {
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
    //set the LED with the ledState of the variable
      digitalWrite(ledPin, ledState);
      previousMillis = currentMillis;
    }
  }
  */
  
}
void writeLog(String input) {
  File logFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (!logFile) { // there is an issue here but that is for tomorrow
    Serial.println("Unable to open log file for writing!");
    status = 3;    
    Serial.println(status);
    Serial.println("Should be fast blinking");
  }
  Serial.println(input);
  logFile.println(input);
  logFile.flush();
  logFile.close();
}