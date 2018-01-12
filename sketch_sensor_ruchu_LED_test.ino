#include <LiquidCrystal.h>
LiquidCrystal lcd(2,3,4,5,6,7);
int odczytanaWartosc = 0;
boolean takeLowTime;  
boolean lockLow = true;
int calibrationTime = 15;   
long unsigned int pause = 5000;  
long unsigned int lowIn;     

int pirPin = 11;    //the digital pin connected to the PIR sensor's output
int ledPin = 8;

void setup() {
  lcd.begin(16, 2); 
  lcd.clear();
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(pirPin, LOW);

  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(100);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

void loop() {
  
   if(digitalRead(pirPin) == HIGH){
       digitalWrite(ledPin, HIGH);
       delay(10000);
       //the led visualizes the sensors output pin state
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state

       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           }
       }
   odczytanaWartosc = analogRead(A0); //odczytanie wartości z Adc

  if (odczytanaWartosc <100){
    digitalWrite(8, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("JEST NOC");
  lcd.setCursor(0,1);
  lcd.print("AUT. SWIATLO:ON");
  }else{
  lcd.clear();  
  digitalWrite(8,LOW);
  lcd.setCursor(0, 0); //Ustawienie kursora
  lcd.print("JEST DZIEN");
  lcd.setCursor(0,1);
  lcd.print("AUT. SWIATLO:OFF");
    
}
delay(50);

}
