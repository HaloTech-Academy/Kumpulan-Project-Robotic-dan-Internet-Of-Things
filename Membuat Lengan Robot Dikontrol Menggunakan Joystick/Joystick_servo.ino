#include <Servo.h> //Memanggil library servo

Servo servo1, servo2;

int joyX = A0;
int joyY = A1;

int x, y; //Variable digunakan untuk membaca pin analog

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  servo1.attach(9); //Untuk mengendalikan servo 1 pada pin 9
  servo2.attach(10); //Untuk mengendalikan servo 2 pada pin 10

}

void loop() {
  // put your main code here, to run repeatedly:
  x = map (analogRead (joyX), 0, 1023, 0, 180); //Mengubah nilai analog input yang besarannya 0 sampai 1023 menjadi untuk putaran servo 0 sampai 180 derajat
  y = map (analogRead (joyY), 0, 1023, 0, 180);
  servo1.write (x);
  servo2.write (y);
  delay (10);
}  
