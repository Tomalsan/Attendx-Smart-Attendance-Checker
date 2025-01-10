#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <Wire.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>
#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <deprecated.h>
#include <require_cpp11.h>

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 //RX slave select
#define RST_PIN 9

const int Password_Length = 4;
// Character to hold password input
String Data;
// Password
String Master = "1234";

byte data_count = 0;

// Character to hold key input
char customKey;
char enter;


MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

LiquidCrystal_I2C lcd(0x27, 16, 2);

byte card_ID[4]; //card UID size 4byte
byte Name1[4]={0x7B,0x66,0xA5,0x1C};//first UID card
byte Name2[4]={0x63,0xD9,0x8F,0xA9};//second UID card

//if you want the arduino to detect the cards only once
int NumbCard[2];//this array content the number of cards. in my case i have just two cards.
int j=0;  
int t;      

int const RedLed=A1;
int const GreenLed=A2;
int const Buzzer=8;

String pass;//pass
String Name;//user name
long Uid;//user number
int n ;//The number of card you want to detect (optional)  


const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {7, 6, 5, }; 
byte colPins[COLS] = {4, 3, 2, }; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char* password="4567";

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();  // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  
  Wire.begin();       // Initialize I2C communication
  lcd.begin(16, 2);   // Initialize the LCD
  lcd.backlight();
  lcd.setCursor(4, 0);
  lcd.print("Welcome!!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Show ID!!");
  
          // Delay to show the message
  
  Serial.println("CLEARSHEET");                 // clears starting at row 1
  Serial.println("LABEL,Date,Time,Emp Name,Uid,IN/OUT");// make four columns (Date,Time,[Name:"user name"]line 48 & 52,[Number:"user number"]line 49 & 53)

  pinMode(RedLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(Buzzer,OUTPUT);

   }
    
void loop() {
    
  

  //look for new card
   if ( ! mfrc522.PICC_IsNewCardPresent()) {
  return;//got to start of loop if there is no card present
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()) {
  return;//if read card serial(0) returns 1, the uid struct contians the ID of the read card.
 }
 




 for (byte i = 0; i < mfrc522.uid.size; i++) {
     card_ID[i]=mfrc522.uid.uidByte[i];

       if(card_ID[i]==Name1[i]){
       Name="Tomal";//user name
       Uid=1907098;//user number
       j=0;//first number in the NumbCard array : NumbCard[j]
       pass="1234";
      }
      else if(card_ID[i]==Name2[i]){
       Name="Nasif";//user name
       Uid=1907120;//user number
       j=1;//Second number in the NumbCard array : NumbCard[j]
       pass="5678";
      }
      else{
        j:
          analogWrite(GreenLed,0);
          analogWrite(RedLed,220);
          t=millis();
          
        while(millis()-t<=4000)
         {
           digitalWrite(8,HIGH);
           delay(100);
           digitalWrite(8,LOW);
           delay(50);
          }
          lcd.backlight();
       lcd.setCursor(0,0);
       lcd.clear();
       lcd.print("ID Not Recognized!!");
       delay(3000);
       lcd.clear();
       lcd.print("Please Show ID!!");
       
          goto cont;//go directly to line 85
     }
}
      if(NumbCard[j] == 1){//to check if the card already detect
      //if you want to use LCD
            NumbCard[j] = 2;
            lcd.backlight();
            lcd.clear();
       lcd.print("Goodbye "+Name);
       lcd.print("!");
       delay(4000);
       lcd.clear();
       lcd.print("Please Show ID!!");
       lcd.setCursor(0, 1);
       lcd.print("or Put Password");
            Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Uid);
      Serial.print(",");
      Serial.println("OUT");
      
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      
         }
         else if(NumbCard[j] == 2)
         {
            //All card activity done for the day !!
         }
      else{
      NumbCard[j] = 1;//put 1 in the NumbCard array : NumbCard[j]={1,1} to let the arduino know if the card was detecting 
          lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    while(1){
    customKey = customKeypad.getKey();
    if (customKey) {
    // Enter keypress into array and increment counter
    Data += customKey;
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
       }

if (data_count == Password_Length) {
    lcd.clear();

    if (Data == pass) {
         Serial.print("DATA,DATE,TIME," + Name);//send the Name to excel
      Serial.print(",");
      Serial.print(Uid);
      Serial.print(",");
      Serial.println("IN"); //send the Number to excel
      analogWrite(GreenLed,220);
      analogWrite(RedLed,0);
      digitalWrite(Buzzer,HIGH);
      
      delay(500);
      digitalWrite(Buzzer,LOW);
      
      Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
      lcd.backlight();
       lcd.setCursor(0,0);
       lcd.clear();
       lcd.print("ID Recognized!!");
       delay(1000);
       lcd.clear();
       lcd.print("Access Granted!!");
       lcd.setCursor(0,1);
       lcd.print("Welcome "+Name);
       lcd.print("!");
       delay(2000);
       lcd.clear();
       lcd.print("Please Show ID!!");
       goto cont;
      
   }
         
    else {
      goto j;
     
    }

    
  }
   
      
}
      delay(1000);
cont:
delay(2000);
analogWrite(GreenLed,0);
analogWrite(RedLed,0);
}
    


}
