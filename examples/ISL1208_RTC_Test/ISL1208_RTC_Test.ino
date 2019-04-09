
#include <ISL1208_RTC.h>

ISL1208_RTC myRtc = ISL1208_RTC();

void setup() {
    Serial.begin(115200);
    myRtc.begin();
    myRtc.updateTime("T1801050835120#");
} 

void loop() {
    myRtc.printTime();
    delay(1000);
}