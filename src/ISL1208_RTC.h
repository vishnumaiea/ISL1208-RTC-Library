
//========================================================================//
//
//  ## ISL1208-RTC-Library ##
//
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible
//  library for ISL1208.
//
//  Filename: ISL1208_RTC.h
//  Description: Header file for ISL1208_RTC library.
//  Library version: 1.4.7
//  Author: Vishnu Mohanan (@vishnumaiea)
//  Source: https://github.com/vishnumaiea/ISL1208-RTC-Library
//  Initial release: IST 11:49:42 AM, 27-05-2018, Sunday
//  License: MIT
//
//  File last modified: +05:30 22:05:27 PM 09-12-2021, Thursday
//
//========================================================================//

#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

#define ISL1208_RTC_DEBUG //uncomment this line to enable debug info

#ifndef _ISL1208_RTC_H_
#define _ISL1208_RTC_H_

//========================================================================//

//register addresses

#define ISL1208_ADDRESS   0x6F  //I2C slave addess of RTC IC

#define ISL1208_SC     0x00  //seconds register
#define ISL1208_MN     0x01  //minutes register
#define ISL1208_HR     0x02  //hours register
#define ISL1208_DT     0x03  //date register
#define ISL1208_MO     0x04  //month register
#define ISL1208_YR     0x05  //year register
#define ISL1208_DW     0x06  //day of the week register

#define ISL1208_SR     0x07  //status register
#define ISL1208_INT    0x08  //interrupt register
#define ISL1208_ATR    0x0A  //analog trimming register
#define ISL1208_DTR    0x0B  //digital trimming register

#define ISL1208_SCA    0x0C  //alarm seconds register
#define ISL1208_MNA    0x0D  //alarm minutes register
#define ISL1208_HRA    0x0E  //alarm hours register
#define ISL1208_DTA    0x0F  //alarm date register
#define ISL1208_MOA    0x10  //alarm month register
#define ISL1208_DWA    0x11  //alarm day of the week register

#define ISL1208_USR1    0x12  //user memory 1
#define ISL1208_USR2    0x13  //user memory 2

//========================================================================//
//main class

class ISL1208_RTC {
  public:
    //all the following byte variables store in DEC format. The BCD conversion is carried out by the functions
    byte yearValue, monthValue, dateValue, dayValue, hourValue, minuteValue, secondValue, periodValue;
    byte monthValueAlarm, dateValueAlarm, dayValueAlarm, hourValueAlarm, minuteValueAlarm, secondValueAlarm, periodValueAlarm;
    byte startOfTheWeek;
    byte tempByte;

    ISL1208_RTC(); //constructor
    void begin(); //initializer
    bool isRtcActive(); //checks if the RTC is available on the I2C bus
    bool updateTime(); //updates time registers from variables
    bool setTime (String); //updates time registers from a formatted time string
    bool updateAlarmTime(); //updates alarm registers from variables
    bool setAlarmTime (String); //updates alarm registers from a formatted alarm time string
    bool fetchTime(); //reads RTC time and alarm registers and updates the variables
    int getHour(); //returns the 12 format hour in DEC
    int getMinute(); //returns minutes in DEC
    int getSecond(); //returns seconds value
    int getPeriod(); //returns time period. 0 = AM, 1 = PM
    int getDate(); //returns date
    int getDay(); //returns day (0 to 6)
    int getMonth(); //returns month (1 to 12)
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
    String getDayString (int); //returns the first n chars of day string (n = 1 to 9)
    String getAlarmDayString(); //returns the full name of alarm day
    String getAlarmDayString (int); //returns the first n chars of alarm day string (n = 1 to 9)
    String getDateDayString(); //returns a formatted date string with day name (DD-MM-YYYY DAY)
    String getDateDayString (int); //returns a formatted date string with n truncated day name
    String getTimeDateString(); //returns a formatted time date string
    String getTimeDateDayString(); //does what it says!
    String getTimeDateDayString (int); //returns a time, date string with n truncated day string
    String getAlarmString();
    bool printTime(); //prints time to the serial monitor
    bool printAlarmTime(); //prints the alarm time to serial monitor
    byte bcdToDec (byte); //converts a BCD value to DEC
    byte decToBcd (byte); //converts a DEC value to BCD

    private:
      String dayNamesArray[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
};

//========================================================================//

#endif //end _ISL1208_RTC_H_
