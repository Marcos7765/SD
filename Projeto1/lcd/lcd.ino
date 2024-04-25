#include <LiquidCrystal.h>
#include <stdlib.h>
#include <math.h>


struct Time {
  uint8_t second;
  uint8_t minute;
};

inline void incrementTime(struct Time *time) {
    time->second += 1;
    if (time->second == 60) {
        time->second = 0;
        time->minute = time->minute + 1;
        if (time->minute == 60) {
            time->minute = 0;
        }
    }
}

inline void printTemp(float temp, LiquidCrystal &lcd) {
  lcd.setCursor(11, 1);
  lcd.print(String(temp));
  lcd.setCursor(15, 1);
  lcd.print("C");
}

inline void printMode(unsigned int mode, LiquidCrystal &lcd) {
  lcd.setCursor(7, 1);
  lcd.print(String("0") + String(mode) + " |");
}

inline void printTime(struct Time *time, LiquidCrystal &lcd) {
  lcd.setCursor(0,1);
  lcd.print(String(time->minute) + ":" + (time->second < 10 ? "0" : "") + String(time->second) + " |");
}

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
struct Time *time;

void setup() {
  time = (struct Time *)malloc(sizeof(struct Time));
  time->second = 0;
  time->minute = 0;
  lcd.begin(16, 2);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Tempo|");

  lcd.setCursor(6, 0);
  lcd.print("Modo|");

  lcd.setCursor(11, 0);
  lcd.print("Temp");

  printTemp(13.3, lcd);
  printMode(1, lcd);
  printTime(time, lcd);
  incrementTime(time);
  delay(1000);
}
