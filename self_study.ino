#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

int tx = 13;
int rx = 8;
SoftwareSerial bt(tx,rx);
char name1[50];

const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data

boolean newData = false;
int UpperThreshold = 518;
int LowerThreshold = 490;
int reading = 0;
float BPM = 0.0;
bool IgnoreReading = false;
bool FirstPulseDetected = false;
unsigned long FirstPulseTime = 0;
unsigned long SecondPulseTime = 0;
unsigned long PulseInterval = 0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
 Serial.begin(9600);
 Serial.println("<Arduino is ready>");
 lcd.begin(16, 2);
 bt.begin(9600);
 pinMode(8, OUTPUT);
}


void recvWithEndMarker() {
 static byte ndx = 0;
 char endMarker = '\n';
 char rc;
 
 // if (Serial.available() > 0) {
           while (Serial.available() > 0 && newData == false) {
 rc = Serial.read();

 if (rc != endMarker) {
 receivedChars[ndx] = rc;
 ndx++;
 if (ndx >= numChars) {
 ndx = numChars - 1;
 }
 }
 else {
 receivedChars[ndx] = '\0'; // terminate the string
 ndx = 0;
 newData = true;
 }
 }
}

void showNewData() {
 if (newData == true) {
 Serial.print("Your Name: ");
 Serial.println(receivedChars);
 newData = false;
 }
}
void loop()
{

      reading = analogRead(A0); 
      if(reading > UpperThreshold && IgnoreReading == false)
      {
        if(FirstPulseDetected == false)
        {
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else
        {
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }
      if(reading < LowerThreshold)
      {
        IgnoreReading = false;
      }  

      BPM = (1.0/PulseInterval) * 60.0 * 1000;
      Serial.print(BPM);
      Serial.println(" BPM");
      //delay(2000);
      Serial.flush();
      if( BPM > 60 && BPM < 76)
      {
        Serial.println("Enter your name: ");
        delay(10000);
        recvWithEndMarker();
        showNewData();
        Serial.print("Your Heart Rate is: ");
        Serial.println(BPM);
        lcd.print("BPM:");
        lcd.print(BPM);
        lcd.setCursor(0, 1);
        lcd.print("Name: ");
        static int i=0;
        for( i=0;receivedChars[i]!='\0'; i++)
        {
          lcd.print(receivedChars[i]);
          name1[i] = receivedChars[i];
        }
        name1[i] = '\0';
        int bpmsend = 0;
        bpmsend = (int)BPM;
        Serial.println(receivedChars);
        Serial.println(bpmsend);
        //if(bt.available()>0)
        //{
            //bt.print("|");
            //Serial.println(receivedChars);
            //Serial.println(bpmsend);
            digitalWrite(8 , HIGH);
            bt.print("Test");
            bt.print("|");
            bt.print(receivedChars);
            bt.print("|");
            bt.print(bpmsend);
            //Serial.println(receivedChars);
            //Serial.println(bpmsend);
            //bt.print("|");
          //Serial.print("|");
       //}
        while(1){}
      }
      
}

