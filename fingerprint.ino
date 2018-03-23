


#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

int getFingerprintIDez();

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


void setup()  
{
  //pinMode(7,INPUT_PULLUP); //camr ung tu
  pinMode(8,OUTPUT);       // relay
  pinMode(10,INPUT_PULLUP); // BUTTON 
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  
  Serial.begin(9600);
  Serial.println("Adafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
  digitalWrite(8,HIGH);
}

void loop()                     // run over and over again
{
 // digitalWrite(8,HIGH);
//  getFingerprintID();
  // getFingerprintIDez();
              //don't ned to run this at full speed.
  
   
//  while(digitalRead(7)==0)
//{
    
    getFingerprintID();
    Serial.println(digitalRead(10));
    Serial.print("Confidece: ");
    Serial.println(finger.confidence);  
    if (finger.confidence>50 || digitalRead(10) == LOW   )
    {
      Serial.println("mo roi");
      Serial.println(digitalRead(10));
      digitalWrite(8,LOW);
      delay(5000);
      finger.confidence=0;
      digitalWrite(8,HIGH);
    }
    
    Serial.print("Confidence: ");
   // finger.confidence=0;
  //    else digitalWrite(8,HIGH);
  //}
 // while(digitalRead(7)==1) {
    
   // if (digitalRead(7)==0)
    //{
      //delay(5000);
  //    digitalWrite(8,HIGH);
   // }
//}
 //  Serial.println(digitalRead(7));
 // delay(1000);
  
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
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
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  //if (finger.confidence >50 ) {digitalWrite(8,HIGH);}
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
 // if (finger.confidence >50 ) {digitalWrite(8,HIGH);}
}
