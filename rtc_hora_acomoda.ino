#include <DS3231.h>

// DS3231_Serial_Easy
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
//
// A quick demo of how to use my DS3231-library to 
// quickly send time and date information over a serial link
//
// To use the hardware I2C (TWI) interface of the Arduino you must connect
// the pins as follows:
//
// Arduino Uno/2009:
// ----------------------
// DS3231:  SDA pin   -> Arduino Analog 4 or the dedicated SDA pin
//          SCL pin   -> Arduino Analog 5 or the dedicated SCL pin
//
// Arduino Leonardo:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 2 or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 3 or the dedicated SCL pin
//
// Arduino Mega:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL pin
//
// Arduino Due:
// ----------------------
// DS3231:  SDA pin   -> Arduino Digital 20 (SDA) or the dedicated SDA1 (Digital 70) pin
//          SCL pin   -> Arduino Digital 21 (SCL) or the dedicated SCL1 (Digital 71) pin
//
// The internal pull-up resistors will be activated when using the 
// hardware I2C interfaces.
//
// You can connect the DS3231 to any available pin but if you use any
// other than what is described above the library will fall back to
// a software-based, TWI-like protocol which will require exclusive access 
// to the pins used, and you will also have to use appropriate, external
// pull-up resistors on the data and clock signals.
//

//Declaration of Arduino pins used as HCF4511 inputs
const int A=11;
const int B=10;
const int C=9;
const int D=8;
const int UNl=6;
const int DEl=7;

const int UNv=4;
const int DEv=5;

int count = 0; //the variable used to show the number
int decena = 0;
int unidad = 0;

int horaN = 0;
int minutoN = 0;

int a;

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

void setup()
{
  // Setup Serial connection
  Serial.begin(115200);
  Serial1.begin(9600);
  
  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  //rtc.setDOW(WEDNESDAY);     // Set Day-of-Week to SUNDAY
  //rtc.setTime(19, 32, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(7, 11, 2016);   // Set the date to January 1st, 2014

  pinMode(A, OUTPUT); //LSB
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT); //MSB
  pinMode(UNl, OUTPUT);
  pinMode(DEl, OUTPUT);
  pinMode(UNv, OUTPUT);
  pinMode(DEv, OUTPUT);


  digitalWrite (UNv, HIGH);
  digitalWrite (DEv, HIGH);
  digitalWrite (UNl, HIGH);
  digitalWrite (DEl, HIGH);
      
 
}

void loop()
{

pido_hora();
// sacar para probar si funciona dando la hora
 if (Serial1.available())
      // if text arrived in from BT serial...
      
      {
        a=(Serial1.read());
        if (a==1)
        {// titilar los digitos
          Serial.println("dato 1");
          Serial.print(a,DEC);
          parpadeo();
        }
//        if (a==2)
//        {
//          Serial.println("dato 2");
//          Serial.print(a,DEC);
//          count--;
//          delay(100); //the delay prevent from button bouncing
//          if (count <= 0) //we want to count from 0 to 9!
//            count = 0;
//        }
//        if (a==3)
//        {
//          count++;
//          count++;
//          delay(100); //the delay prevent from button bouncing
//          if (count >= 100) //we want to count from 0 to 9!
//            count = 0;
//        }   
//        if (a == 4)
//        {
//          
//        }
        }
        // you can add more "if" statements with other characters to add more commands
        //to_BCD(count); //convert to binary 
}

void pido_hora()
{
  // Send Day-of-Week
//  Serial.print(rtc.getDOWStr());
//  Serial.print(" ");
//  
//  // Send date
//  Serial.print(rtc.getDateStr());
//  Serial.print(" -- ");

  // Send time
  //Serial.println(rtc.getTimeStr());
  String tiempo=rtc.getTimeStr();
  Serial.println(tiempo);
  String hora = tiempo.substring(0,2);
  //Serial.println(hora);
  horaN = hora.toInt();
  //Serial.println(horaN);
  to_BCD(horaN, 0);
  delay (300);
  String minuto = tiempo.substring(3,5);
  //Serial.println(minuto);
  minutoN = minuto.toInt();
  to_BCD(minutoN, 1);
  // Wait one second before repeating :)
  delay (300);
}

