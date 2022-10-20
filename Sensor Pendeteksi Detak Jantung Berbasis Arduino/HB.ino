//Library LCD I2C
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define samp_siz 4
#define rise_threshold 5

//Pin Variabel
int LedKuning=2;
int LedHijau=3;
int LedMerah=4;

// Pulse Monitor Test Script
int sensorPin = A0;

void setup() {
  pinMode(LedKuning,OUTPUT);
  pinMode(LedHijau,OUTPUT);
  pinMode(LedMerah,OUTPUT);
  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Detak Jantung:");
}

void loop ()
{
   float reads[samp_siz], sum;
   long int now, ptr;
   float last, reader, start;
   float first, second, third, before, print_value;
   bool rising;
   int rise_count;
   int n;
   long int last_beat;
   for (int i = 0; i < samp_siz; i++)
     reads[i] = 0;
   sum = 0;
   ptr = 0;
   while(1)
   {
     // calculate an average of the sensor
     // during a 20 ms period (this will eliminate
     // the 50 Hz noise caused by electric light
     n = 0;
     start = millis();
     reader = 0.;
     do
     {
       reader += analogRead (sensorPin);
       n++;
       now = millis();
     }
     while (now < start + 20);  
     reader /= n;  // we got an average
     // Add the newest measurement to an array
     // and subtract the oldest measurement from the array
     // to maintain a sum of last measurements
     sum -= reads[ptr];
     sum += reader;
     reads[ptr] = reader;
     last = sum / samp_siz;
     // now last holds the average of the values in the array
     // check for a rising curve (= a heart beat)
     if (last > before)
     {
       rise_count++;
       if (!rising && rise_count > rise_threshold)
       {
         // Ok, we have detected a rising curve, which implies a heartbeat.
         // Record the time since last beat, keep track of the two previous
         // times (first, second, third) to get a weighed average.
         // The rising flag prevents us from detecting the same rise 
         // more than once.
         rising = true;
         first = millis() - last_beat;
         last_beat = millis();
         // Calculate the weighed average of heartbeat rate
         // according to the three last beats
         print_value = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
//         Serial.print(print_value);
//         Serial.println(" BPM");
//         Serial.print('\n');

        //LCD Display monitor
         lcd.backlight();
         lcd.setCursor(0,1);
         lcd.print(print_value);
         lcd.setCursor(5,1);
         lcd.print(" bpm");
         
        //Condition 
         if(print_value<60){
          lcd.setCursor(10,1);
          lcd.print("Rendah");
          digitalWrite(LedKuning,HIGH);
          digitalWrite(LedHijau,LOW);
          digitalWrite(LedMerah,LOW);
          delay(10);
          }
          
         if(print_value>=60&&print_value<=100){
          lcd.setCursor(10,1);
          lcd.print("Normal");
          digitalWrite(LedKuning,LOW);
          digitalWrite(LedHijau,HIGH);
          digitalWrite(LedMerah,LOW);
          delay(10);
          }
          if(print_value>100){
          lcd.setCursor(10,1);
          lcd.print("Tinggi");
          digitalWrite(LedKuning,LOW);
          digitalWrite(LedHijau,LOW);
          digitalWrite(LedMerah,HIGH);
          delay(10);
            }
         third = second;
         second = first;
       }
     }
     else
     {
       // Ok, the curve is falling
       rising = false;
       rise_count = 0;
     }
     before = last;
     ptr++;
     ptr %= samp_siz;
   }

} 
