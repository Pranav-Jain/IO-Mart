#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

// Variables for fingerprint scanner
int getFingerprintIDez();
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int Fingerprint_ID = -1;
// *************************************************

// Variables for Bluetooth 
SoftwareSerial bluetooth(4, 5);
int blue_tooth = 0;
// **************************

// Variables for LDR
int sensorPin = A0;
int sensorValue = 0;
int ldr = -1;
// ********************

// Variables for solenoid
int solenoidPin = 8;
// ********************

// Variables for Ultrasonic Sensor
int trigPin = 10;
int echoPin = 11;
long duration;
long distance;
// ********************

void setup()  
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  
  bluetooth.begin(9600);
  
  pinMode(solenoidPin, OUTPUT);
  
  while (!Serial);
    Serial.begin(9600);
  
  Serial.println("Welcome to IO Mart");
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  Serial.println("Waiting for valid finger...");
}

void loop()                     
{
    //ultrasonic();
    Fingerprint_ID = getFingerprintIDez();
    if(Fingerprint_ID != -1)
    {
      Serial.println("Thankyou for shopping with us");
      delay(2000);
      Serial.println("Welcome to IO Mart");
      Serial.println("Waiting for valid finger...");
    }
    Fingerprint_ID = -1;
    //delay(1000);
}

void Bluetooth()
{
  bluetooth.println("\nyes\n");
  delay(100);
}

void LDR()
{
  sensorValue = analogRead(sensorPin);
  //Serial.println(sensorValue);
  if(sensorValue > 800)
  {
    ldr = 1;
  }
  else
    ldr = 0;
  delay(100);
}

void ultrasonic()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= duration*0.034/2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.println();
  
  if(Fingerprint_ID != finger.fingerID)
  {
    blue_tooth = 0;
  }
  delay(2000);
  ldr = -1;
  digitalWrite(solenoidPin, HIGH);
  delay(100);
  digitalWrite(solenoidPin, HIGH);
  delay(100);
  digitalWrite(solenoidPin, HIGH);

    ultrasonic();
    LDR();
    int c = 0;
    while(ldr != 1 && c<15)
    {
      LDR();
      c++;
      delay(1000);
    }
    if(ldr==1)
    {
      if(blue_tooth == 0)
      {
        Bluetooth();
        blue_tooth = 1;
      }
    }
    ultrasonic();
    delay(2000);
    while(distance > 25 && ldr==1)
    {
      LDR();
      ultrasonic();
      delay(3000);
    }
    delay(4000);
    if(ldr==0)
    {
      bluetooth.println("\nno\n");
      delay(5000);
    }
    if(ldr == 1)
      delay(3000);
    digitalWrite(solenoidPin, LOW);
  return finger.fingerID; 
}
