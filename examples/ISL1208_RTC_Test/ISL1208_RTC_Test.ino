
//========================================================================//
//                                                                        //
//  ## ISL1208-RTC-Library Arduino Example ##                             //
//                                                                        //
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible        //
//  library for ISl1208                                                   //
//                                                                        //
//  Filename : ISL1208_RTC_test.ino                                       //
//  Description : Example Arduino sketch.                                 //
//  Library version : 1.4                                                 //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ISL1208-RTC-Library           //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:49:42 AM, 27-05-2018, Sunday                 //
//  License : MIT                                                         //
//                                                                        //
//  File last modified : IST 10:32 AM 10-04-2019, Wednesday               //
//                                                                        //
//========================================================================//

#include <ISL1208_RTC.h>

ISL1208_RTC myRtc = ISL1208_RTC();  //create a new object

//========================================================================//

void setup() {
  Serial.begin(115200);  //to print debug info
  Wire.begin();  //initialize I2C
  myRtc.begin();  //initialize RTC
  myRtc.updateTime("T1801050835120#");  //send the time update string
}

//========================================================================//

void loop() {
  myRtc.printTime();
  delay(1000);
}

//========================================================================//
