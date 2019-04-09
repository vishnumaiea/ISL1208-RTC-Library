
#include <ISL1208_RTC.h>

ISL1208_RTC myRtc = ISL1208_RTC();

void setup() {
    myRtc.begin();
    myRtc.isRtcActive();
}

void loop() {
    myRtc.getAlarmDate();
}