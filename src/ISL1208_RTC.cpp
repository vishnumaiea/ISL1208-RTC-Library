
//========================================================================//
//                                                                        //
//  ## ISL1208-RTC-Library ##                                             //
//                                                                        //
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible        //
//  library for ISL1208                                                   //
//                                                                        //
//  Filename : ISL1208_RTC.cpp                                            //
//  Description : Part of ISL1208 RTC library.                            //
//  Library version : 1.4.6                                               //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ISL1208-RTC-Library           //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:49:42 AM, 27-05-2018, Sunday                 //
//  License : MIT                                                         //
//                                                                        //
//  File last modified : IST 09:07 PM 30-10-2019, Wednesday               //
//                                                                        //
//========================================================================//

#include "ISL1208_RTC.h"

//========================================================================//
//constructor

ISL1208_RTC::ISL1208_RTC () {
}

//========================================================================//
//alternate initializer

void ISL1208_RTC::begin() {
  yearValue = 0;
  monthValue = 0;
  dateValue = 0;
  dayValue = 0;
  hourValue = 0;
  minuteValue =  0;
  secondValue = 0;
  periodValue = 0;

  secondValueAlarm = 0;
  minuteValueAlarm = 0;
  hourValueAlarm = 0;
  dateValueAlarm = 0;
  monthValueAlarm = 0;
  dayValueAlarm = 0;

  startOfTheWeek = 0;
  tempByte = 0;

  //set the WRTC (Write RTC Enable Bit) bit to 1 to enable the RTC
  //only then the RTC start counting
  Wire.beginTransmission(ISL1208_ADDRESS);
  Wire.write(ISL1208_SR); //status register
  Wire.write(0x10); //enable WRTC 
  Wire.endTransmission();
}

//========================================================================//
//determines if RTC is available on the bus

bool ISL1208_RTC::isRtcActive() {
  Wire.beginTransmission(ISL1208_ADDRESS); //send the address
  byte error = Wire.endTransmission(); //read ACK

  if(error == 0) { //if RTC is available
    #ifdef ISL1208_RTC_DEBUG
      // Serial.println(F("RTC found on the bus."));
    #endif
    return true;
  }

  #ifdef ISL1208_RTC_DEBUG
    Serial.println(F("Couldn't find RTC."));
  #endif

  return false;
}

//========================================================================//
//fetches time and alarm from serial monitor and update the RTC registers
//first save time values to the variables and then call this function

bool ISL1208_RTC::updateTime() {
  if(!isRtcActive()) { //check RTC
    return false;
  }

  else {
    if((yearValue > 99) || (monthValue > 12) || (monthValue < 1) || (dateValue > 31) || (dateValue < 1) || (hourValue > 23) ||
      (minuteValue > 59) || (secondValue > 59) || (dayValue > 6)) {
        #ifdef ISL1208_RTC_DEBUG
          Serial.print(F("Invalid Date and Time"));
        #endif

        return false;
    }

    #ifdef ISL1208_RTC_DEBUG
      Serial.println();
      Serial.println(F("Updating time from saved values.."));
      Serial.print(F("Date and Time is "));
      Serial.print(hourValue);
      Serial.print(F(":"));
      Serial.print(minuteValue);
      Serial.print(F(":"));
      Serial.print(secondValue);
      Serial.print(F(" "));

      if(periodValue == 1) Serial.print(F("PM, "));
      else Serial.print(F("AM, "));

      Serial.print(dateValue);
      Serial.print(F("-"));
      Serial.print(monthValue);
      Serial.print(F("-"));
      Serial.print(yearValue);
      Serial.print(F(", "));
      Serial.println(dayNamesArray[(startOfTheWeek + dayValue) % 7]);

      // switch (dayValue) { //default start of week is Sunday
      //   case 0:
      //     Serial.println(F("Sunday"));
      //     break;

      //   case 1:
      //     Serial.println(F("Monday"));
      //     break;

      //   case 2:
      //     Serial.println(F("Tuesday"));
      //     break;
        
      //   case 3:
      //     Serial.println(F("Wednesday"));
      //     break;
        
      //   case 4:
      //     Serial.println(F("Thursday"));
      //     break;

      //   case 5:
      //     Serial.println(F("Friday"));
      //     break;
        
      //   case 6:
      //     Serial.println(F("Saturday"));
      //     break;
        
      //   default:
      //     break;
      // }
    #endif

    //write to time register
    Wire.beginTransmission(ISL1208_ADDRESS); //send the I2C address of RTC
    Wire.write(ISL1208_SC); //starting address of time register
    Wire.write(decToBcd(secondValue)); //convert the dec value to BCD ans send
    Wire.write(decToBcd(minuteValue));

    //make a copy of the original variable so we don't lose the DEC formatted values
    tempByte = decToBcd(hourValue); //convert to BCD
    if(periodValue == 1) tempByte |= B00100000; //if PM (1 = PM)
    else tempByte &= B00011111; //if AM (0 = AM)
    Wire.write(tempByte); //write the modified hour value with AM/PM

    Wire.write(decToBcd(dateValue));
    Wire.write(decToBcd(monthValue));
    Wire.write(decToBcd(yearValue));
    Wire.write(decToBcd(dayValue));
    Wire.endTransmission();
  }

  return true;
}

