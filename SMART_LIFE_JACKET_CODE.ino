#include <Softwar
 
TinyGPS gps;
SoftwareSerial mySerial(9, 11);
SoftwareSerial ss(3, 5);
float flat, flon;
int heartRate;

int oldHeartRate = 0;  // last heart rate reading from analog input
long previousMillis = 0;  // last time the heart rate was checked, in ms

void setup() {
  Serial.begin(9600);    // initialize serial communication
 // pinMode(13, OUTPUT);   // initialize the LED on pin 13 to indicate when a central is connected
//  Serial.begin(9600); 
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  ss.begin(9600);
  Serial.print("GPS COORDINATES:::");
  delay(100);

  
 }

void loop() {

 bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
    long currentMillis = millis();
      // if 200ms have passed, check the heart rate measurement:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
       int heartRateMeasurement = analogRead(A0);
  heartRate = map(heartRateMeasurement, 0, 1023, 0, 100);
  if (heartRate != oldHeartRate) {      // if the heart rate has changed
    Serial.print("Heart Rate is now: "); // print it
    Serial.println(heartRate);
    const unsigned char heartRateCharArray[2] = { 0, (char)heartRate };
    //heartRateChar.setValue(heartRateCharArray, 2);  // and update the heart rate measurement characteristic
    oldHeartRate = heartRate;           // save the level for next comparison
  } 
        
      }
      
    if (newData)
  {
    //float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LATITUDE=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" \tLONGITUDE=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
  Serial.print(" \n");
  //delay(4000);
}    
      //interruptSetup();
      SendMessage();
      break;
//    case 'r':
  //    RecieveMessage();
    //  break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());

  
  
     /* long currentMillis = millis();
      // if 200ms have passed, check the heart rate measurement:
      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        updateHeartRate();
      }*/
    
    // when the central disconnects, turn off the LED:
    
  }

//void updateHeartRate() {
  /* Read the current voltage level on the A0 analog input pin.
     This is used here to simulate the heart rate's measurement.
  */
  
//}

void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+919672030993\"\r"); // Replace x with mobile number
  delay(1000);
 mySerial.println("LATITUDE=");
  delay(100);
  mySerial.println(flat);// The SMS text you want to send
  delay(100);
  mySerial.println("LONGITUDE=");
  delay(100);  
  mySerial.println(flon);
  delay(100);
  mySerial.println("BPM=");
  delay(100);  
  mySerial.println(heartRate);
  delay(100);  
  mySerial.println((char)26);// ASCII code of CTRL+Z
  
  delay(1000);
}
