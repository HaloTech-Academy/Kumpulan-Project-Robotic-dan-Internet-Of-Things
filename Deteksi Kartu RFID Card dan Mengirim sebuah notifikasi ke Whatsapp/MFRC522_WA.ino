//WiFi Library
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// WiFi parameters to be configured
const char* ssid = "AL";
const char* password = "271298al";

//MFRC522 Library
#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 2//D4
#define RST_PIN 0//D3

MFRC522 mfrc522 (SDA_PIN, RST_PIN);

//LCD I2C Library
#include <LiquidCrystal_I2C.h>

//set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

//Variabel untuk menampung WA Bot
String url;

//variable untuk wifi client
WiFiClient client;

//LED Set
int ledPIN = 15;

void setup() {
  Serial.begin(115200);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("Halotech Academy");
  
  //Set LED Pin
  pinMode(ledPIN,OUTPUT);
  
  ConnectToWiFi();
  digitalWrite(ledPIN,HIGH);
  delay(1000);
  digitalWrite(ledPIN,LOW);
  delay(10);

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("WiFi Connected");
  lcd.setCursor(7,1);
  lcd.print(WiFi.SSID());

  delay(1000);
 
  
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Dekatkan kartu RFID anda ke Reader");
  Serial.println();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Please Attach");
  lcd.setCursor(0,1);
  lcd.print("ID Card in here");
}

void loop() {

  //Program MFRC5222
  if (! mfrc522.PICC_IsNewCardPresent())
    return;

  if (! mfrc522.PICC_ReadCardSerial())
    return;

    String IDTAG = "";
    for (byte i=0;i < mfrc522.uid.size;i++)
    {
      IDTAG += mfrc522.uid.uidByte[1];
    }

    Serial.println(IDTAG);
    delay(2000);

  if (mfrc522.PICC_ReadCardSerial())
  {
    //Tidak ada event, Pesan WA tidak dikirim
    Serial.println("Tidak ada event");
  }
  else
  {
    Serial.println("Ada event");

    //LED Indikator
    digitalWrite(ledPIN,HIGH);
    delay(500);
    digitalWrite(ledPIN,LOW);
    delay(10);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ID Card Detect");
    lcd.setCursor(0,1);
    lcd.print("No: "+IDTAG);
    
    //kirim pesan WA ke WA Bot
    kirim_wa(" Ada event!\n\nID Card dengan No: " + IDTAG + "\nTerdeteksi");
  }
}

void ConnectToWiFi()
{
   // Connect to WiFi
  WiFi.begin(ssid, password);
  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.SSID()); 
}

void kirim_wa(String pesan)
{
  url = "http://api.callmebot.com/whatsapp.php?phone=6289633150608&text="+urlencode(pesan)+"&apikey=3480274";
  //kirim pesan
  postData();
}

void postData()
{
  //Siapkan variabel untuk menampung status pesan terkirim atau tidak
  int httpCode;
  //siapkan variabel untuk protokol http yang akan terkoneksi ke server callmebot.com
  HTTPClient http;
  //eksekusi link URL
  http.begin(client,url);
  httpCode = http.POST(url);
  //uji nilai variabel httpCode
  if(httpCode==200)
  {
    Serial.println("Notifikasi WA berhasil terkirim");
  }
  else
  {
    Serial.println("Notifikasi WA gagal terkirim");
  }
  http.end();
}
String urlencode(String str)
{
  String encodedString ="";
  char c;
  char code0, code1, code2;
  for(int i=1; i<str.length();i++)
  {
    c = str.charAt(i);
    //jika ada spasi diantara pesan yg dikirim. maka ganti dengan tanda +
    if(c == ' ')
    { 
      encodedString += '+';
    }
    else if (isalnum(c))
    {
      encodedString += c;
    }
    else
    {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9)
      {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c>>4)&0xf;
      code0 = c + '0';
      if(c>9)
      {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  Serial.println(encodedString);
  return encodedString;
   
}