//========================================================================//
//accepts a time string and updates the registers.
//generate a time string and call this function. Helpful when updating time
//over serial console.

bool ISL1208_RTC::setTime(String timeString) {
  if(!isRtcActive()) {
    return false;
  }

  if(timeString.length() != 16) { //chek if time inputs are valid
    Serial.flush();
    Serial.print(F("Invalid time input - "));
    Serial.print(timeString);
    Serial.print(F(", "));
    Serial.println(timeString.length());
    return false;
  }

  else {
    //Time format is : T17122410304213# (TYYMMDDhhmmsspd#)
    if(timeString.charAt(0) == 'T') { //update time register

      #ifdef ISL1208_RTC_DEBUG
        Serial.println();
        Serial.print(F("Time update received = "));
        Serial.println(timeString);
      #endif

      timeString.remove(0,1); //remove 'T'
      timeString.remove(15); //remove delimiter '#'

      yearValue = byte((timeString.substring(0, 2)).toInt()); //convert string values to decimals
      monthValue = byte((timeString.substring(2, 4)).toInt());
      dateValue = byte((timeString.substring(4, 6)).toInt());
      hourValue = byte((timeString.substring(6, 8)).toInt());
      minuteValue = byte((timeString.substring(8, 10)).toInt());
      secondValue = byte((timeString.substring(10, 12)).toInt());
      periodValue = byte((timeString.substring(12, 13)).toInt());
      dayValue = byte((timeString.substring(13)).toInt());

      if((yearValue > 99) || (monthValue > 12) || (monthValue < 1) || (dateValue > 31) || (dateValue < 1) || (hourValue > 23) ||
        (minuteValue > 59) || (secondValue > 59) || (dayValue > 6)) {
          #ifdef ISL1208_RTC_DEBUG
            Serial.print(F("Invalid Date and Time"));
          #endif

          return false;
      }

      #ifdef ISL1208_RTC_DEBUG
        Serial.print(F("Date and Time is "));
        Serial.print(hourValue);
        Serial.print(F(":"));
        Serial.print(minuteValue);
        Serial.print(F(":"));
        Serial.print(secondValue);
        Serial.print(F(" "));

        if(periodValue == 1) Serial.print(F("PM, "));
        else Serial.print(F("AM, "));

        Serial.print(dateValue);
        Serial.print(F("-"));
        Serial.print(monthValue);
        Serial.print(F("-"));
        Serial.print(yearValue);
        Serial.print(F(", "));
        Serial.println(dayNamesArray[(startOfTheWeek + dayValue) % 7]);
      #endif

      //write to time register
      Wire.beginTransmission(ISL1208_ADDRESS); //send the I2C address of RTC
      Wire.write(ISL1208_SC); //starting address of time register
      Wire.write(decToBcd(secondValue)); //convert the dec value to BCD ans send
      Wire.write(decToBcd(minuteValue));

      hourValue = decToBcd(hourValue); //convert to BCD
      if(periodValue == 1) hourValue |= B00100000; //if PM (1 = PM)
      else hourValue &= B00011111; //if AM (0 = AM)
      Wire.write(hourValue); //write the modified hour value with AM/PM

      Wire.write(decToBcd(dateValue));
      Wire.write(decToBcd(monthValue));
      Wire.write(decToBcd(yearValue));
      Wire.write(decToBcd(dayValue));
      Wire.endTransmission();

      // isTimeSet = true;
    }
  }

  return true;
}

//========================================================================//
//updates alarm registers
//first save time values to the variables and then call this function

