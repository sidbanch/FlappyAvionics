// https://github.com/nhatuan84/esp32-micro-sdcard
#include <mySD.h>

// CONFIGURE THESE
const int[] digitalSensors = [1,2,3,4];
const int sensorOnValue = 2;
const int sensorOffValue = 0;
const int baudRate = 115200;

// logging variables 
int logNumber = 0; 
String fileName;

void setup() {
  Serial.begin(baudRate);
  
  for (int i = 0; i < digitalSensors.length; i++) {
    pinMode(digitalSensors[i], INPUT);
  }  
  
  while (!SD.begin()) {
    Serial.println("SD Initialization failed!");
  }

  Serial.println("SD Initialization success!");

  // figure out log number
  File rootDir = SD.open("/");
  if (rootDir) {
    File entry = rootDir.openNextFile();
    while (entry) {
      logNumber++; 
      entry = rootDir.openNextFile();
    }

    fileName = "LOG_" + String(logNumber) + ".txt"; 
    writeLog("Bootup complete. Log file number " + String(logNumber));    
    
  } else {
    while (!SD.open("/")) {
      Serial.println("Unable to open sd card rooot directory!");
      delay(100);
    }
  
  }
  
  delay(1000); 
}

void loop() {
  
  String s = String(millis()) + ", ";

  for (int i = 0; i < digitalSensors.length; i++) {
    int sensorValue = digitalRead(digitalSensors[i]) == HIGH ? sensorOnValue : sensorOffValue;
    s += String(sensorValue) + ", ";
  }
  s = s.substring(0, s.length() - 2);

  writeLog(s);
}

void writeLog(String input) {
  File logFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (!logFile) Serial.println("Unable to open log file for writing!");
  Serial.println(input);
  logFile.println(input);
  logFile.flush();
  logFile.close();  
}