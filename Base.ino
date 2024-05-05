    #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <elapsedMillis.h>
#include <WiFi.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

#define RE 17
#define DE 16

const char* ssid            = "Iphone";
const char* password        = "hqet4377";

const byte moist[] = {0x01, 0x03, 0x08, 0x02, 0x92, 0x57, 0xB6};
const byte temp[] = {0x01, 0x03, 0x08, 0xFF, 0x9B, 0x57, 0xB6};
const byte EC[] = {0x01, 0x03, 0x08, 0x03, 0xE8, 0x57, 0xB6};
const byte PH[] = {0x01, 0x03, 0x08, 0x00, 0x38, 0x57, 0xB6};
const byte nitro[]  = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos[]   = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte kal[]    = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
byte values[11];
byte MoisValue, TempValue, ECValue, PHValue, NValue, PValue, KValue;
byte SensorReading = 1;

const unsigned long intervalInput   = 3000;   // Interval baca sensor (ms)
const unsigned long intervalOutput  = 1000;   // Interval print LCD (ms)
const unsigned long intervalUpload  = 20000;  // Interval kirim ke Firebase (ms)

elapsedMillis elapsedInput;
elapsedMillis elapsedOutput;
elapsedMillis elapsedUpload;

void setup() {
  lcd.init();
  lcd.backlight();
  //Connect Wifi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi");
    lcd.setCursor(0, 1);
    lcd.print("Connecting to WiFi");

  }
  Serial.println("Connected to WiFi");
  lcd.setCursor(0, 1);
  lcd.print("Connected to WiFi   ");
  delay(5000);
//LCD SET
  lcd.setCursor(0, 0);
  lcd.print("PH   :              ");
  lcd.setCursor(0, 1);
  lcd.print("EC   :              ");
  lcd.setCursor(0, 2);
  lcd.print("Mois :              ");
  lcd.setCursor(0, 3);
  lcd.print("Temp :              ");
  lcd.setCursor(12,0 );
  lcd.print("N : ");
  lcd.setCursor(12,1 );
  lcd.print("P : ");
  lcd.setCursor(12,2 );
  lcd.print("K : ");

pinMode(RE, OUTPUT);
pinMode(DE, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
if (elapsedInput >= intervalInput)
  {
    if (SensorReading == 1)
    {
      SensorRead();
      SensorReading = 0;      
    }
    else
    {
      //Do Nothing
    }
    elapsedInput = 0;
  }

if (elapsedOutput >= intervalOutput)
{
   lcd.setCursor(7,0);
   lcd.print(PHValue);
   lcd.setCursor(7,1);
   lcd.print(ECValue);  
   lcd.setCursor(7,2);
   lcd.print(MoisValue);
   lcd.setCursor(7,3);
   lcd.print(TempValue);
   lcd.setCursor(16,0);
   lcd.print(NValue);
   lcd.setCursor(16,1);
   lcd.print(PValue);
   lcd.setCursor(16,2);
   lcd.print(KValue);
}

}
void SensorRead()
{
  NValue = NRead();
  delay (250);
  PValue = PRead();
  delay (250);
  KValue = KRead();
  delay (250);
  PHValue = PHRead();
  delay (250);
  ECValue = ECRead();
  delay (250);
  MoisValue = MoisRead();
  delay(250);
  TempValue = TempRead();
  delay(250);

}

byte NRead()
{
  // clear the receive buffer
  Serial.flush();

  // switch RS-485 to transmit mode
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);

  // write out the message
  for (uint8_t i = 0; i < sizeof(nitro); i++) Serial.write(nitro[i]);

  // wait for the transmission to complete
  Serial.flush();

  // switching RS485 to receive mode
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // delay to allow response bytes to be received!
  delay(500);

  // read in the received bytes
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte PRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(phos); i++) Serial.write(phos[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte KRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(kal); i++) Serial.write(kal[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte PHRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(PH); i++) Serial.write(PH[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte ECRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(EC); i++) Serial.write(EC[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte MoisRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(moist); i++) Serial.write(moist[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}

byte TempRead()
{
  Serial.flush();
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);
  delay(10);
  for (uint8_t i = 0; i < sizeof(temp); i++) Serial.write(temp[i]);

  Serial.flush();
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);
  delay(500);
  for (byte i = 0; i < 7; i++) 
  {
    values[i] = Serial.read();
  }

  return values[4];
}