bool ISL1208_RTC::updateAlarmTime() {
  if(!isRtcActive()) {
    return false;
  }

  else {
    if((monthValueAlarm > 12) || (monthValueAlarm < 1) || (dateValueAlarm > 31) || (dateValueAlarm < 1) || (hourValueAlarm > 23) ||
      (minuteValueAlarm > 59) || (secondValueAlarm > 59) || (dayValueAlarm > 6)) {
        #ifdef ISL1208_RTC_DEBUG
          Serial.print(F("Invalid alarm Date and Time"));
        #endif

        return false;
    }

    #ifdef ISL1208_RTC_DEBUG
      Serial.println();
      Serial.println(F("Updating alarm time from saved values.."));
      Serial.print(F("Alarm Date and Time is "));
      Serial.print(hourValueAlarm);
      Serial.print(F(":"));
      Serial.print(minuteValueAlarm);
      Serial.print(F(":"));
      Serial.print(secondValueAlarm);
      Serial.print(F(" "));

      if(periodValueAlarm == 1) Serial.print(F("PM, "));
      else Serial.print(F("AM, "));

      Serial.print(dateValueAlarm);
      Serial.print(F("-"));
      Serial.print(monthValueAlarm);
      Serial.println(F(" Every year"));
      Serial.println(F(", "));
      Serial.print(F("Day of week  "));
      Serial.print(F(":  "));
      Serial.println(dayNamesArray[(startOfTheWeek + dayValueAlarm) % 7]);
    #endif

    //write to alarm register
    Wire.beginTransmission(ISL1208_ADDRESS);
    Wire.write(ISL1208_SCA); //alarm seconds register
    Wire.write((B10000000 | (decToBcd(secondValueAlarm)))); //the OR operation is required to enable the alarm register
    Wire.write((B10000000 | (decToBcd(minuteValueAlarm))));

    hourValueAlarm = decToBcd(hourValueAlarm); //convert to BCD
    if(periodValueAlarm == 1) hourValueAlarm |= B00100000; //if PM (1 = PM)
    else hourValueAlarm &= B00011111; //if AM (0 = AM)
    Wire.write((B10000000 | hourValue)); //write the modified hour value with AM/PM

    Wire.write((B10000000 | (decToBcd(dateValueAlarm))));
    Wire.write((B10000000 | (decToBcd(monthValueAlarm))));
    Wire.write(decToBcd(dayValueAlarm));
    Wire.endTransmission();

    // isAlarmSet = true;
  }

  return true;
}

//========================================================================//
//updates alarm registers with a string
//generate a alarm time string and call this function. Helpful when updating
//time over serial console.

bool ISL1208_RTC::setAlarmTime(String alarmString) {
  if(!isRtcActive()) {
    return false;
  }

  //Alarm time format is : AMMDDhhmmsspd# 
  if(alarmString.length() != 14) { //check if time inputs are valid
    Serial.flush();
    Serial.print(F("Invalid time input - "));
    Serial.print(alarmString);
    Serial.print(F(", "));
    Serial.println(alarmString.length());
    return false;
  }

  else {
    if(alarmString.charAt(0) == 'A') { //update alarm register
      #ifdef ISL1208_RTC_DEBUG
        Serial.println();
        Serial.print(F("Alarm update received = "));
        Serial.println(alarmString);
      #endif

      alarmString.remove(0,1); //remove 'A'
      alarmString.remove(13); //remove delimiter #

      monthValueAlarm = byte((alarmString.substring(0, 2)).toInt()); //convert string values to decimals
      dateValueAlarm = byte((alarmString.substring(2, 4)).toInt());
      hourValueAlarm = byte((alarmString.substring(4, 6)).toInt());
      minuteValueAlarm = byte((alarmString.substring(6, 8)).toInt());
      secondValueAlarm = byte((alarmString.substring(8, 10)).toInt());
      periodValueAlarm = byte((alarmString.substring(10, 11)).toInt());
      dayValueAlarm = byte((alarmString.substring(11)).toInt());

      if((monthValueAlarm > 12) || (monthValueAlarm < 1) || (dateValueAlarm > 31) || (dateValueAlarm < 1) || (hourValueAlarm > 23) ||
        (minuteValueAlarm > 59) || (secondValueAlarm > 59) || (dayValueAlarm > 6)) {
          #ifdef ISL1208_RTC_DEBUG
            Serial.print(F("Invalid alarm Date and Time"));
          #endif

          return false;
      }

      #ifdef ISL1208_RTC_DEBUG
        Serial.print(F("Alarm Date and Time is "));
        Serial.print(hourValueAlarm);
        Serial.print(F(":"));
        Serial.print(minuteValueAlarm);
        Serial.print(F(":"));
        Serial.print(secondValueAlarm);
        Serial.print(F(" "));

        if(periodValueAlarm == 1) Serial.print(F("PM, "));
        else Serial.print(F("AM, "));

        Serial.print(dateValueAlarm);
        Serial.print(F("-"));
        Serial.print(monthValueAlarm);
        Serial.println(F(" Every year"));
        Serial.print(F("Day of week  "));
        Serial.print(F(":  "));
        Serial.println(dayNamesArray[(startOfTheWeek + dayValueAlarm) % 7]);
      #endif

      //write to alarm register
      Wire.beginTransmission(ISL1208_ADDRESS);
      Wire.write(ISL1208_SCA); //alarm seconds register
      Wire.write((B10000000 | (decToBcd(secondValueAlarm)))); //the OR operation is required to enable the alarm register
      Wire.write((B10000000 | (decToBcd(minuteValueAlarm))));

      hourValueAlarm = decToBcd(hourValueAlarm); //convert to BCD
      if(periodValueAlarm == 1) hourValueAlarm |= B00100000; //if PM (1 = PM)
      else hourValueAlarm &= B00011111; //if AM (0 = AM)
      Wire.write((B10000000 | hourValue)); //write the modified hour value with AM/PM

      Wire.write((B10000000 | (decToBcd(dateValueAlarm))));
      Wire.write((B10000000 | (decToBcd(monthValueAlarm))));
      Wire.write(decToBcd(dayValueAlarm));
      Wire.endTransmission();
    

      // isAlarmSet = true;
    }

    else return false;
  }

  return true;
}

