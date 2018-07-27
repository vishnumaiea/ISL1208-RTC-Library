
//========================================================================//
//                                                                        //
//  ## ISL1208-RTC-Library ##                                             //
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible        //
//  library for ISl1208                                                   //
//                                                                        //
//  Filename : ISL1208_RTC.h                                              //
//  Description : Part of ISL1208 RTC library.                            //
//  Library version : 1.3                                                 //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ESP32-ILI9481-LCD-Library     //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : +05:30 11:49:42 AM, 27-05-2018, Sunday              //
//  License : MIT                                                         //
//                                                                        //
//  File last modified : +05:30 11:06:16 PM, 27-07-2018, Friday           //
//                                                                        //
//========================================================================//

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

#ifndef _ISL1208_RTC_H_
#define _ISL1208_RTC_H_

//========================================================================//

//register addresses

#define ISL1208_ADDRESS 0x6F  //I2C slave addess of RTC IC

#define ISL1208_SC      0x00  //seconds register
#define ISL1208_MN      0x01  //minutes register
#define ISL1208_HR      0x02  //hours register
#define ISL1208_DT      0x03  //date register
#define ISL1208_MO      0x04  //month register
#define ISL1208_YR      0x05  //year register
#define ISL1208_DW      0x06  //day of the week register

#define ISL1208_SCA     0x0C  //alarm seconds register
#define ISL1208_MNA     0x0D  //alarm minutes register
#define ISL1208_HRA     0x0E  //alarm hours register
#define ISL1208_DTA     0x0F  //alarm date register
#define ISL1208_MOA     0x10  //alarm month register
#define ISL1208_DWA     0x11  //alarm day of the week register

//========================================================================//
//main class

class ISL1208_RTC {
  private:

  public:
    byte yearValue, monthValue, dateValue, dayValue, hourValue, minuteValue, secondValue, periodValue;
    byte monthValueAlarm, dateValueAlarm, dayValueAlarm, hourValueAlarm, minuteValueAlarm, secondValueAlarm, periodValueAlarm;

    bool rtc_debug_enable; //prints errors and info to serial monitor is enabled

    ISL1208_RTC(); //constructor
    void begin(); //alternate initializer
    bool isRtcActive(); //checks if the RTC is available on the I2C bus
    bool updateTime(); //update time registers from variables
    bool updateTime(String); //updates time registers from a formatted time string
    bool updateAlarmTime(); //updates alarm registers from variables
    bool updateAlarmTime(String); //updates alarm registers from a formatted alarm time string
    bool fetchTime(); //reads RTC time and alarm registers and updates the variables
    int getHour(); //returns the 12 format hour in DEC
    int getMinute(); //returns minutes in DEC
    int getSecond(); //returns seconds value
    int getPeriod(); //returns time period. 0 = AM, 1 = PM
    int getDate(); //returns date
    int getDay(); //returns day (0 to 6)
    int getMonth(); //returns month (0 to 12)
    int getYear(); //returns year (00 = 2000, 99 = 2099)
    int getAlarmHour();
    int getAlarmMinute();
    int getAlarmSecond();
    int getAlarmPeriod(); //0 = AM, 1 = PM
    int getAlarmDate();
    int getAlarmDay();
    int getAlarmMonth();
    String getTimeString(); //returns formatted time string (hh:mm:ss pp)
    String getDateString(); //returns formatted date string (DD-MM-YYYY)
    String getDayString(); //returns the full name of day
    String getDayString(int); //returns the first n chars of day string (n = 1 to 9)
    String getDateDayString(); //returns a formatted date string with day name (DD-MM-YYYY DAY)
    String getDateDayString(int); //returns a formatted date string with n truncated day name
    String getTimeDateString(); //returns a formatted time date string
    String getTimeDateDayString(); //does what it says!
    String getTimeDateDayString(int); //returns a time, date string with n truncated day string
    String getAlarmString();
    bool printTime(); //prints time to the serial monitor
    bool printAlarmTime(); //prints the alarm time to serial monitor
    byte bcdToDec(byte); //converts a BCD value to DEC
    byte decToBcd(byte); //converts a DEC value to BCD
};

//========================================================================//

#endif //end _ISL1208_RTC_H_