void parpadeo()
{
  String tiempo=rtc.getTimeStr();
  Serial.println(tiempo);
  String hora = tiempo.substring(0,2);
  //Serial.println(hora);
  int nueva_hora = hora.toInt();
  do
  {
  //en que lugar
  digitalWrite (UNl, LOW);
  digitalWrite (DEl, HIGH);
  digitalWrite (UNv, HIGH);
  digitalWrite (DEv, HIGH);
  //valor para que se apague el display
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  //delay(400);
  //en que lugar
  digitalWrite (UNl, HIGH);
  digitalWrite (DEl, LOW);
  digitalWrite (UNv, HIGH);
  digitalWrite (DEv, HIGH);
  delay(300);
  pido_hora();
  a=(Serial1.read());
  if (a==2)
  {//adelanto la hora de a uno
    nueva_hora++;
    Serial1.print(10);
    if (nueva_hora >=24)
    {
      nueva_hora=0;
    };
    
    Serial.println(nueva_hora);
    String tiempo=rtc.getTimeStr();
    String minuto = tiempo.substring(3,5);
    int nuevo_minuto = minuto.toInt();
    rtc.setTime(nueva_hora, nuevo_minuto, 0);// Set the time to 12:00:00 (24hr format)
  };
  } while (a!=3);
  poner_hora();
}

void poner_hora()
{
 String tiempo=rtc.getTimeStr();
  Serial.println(tiempo);
  String hora = tiempo.substring(3,5);
  Serial.println(hora);
  int nueva_min = hora.toInt();
  do
  {
  //en que lugar
  digitalWrite (UNl, HIGH);
  digitalWrite (DEl, HIGH);
  digitalWrite (UNv, LOW);
  digitalWrite (DEv, HIGH);
  //valor para que se apague el display
  digitalWrite(A, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(C, HIGH);
  digitalWrite(D, HIGH);
  //delay(400);
  //en que lugar
  digitalWrite (UNl, HIGH);
  digitalWrite (DEl, HIGH);
  digitalWrite (UNv, HIGH);
  digitalWrite (DEv, LOW);
  delay(300);
  pido_hora();
  a=(Serial1.read());
  if (a==2)
  {//adelanto la hora de a uno
    nueva_min++;
    Serial1.print(10);
    if (nueva_min >=60)
    {
      nueva_min=0;
    };
    
    Serial.println(nueva_min);
    String tiempo=rtc.getTimeStr();
    String hora = tiempo.substring(0,2);
    int nueva_hora = hora.toInt();
    rtc.setTime(nueva_hora, nueva_min, 0);// Set the time to 12:00:00 (24hr format)
  };
  } while (a!=3); 
}

void to_BCD(int valor, int donde)
{    
    decena = valor / 10;
    unidad = valor % 10;
    if (donde == 0){
      digitalWrite (UNl, LOW);
      digitalWrite (DEl, HIGH);
      digitalWrite (UNv, HIGH);
      digitalWrite (DEv, HIGH);
    }
    else {
      digitalWrite (UNv, LOW);
      digitalWrite (DEv, HIGH);
      digitalWrite (UNl, HIGH);
      digitalWrite (DEl, HIGH);
    };
    Escribo(unidad);
    delay(200);
    if (donde == 0){
      digitalWrite (UNl, HIGH);
      digitalWrite (DEl, LOW);
      digitalWrite (UNv, HIGH);
      digitalWrite (DEv, HIGH);
    }
    else {
      digitalWrite (UNv, HIGH);
      digitalWrite (DEv, LOW);
      digitalWrite (UNl, HIGH);
      digitalWrite (DEl, HIGH);
    };
    Escribo(decena);
    delay(200);
//  };
}

void Escribo(int Dato)
{

  switch (Dato) {
  case 0: //write 0000
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
     
    case 1: //write 0001
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
     
    case 2: //write 0010
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
     
    case 3: //write 0011
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, LOW);
      digitalWrite(D, LOW);
      break;
     
    case 4: //write 0100
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
     
    case 5: //write 0101
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
     
    case 6: //write 0110
      digitalWrite(A, LOW);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
     
    case 7: //write 0111
      digitalWrite(A, HIGH);
      digitalWrite(B, HIGH);
      digitalWrite(C, HIGH);
      digitalWrite(D, LOW);
      break;
     
    case 8: //write 1000
      digitalWrite(A, LOW);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break;
     
    case 9: //write 1001
      digitalWrite(A, HIGH);
      digitalWrite(B, LOW);
      digitalWrite(C, LOW);
      digitalWrite(D, HIGH);
      break; 
  }
}