//========================================================================//
//fetches current time and alarm values from RTC and save to variables

bool ISL1208_RTC::fetchTime() {
  if(!isRtcActive()) {
    return false;
  }

  else {
    Wire.beginTransmission(ISL1208_ADDRESS); //send I2C address of RTC
    Wire.write(ISL1208_SC); //time seconds register
    Wire.endTransmission();

    Wire.requestFrom(ISL1208_ADDRESS,7); // now get the bytes of data...

    secondValue = bcdToDec(Wire.read()); //read 6 bytes of data and convert them to DEC
    minuteValue = bcdToDec(Wire.read());
    hourValue = Wire.read();

    if((hourValue & B00100000) == 0) { //check HR21 bit (AM/PM)
      periodValue = 0; //AM
    }
    else {
      periodValue = 1; //PM
    }

    hourValue = bcdToDec(hourValue & B00011111);
    dateValue = bcdToDec(Wire.read());
    monthValue = bcdToDec(Wire.read());
    yearValue = bcdToDec(Wire.read());
    dayValue = bcdToDec(Wire.read());

    Wire.beginTransmission(ISL1208_ADDRESS);
    Wire.write(ISL1208_SCA); //alarm seconds register
    Wire.endTransmission();

    Wire.requestFrom(ISL1208_ADDRESS,6); // now get the byte of data...

    //AND operation is to remove the ENABLE bit (MSB) of each register value
    secondValueAlarm = bcdToDec(B01111111 & Wire.read());
    minuteValueAlarm = bcdToDec(B01111111 & Wire.read());
    hourValueAlarm = B01111111 & Wire.read();

    if((hourValueAlarm & B00100000) == 0) { //check HR21 bit (AM/PM)
      periodValueAlarm = 0; //AM
    }
    else {
      periodValueAlarm = 1; //PM
    }

    hourValueAlarm = bcdToDec(hourValueAlarm & B00011111);
    dateValueAlarm = bcdToDec(B01111111 & Wire.read());
    monthValueAlarm = bcdToDec(B01111111 & Wire.read());
    dayValueAlarm = bcdToDec(B01111111 & Wire.read());
  }
  return true;
}

//========================================================================//

int ISL1208_RTC::getHour() {
  fetchTime();
  return hourValue;
}

//========================================================================//

int ISL1208_RTC::getMinute() {
  fetchTime();
  return minuteValue;
}

//========================================================================//

int ISL1208_RTC::getSecond() {
  fetchTime();
  return secondValue;
}

//========================================================================//

int ISL1208_RTC::getPeriod() {
  fetchTime();
  return periodValue;
}

//========================================================================//

int ISL1208_RTC::getDay() {
  fetchTime();
  return dayValue;
}

//========================================================================//

int ISL1208_RTC::getDate() {
  fetchTime();
  return dateValue;
}

//========================================================================//

int ISL1208_RTC::getMonth() {
  fetchTime();
  return monthValue;
}

//========================================================================//

int ISL1208_RTC::getYear() {
  fetchTime();
  return yearValue;
}

//========================================================================//

