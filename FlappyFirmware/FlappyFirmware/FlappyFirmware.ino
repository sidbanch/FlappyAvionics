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

void setup() {
  


  //Initialize SD and serial communications
  Serial.begin(baudRate);
  while (!SD.begin()) {
    Serial.println("SD Initialization failed!");
    delay(1000);
  }
  Serial.println("SD Initialization success!");
  
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
  //Log CSV 
  Serial.print(analogRead(A1));
  Serial.print(",");
  Serial.print(analogRead(A3));
  Serial.print(",");
  Serial.print(analogRead(A5));
  Serial.print(",");
  Serial.println(analogRead(A7));
  /*
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

  delay(100);
  */
}

void writeLog(String input) {
  File logFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (!logFile) Serial.println("Unable to open log file for writing!");
  Serial.println(input);
  logFile.println(input);
  logFile.flush();
  logFile.close();
}







