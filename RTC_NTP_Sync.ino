#include <Wire.h>

void setup() {
  Serial.begin(115200);
  UpdateTime();
}

void loop() {
  DisplayTime();
  delay(1000);
}

void DisplayTime() {
  DateTime Now = rtc.now();
  Serial.print((Now.day() < 10 ? "0" : "") + String(Now.day()) + "/");
  Serial.print((Now.month() < 10 ? "0":"") + String(Now.month()) + "/");
  Serial.print(String(Now.year()) + " ");
  Serial.print((Now.hour() < 10 ? "0":"") + String(Now.hour()) + ":");
  Serial.print((Now.minute() < 10 ? "0":"") + String(Now.minute()) + ":");
  Serial.println((Now.second() < 10 ? "0":"") + String(Now.second()));
}

//###############################################
#include "WiFi.h"
#include "RTClib.h"
#include "time.h"
RTC_DS3231 rtc;
//RTC_DS1307 rtc;
//const char* Timezone = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Europe
const char* Timezone = "GMT0BST,M3.5.0/01,M10.5.0/02";  // UK
const char* ssid = "yourSSID";
const char* password = "yourPASSWORD";

void UpdateTime() {
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 3600;
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, "time.nist.gov");  //(gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", Timezone, 1);                                                  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  tzset();                                                                    // Set the TZ environment variable
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo, 15000)) {  // Wait for 15-sec for time to synchronise
    Serial.println("Failed to obtain time");
  }
  time_t now;              // this is the epoch
  tm tm;                   // the structure tm holds time information in a more convient way
  time(&now);              // read the current time
  localtime_r(&now, &tm);  // update the structure tm with the current time
  int year = tm.tm_year + 1900;
  int month = tm.tm_mon + 1;
  int day = tm.tm_mday;
  int hour = tm.tm_hour;
  int minute = tm.tm_min;
  int second = tm.tm_sec;
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
  Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
}
//###############################################