int ISL1208_RTC::getAlarmHour() {
  fetchTime();
  return hourValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmMinute() {
  fetchTime();
  return minuteValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmSecond() {
  fetchTime();
  return secondValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmPeriod() {
  fetchTime();
  return periodValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmDay() {
  fetchTime();
  return dayValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmDate() {
  fetchTime();
  return dateValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmMonth() {
  fetchTime();
  return monthValueAlarm;
}

//========================================================================//
//converts the BCD read from RTC register to DEC for transmission

byte ISL1208_RTC::bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}

//========================================================================//
//converts the DEC input values to BCD in order to update the RTC register

byte ISL1208_RTC::decToBcd(byte val) {
  return ((val/10*16) + (val%10));
}

//========================================================================//

String ISL1208_RTC::getTimeString() {
  String tempString = String(getHour());
  tempString += ':';
  tempString += String(getMinute());
  tempString += ':';
  tempString += String(getSecond());
  tempString += ' ';
  if(getPeriod() == 0) tempString += "AM";
  else tempString += "PM";

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDateString() {
  String tempString = String(getDate());
  tempString += '-';
  tempString += String(getMonth());
  tempString += '-';
  tempString += String(getYear() + 2000);

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDayString() {
  return dayNamesArray[(startOfTheWeek + getDay()) % 7];
}

//========================================================================//

String ISL1208_RTC::getDayString(int n) {
  String tempString = getDayString();
  tempString.remove(n);
  return tempString;
}

//========================================================================//

String ISL1208_RTC::getAlarmDayString() {
  return dayNamesArray[(startOfTheWeek + getAlarmDay()) % 7];
}

//========================================================================//

String ISL1208_RTC::getAlarmDayString(int n) {
  String tempString = getAlarmDayString();
  tempString.remove(n);
  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDateDayString() {
  String tempString = getDateString();
  tempString += ", ";
  tempString += getDayString();

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDateDayString(int n) {
  String tempString = getDateString();
  tempString += ", ";
  tempString += getDayString(n);

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getTimeDateString() {
  String tempString = getTimeString();
  tempString += ", ";
  tempString += getDateString();

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getTimeDateDayString() {
  String tempString = getTimeString();
  tempString += ", ";
  tempString += getDateString();
  tempString += ", ";
  tempString += getDayString();

  return tempString;
}


//========================================================================//

String ISL1208_RTC::getTimeDateDayString(int n) {
  String tempString = getTimeString();
  tempString += ", ";
  tempString += getDateString();
  tempString += ", ";
  tempString += getDayString(n);

  return tempString;
}

//========================================================================//
//reads the RTC time register and prints the time
//type "t" to the console to get the current time

bool ISL1208_RTC::printTime() {
  if(fetchTime()) {
    Serial.println();
    Serial.print(F("Time is "));
    Serial.print(hourValue);
    Serial.print(':');
    Serial.print(minuteValue);
    Serial.print(':');
    Serial.print(secondValue);
    Serial.print(' ');

    if(periodValue == 0) {
      Serial.print(F("AM, "));
    }
    else {
      periodValue = 1;
      Serial.print(F("PM, "));
    }

    Serial.print(dateValue);
    Serial.print('-');
    Serial.print(monthValue);
    Serial.print('-');
    Serial.println(yearValue);
    Serial.print(F(", "));
    Serial.println(dayNamesArray[(startOfTheWeek + dayValue) % 7]);

    return true;
  }

  else {
    Serial.println(F("Error: Could not read RTC."));
  }
  return false;
}

//========================================================================//
//reads the RTC alarm register and prints the alarm time
//type "a" to the console for alarm time

bool ISL1208_RTC::printAlarmTime() {
  if(fetchTime()) {
    Serial.println();
    Serial.print(F("Alarm Time is "));
    Serial.print(hourValueAlarm);
    Serial.print(':');
    Serial.print(minuteValueAlarm);
    Serial.print(':');
    Serial.print(secondValueAlarm);
    Serial.print(' ');

    if(periodValueAlarm == 0) { //check HR21 bit (AM/PM)
      Serial.print(F("AM, "));
    }
    else {
      Serial.print(F("PM, "));
    }

    Serial.print(dateValueAlarm);
    Serial.print('-');
    Serial.print(monthValueAlarm);
    Serial.println(F(" Every year"));
    Serial.print(F(" Day of week"));
    Serial.print(F(" :  "));
    Serial.println(dayNamesArray[(startOfTheWeek + dayValueAlarm) % 7]);

    return true;
  }

  else {
    Serial.println(F("Error: Could not read RTC."));
  }
  return false;
}

//========================================================================//
