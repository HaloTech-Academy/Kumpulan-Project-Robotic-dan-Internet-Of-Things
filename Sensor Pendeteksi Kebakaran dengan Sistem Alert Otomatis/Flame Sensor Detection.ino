 //Libarary LCD I2C
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

int HijauLed = 2;
int MerahLed = 4;
int SenApi = 5;
int Buzzer = 6;
int SensorApi;

void setup() {

  // initialize the lcd 
  lcd.begin();  
  lcd.clear();
  Serial.begin(9600);

  pinMode(HijauLed,OUTPUT);
  pinMode(MerahLed,OUTPUT);
  pinMode(SenApi,INPUT);
  pinMode(Buzzer,OUTPUT);

  lcd.setCursor(2,0);
  lcd.print("Deteksi Api:");
}

void loop() {
  
SensorApi = digitalRead(SenApi);

if(SensorApi==1){
digitalWrite(MerahLed,HIGH);
digitalWrite(HijauLed,LOW);
lcd.setCursor(0,1);
lcd.print("   Terdeteksi   ");
digitalWrite(Buzzer,HIGH);
delay(10);
}

else{
  digitalWrite(HijauLed,HIGH);
  digitalWrite(MerahLed,LOW);
  lcd.setCursor(0,1);
  lcd.print("Tidak Terdeteksi");
  digitalWrite(Buzzer,LOW);
  delay(10);
  }

}
