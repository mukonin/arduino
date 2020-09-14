#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

const int nixie_d_1 = 12;
const int nixie_d_2 = 10;
const int nixie_d_4 = 9;
const int nixie_d_8 = 11;

const int row_s_0 = 4; // l_6
const int row_s_1 = 5; // l_5
const int row_m_0 = 6; // l_4
const int row_m_1 = 21; // l_3
const int row_h_0 = 22; // l_2
const int row_h_1 = 23; // l_1

const int button_s = 18;
const int button_m = 19;
const int button_h = 20;

const int rows[6] = {row_s_0, row_s_1, row_m_0, row_m_1, row_h_0, row_h_1};

int button_s_state = 0;
int button_m_state = 0;
int button_h_state = 0;

void setup() {

  pinMode(nixie_d_1, OUTPUT);
  pinMode(nixie_d_2, OUTPUT);
  pinMode(nixie_d_4, OUTPUT);
  pinMode(nixie_d_8, OUTPUT);

  pinMode(row_s_0, OUTPUT);
  pinMode(row_s_1, OUTPUT);
  pinMode(row_m_0, OUTPUT);
  pinMode(row_m_1, OUTPUT);
  pinMode(row_h_0, OUTPUT);
  pinMode(row_h_1, OUTPUT);

  pinMode(button_s, INPUT);
  pinMode(button_m, INPUT);
  pinMode(button_h, INPUT);

  Wire.begin();

  check_clock();

  delay(1000);

  test_tubes();

}

void check_clock() {
  tmElements_t tm;
  if (!RTC.read(tm) && RTC.chipPresent()) {
    tm.Hour = 0;
    tm.Minute = 0;
    tm.Second = 0;
    tm.Day = 0;
    tm.Month = 1;
    tm.Year = CalendarYrToTm(2000);
    RTC.write(tm);
  }
}

void test_tubes() {
  int digits[6];
  int test[19] = {1, 6, 2, 7, 5, 0, 4, 9, 8, 3, 8, 9, 4, 0, 5, 7, 2, 6, 1};
  for (int digit = 0; digit <= 19; digit++) {
    for (int index = 0; index < 6; index++) {
      digits[index] = test[digit];
    }
    for (int i = 0; i < 5; i++) {
      display_time(digits);
    }
  }
}

void loop() {
  check_buttons();

  tmElements_t tm;
  int digits[6];

  RTC.read(tm);

  int seconds = tm.Second;
  digits[0] = seconds % 10;
  digits[1] = seconds / 10;

  int minutes = tm.Minute;
  digits[2] = minutes % 10;
  digits[3] = minutes / 10;

  int hours = tm.Hour;
  digits[4] = hours % 10;
  digits[5] = hours / 10;

  display_time(digits);
}

void check_buttons() {
  int state_s = digitalRead(button_s);
  int state_m = digitalRead(button_m);
  int state_h = digitalRead(button_h);

  if (state_s && !button_s_state) {
    button_s_state = 1;
    tmElements_t tm;
    RTC.read(tm);
    tm.Second = 0;
    RTC.write(tm);
  }
  if (!state_s && button_s_state) {
    button_s_state = 0;
  }

  if (state_m && !button_m_state) {
    button_m_state = 1;
    tmElements_t tm;
    RTC.read(tm);
    if (tm.Minute == 59) {
      tm.Minute = 0;
    } else {
      tm.Minute++;
    }
    RTC.write(tm);
  }
  if (!state_m && button_m_state) {
    button_m_state = 0;
  }

  if (state_h && !button_h_state) {
    button_h_state = 1;
    tmElements_t tm;
    RTC.read(tm);
    if (tm.Hour == 23) {
      tm.Hour = 0;
    } else {
      tm.Hour++;
    }
    RTC.write(tm);
  }
  if (!state_h && button_h_state) {
    button_h_state = 0;
  }
}

void display_time(int digits[]) {
  for (int index = 0; index < 6; index++)
  {
    all_off();

    int digit = digits[index];

    digitalWrite(nixie_d_1, digit % 2);
    digitalWrite(nixie_d_2, (digit / 2) % 2);
    digitalWrite(nixie_d_4, (digit / 4) % 2);
    digitalWrite(nixie_d_8, (digit / 8) % 2);

    digitalWrite(rows[index], HIGH);

    delayMicroseconds(1000);
  }
}

void all_off() {
  digitalWrite(row_s_0, LOW);
  digitalWrite(row_s_1, LOW);
  digitalWrite(row_m_0, LOW);
  digitalWrite(row_m_1, LOW);
  digitalWrite(row_h_0, LOW);
  digitalWrite(row_h_1, LOW);

  digitalWrite(nixie_d_1, HIGH);
  digitalWrite(nixie_d_2, HIGH);
  digitalWrite(nixie_d_4, HIGH);
  digitalWrite(nixie_d_8, HIGH);

  delayMicroseconds(1000);
}
