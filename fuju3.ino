
#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 2
MPU6050 accelgyro;

int WarningValue = 100;
int WarningValueN = -100;
int inByte = 0;         // incoming serial byte
int Motor = 13;
int Speaker = 3;
int16_t ax, ay, az;
int16_t gx, gy, gz;
bool blinkState = false;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x20 for a 16 chars and 2 line display
double temperature1, humidity1;
void setup()
{

  pinMode(Motor, OUTPUT);
  pinMode(Speaker, OUTPUT);
  digitalWrite(Motor, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(Speaker, LOW);    // turn the LED off by making the voltage LOW
  Wire.begin();
  accelgyro.initialize();
  lcd.init();                      // initialize the lcd

  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("Hello, !");
  Serial.begin(9600); // for PC
  Serial1.begin(9600);// send to phone
  Serial.println("DHT11 TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.println();
}

void  MPU()
{
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  Serial.print("a/g:\t");
  Serial.print(ax / 16384); Serial.print("\t");
  Serial.print(ay / 16384); Serial.print("\t");
  Serial.print(az / 16384); Serial.print("\t");
  Serial.print(gx / 131); Serial.print("\t");
  Serial.print(gy / 131); Serial.print("\t");
  Serial.println(gz / 131);

  if ((gx / 131 > WarningValue) || (gy / 131 > WarningValue) || (gz / 131 > WarningValue) || (gx / 131 < WarningValueN) || (gy / 131 < WarningValueN) || (gz / 131 < WarningValueN))
  {
    Serial1.println("Warning!!!!!");
    Serial.println("Warning!!!!!!!!!!!!!!!!!!!");
    int i = 0;
    for (i = 0; i < 5000; i++)
    {
      digitalWrite(Speaker, HIGH);    // turn the LED off by making the voltage LOW
      delayMicroseconds(200);
      digitalWrite(Speaker, LOW);    // turn the LED off by making the voltage LOW
      delayMicroseconds(200);
    }


  }
}
void temp()
{
  int chk = DHT11.read(DHT11PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      Serial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
  humidity1 = (float)DHT11.humidity;
  temperature1 = (float)DHT11.temperature;
  Serial.println((float)DHT11.humidity, 2);
  Serial.println((float)DHT11.temperature, 2);
  //*********************show on the LCD *****************************//
  lcd.clear();                // LCD清屏
  lcd.print("Humidity:");
  lcd.print(humidity1);
  lcd.print(" %");
  lcd.setCursor(0, 1);               // 定位LCD光标为第一行起始位置0，0（列在前，行在后）
  lcd.print("Temper:");
  lcd.print(temperature1);
  lcd.print(" 'C");

  //*********************show in the phone******************************//
  Serial1.println("");
  Serial1.print("Humidity:");
  Serial1.print(humidity1);
  Serial1.println("%");

  Serial1.print("Temperature:");
  Serial1.print(temperature1);
  Serial1.println(" 'C");

}

void MotorControl()
{

  if (Serial1.available() > 0) {
    // get incoming byte:
    inByte = Serial1.read();
    if (inByte == 0xA5 )
    {
      digitalWrite(Motor, HIGH);    // turn the LED off by making the voltage LOW
      Serial.println("#####################");
      Serial.println(inByte, HEX);
    }

    if (inByte == 0xAA )
    {
      digitalWrite(Motor, LOW);    // turn the LED off by making the voltage LOW
      Serial.println(inByte, HEX);
    }
  }
}

void loop()
{
  temp();//send 温度  湿度
  MPU();
  MotorControl();
  delay(200);
}
