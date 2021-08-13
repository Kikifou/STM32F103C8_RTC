
//*****************************************
// Carte relais commander par un arduino
//*****************************************                     
 
// version 1.0
#include <Arduino.h>
#include <LiquidCrystal_PCF8574.h>
#include <STM32RTC.H>
#include <Wire.h>
String inputString = "";         // a String to hold incoming data
STM32RTC& rtc = STM32RTC::getInstance();
LiquidCrystal_PCF8574 lcd(0x27); // set the LCD address to 0x27 for a 16 chars and 2 line display

int show = -1;
int error;
bool alarm = false;
int timeoutAlarm = 10;

bool stringComplete = false;

void alarmMatch(void *data)
{
  UNUSED(data);
  digitalWrite(LED_BUILTIN, LOW);
  alarm = true;
  //Serial.println("Alarm Match!");
}

// On ajoute un zéro lorsque le nombre est plus petit que 10
void print2digits(int number) {
  if (number < 10) {
    lcd.print("0");
  }
  lcd.print(number);
}

void initHeure()
{
  rtc.setDay(12);
  rtc.setMonth(8);
  rtc.setYear(21);
  rtc.setHours(21);
  rtc.setMinutes(02);
  rtc.setSeconds(0);
}


void afficheHeure()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   ");
  print2digits(rtc.getDay());
  lcd.print("-");
  print2digits(rtc.getMonth());
  lcd.print("-");
  lcd.print(rtc.getYear() + 2000);
  lcd.setCursor(0,1);
  lcd.print("    ");
  print2digits(rtc.getHours());
  lcd.print(":");
  print2digits(rtc.getMinutes());
  lcd.print(":");
  print2digits(rtc.getSeconds());
  //lcd.println("");
}


void setup()
{
//*********************************** 
// Declaration de la sortie LED
//*********************************** 
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

//*********************************** 
// Initialisation de l'afficheur
//*********************************** 
  // Pin I2C
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);

  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();

  lcd.begin(16, 2); // initialize the lcd
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.print("  Liaison I2C  ");

  //delay(500);
  rtc.begin(true);
  
  rtc.attachInterrupt(alarmMatch);
  rtc.setAlarmDay(12);
  rtc.setAlarmTime(21, 02, 10, 123);
  rtc.enableAlarm(rtc.MATCH_DHHMMSS);

  initHeure();
//*********************************** 
// Initialisation du port serie
//*********************************** 
  serialEvent(); 
  Serial.begin(115200);
  // wait on Serial to be available
  delay(1000);
  //while (!Serial)  ;
  
  //********************************
  // Test la presence de l'afficheur
  //********************************
  //Serial.println("Error: ");
  //Serial.println(error);
  if (error == 0) {
     show = 0;
  }
  else 
  {
    Serial.println(": LCD not found.");
  }
}

//***********************************
// FONCTION DE L'AFFICHEUR POUR MEMO
//***********************************
//    Fonction LCD
//    lcd.setBacklight(0);
//    lcd.setBacklight(255);
//    lcd.clear();
//    lcd.cursor();
//    lcd.noCursor();
//    lcd.blink();
//    lcd.print("Cursor OFF");
//    lcd.noBlink();
//    lcd.display();
//    lcd.setCursor(0, 0);
//    lcd.scrollDisplayLeft();
//    lcd.scrollDisplayRight();


void loop()
{
 // lcd.clear();
 // lcd.setCursor(0,0);
    //chaine = String("I = ") + String(iBoucle);
 //lcd.print("Bonjour");
 afficheHeure();
 //Test la Réception d'une chaine
 //if(Serial.available() > 0) digitalWrite(LED_BUILTIN, HIGH);
   
  if(alarm && timeoutAlarm)
  {
     lcd.setBacklight(0);
     digitalWrite(LED_BUILTIN, LOW);
     delay(300); 
     lcd.setBacklight(255);
     digitalWrite(LED_BUILTIN, HIGH);
     delay(300); 
     timeoutAlarm--;
  }
  else{
    delay(600);
  }
}

