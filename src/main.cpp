#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows
const char startChar = '#';
const char endChar = '!';
const char sapperationChar = ',';

int interval = 1000;

unsigned long previousMillis = 0;
String mes;

enum busStopState { connecting,
                    unOccupied,
                    occupied };

busStopState currentState = connecting;

void updateLcd(String buslijn, int minutes) {
  lcd.setCursor(4, 0);
  lcd.print(" ");
  if (minutes > 0) {
    lcd.print(minutes);
  } else {
    lcd.print("<1");
  }
}

void readMessge() {
  bool serialStarted = false;
  String serialRecieved = "";
  if (Serial.available() > 0) {
    char readByte = Serial.read();
    if (readByte == startChar) {
      serialRecieved = "";

      serialStarted = true;
    } else if (serialStarted) {
      if (readByte == endChar) {
        //reset boolean
        serialStarted = false;


        separateString(serialRecieved);
      } else {
        serialRecieved += readByte;
      }
    }
  }
}

void separateString(String str) {
  int commaIndex = str.indexOf('sapperationChar');  // Find the comma index
  bool busArrived = false;
  int stopTimer;
  if (commaIndex != -1) {
    for (int i = 0; i < str.length(); i++) {
      if (i < commaIndex) {
        busArrived += serialRecieved[i];
      } else if (i > commaIndex) {
        stopTimer += serialRecieved[i];
      }
    }
  }
}


void setup() {
  Serial.begin(9600);
  lcd.init();  // initialize the lcd
  lcd.backlight();
}

void loop() {

  switch (currentState) {
    case connecting:
      if (Serial.available() > 0) {
        char readByte = Serial.read();
        if (readByte == startChar) {
          serialRecieved = "";

          serialStarted = true;
        } else if (serialStarted) {
          if (readByte == endChar) {
            //reset boolean
            serialStarted = false;

            if (serialRecieved == "CONNECT,0") {
              currentState = unOccupied;
            }
          } else {
            serialRecieved += readByte;
          }
        }
      }

      break;
    case unOccupied:

      //todo: intergrate in chekc logic

      break;

    case occupied:


      break;
  }
}
