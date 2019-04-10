
//========================================================================//
//                                                                        //
//  ## ISL1208-RTC-Library ##                                             //
//                                                                        //
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible        //
//  library for ISl1208                                                   //
//                                                                        //
//  Filename : ISL1208_RTC.cpp                                            //
//  Description : Part of ISL1208 RTC library.                            //
//  Library version : 1.4.1                                               //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ISL1208-RTC-Library           //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:49:42 AM, 27-05-2018, Sunday                 //
//  License : MIT                                                         //
//                                                                        //
//  File last modified : IST 10:32 AM 10-04-2019, Wednesday               //
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
  hourValue = 0;
  minuteValue =  0;
  secondValue = 0;
  periodValue = 0;

  secondValueAlarm = 0;
  minuteValueAlarm = 0;
  hourValueAlarm = 0;
  dateValueAlarm = 0;
  monthValueAlarm = 0;

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
    #ifdef ISL1208_RTC_DEBUG
      Serial.println();
      Serial.println(F("Time update received."));
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
      Serial.println(yearValue);
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

  if(timeString.length() != 15) { //chek if time inputs are valid
    Serial.flush();
    Serial.print(F("Invalid time input - "));
    Serial.print(timeString);
    Serial.print(F(", "));
    Serial.println(timeString.length());
    return false;
  }

  else {
    //Time format is : T1712241030421#
    if(timeString.charAt(0) == 'T') { //update time register

      #ifdef ISL1208_RTC_DEBUG
        Serial.println();
        Serial.print(F("Time update received = "));
        Serial.println(timeString);
      #endif

      timeString.remove(0,1); //remove 'T'
      timeString.remove(14); //remove delimiter '#'

      yearValue = byte((timeString.substring(0, 2)).toInt()); //convert string values to decimals
      monthValue = byte((timeString.substring(2, 4)).toInt());
      dateValue = byte((timeString.substring(4, 6)).toInt());
      hourValue = byte((timeString.substring(6, 8)).toInt());
      minuteValue = byte((timeString.substring(8, 10)).toInt());
      secondValue = byte((timeString.substring(10, 12)).toInt());
      periodValue = byte((timeString.substring(12)).toInt());

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
        Serial.println(yearValue);
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
    #ifdef ISL1208_RTC_DEBUG
      Serial.println();
      Serial.println(F("Alarm update received."));
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

  //Alarm time format is : A12241030421#
  if(alarmString.length() != 13) { //chek if time inputs are valid
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
      alarmString.remove(12); //remove delimiter #

      monthValueAlarm = byte((alarmString.substring(0, 2)).toInt()); //convert string values to decimals
      dateValueAlarm = byte((alarmString.substring(2, 4)).toInt());
      hourValueAlarm = byte((alarmString.substring(4, 6)).toInt());
      minuteValueAlarm = byte((alarmString.substring(6, 8)).toInt());
      secondValueAlarm = byte((alarmString.substring(8, 10)).toInt());
      periodValueAlarm = byte((alarmString.substring(10)).toInt());

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

    secondValue = Wire.read(); //read 6 bytes of data
    minuteValue = Wire.read();
    hourValue = Wire.read();
    dateValue = Wire.read();
    monthValue = Wire.read();
    yearValue = Wire.read();
    dayValue = Wire.read();

    if((hourValue & B00100000) == 0) { //check HR21 bit (AM/PM)
      periodValue = 0; //AM
    }
    else {
      periodValue = 1; //PM
    }

    Wire.beginTransmission(ISL1208_ADDRESS);
    Wire.write(ISL1208_SCA); //alarm seconds register
    Wire.endTransmission();

    Wire.requestFrom(ISL1208_ADDRESS,6); // now get the byte of data...

    //AND operation is to remove the ENABLE bit (MSB) of each register value
    secondValueAlarm = B01111111 & Wire.read();
    minuteValueAlarm = B01111111 & Wire.read();
    hourValueAlarm = B01111111 & Wire.read();
    dateValueAlarm = B01111111 & Wire.read();
    monthValueAlarm = B01111111 & Wire.read();
    dayValueAlarm = B01111111 & Wire.read();
  }
  return true;
}

//========================================================================//

int ISL1208_RTC::getHour() {
  fetchTime();
  return bcdToDec(hourValue & B00011111);
}

//========================================================================//

int ISL1208_RTC::getMinute() {
  fetchTime();
  return bcdToDec(minuteValue);
}

//========================================================================//

int ISL1208_RTC::getSecond() {
  fetchTime();
  return bcdToDec(secondValue);
}

//========================================================================//

int ISL1208_RTC::getPeriod() {
  fetchTime();
  if((hourValue & B00100000) == 0) { //check HR21 bit (AM/PM)
    periodValue = 0; //AM
    return periodValue;
  }
  else {
    periodValue = 1; //PM
    return periodValue;
  }
}

//========================================================================//

int ISL1208_RTC::getDay() {
  fetchTime();
  return dayValue;
}

//========================================================================//

int ISL1208_RTC::getDate() {
  fetchTime();
  return bcdToDec(dateValue);
}

//========================================================================//

int ISL1208_RTC::getMonth() {
  fetchTime();
  return bcdToDec(monthValue);
}

//========================================================================//

int ISL1208_RTC::getYear() {
  fetchTime();
  return bcdToDec(yearValue);
}

//========================================================================//

int ISL1208_RTC::getAlarmHour() {
  fetchTime();
  return bcdToDec(hourValueAlarm & B00011111);
}

//========================================================================//

int ISL1208_RTC::getAlarmMinute() {
  fetchTime();
  return bcdToDec(minuteValueAlarm);
}

//========================================================================//

int ISL1208_RTC::getAlarmSecond() {
  fetchTime();
  return bcdToDec(secondValueAlarm);
}

//========================================================================//

int ISL1208_RTC::getAlarmPeriod() {
  fetchTime();
  if((hourValueAlarm & B00100000) == 0) { //check HR21 bit (AM/PM)
    periodValueAlarm = 0; //AM
    return periodValueAlarm;
  }
  else {
    periodValueAlarm = 1; //PM
    return periodValueAlarm;
  }
}

//========================================================================//

int ISL1208_RTC::getAlarmDay() {
  fetchTime();
  return dayValueAlarm;
}

//========================================================================//

int ISL1208_RTC::getAlarmDate() {
  fetchTime();
  return bcdToDec(dateValueAlarm);
}

//========================================================================//

int ISL1208_RTC::getAlarmMonth() {
  fetchTime();
  return bcdToDec(monthValueAlarm);
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
  fetchTime();
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
  fetchTime();
  String tempString = String(getDate());
  tempString += '-';
  tempString += String(getMonth());
  tempString += '-';
  tempString += String(getYear() + 2000);

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDayString() {
  fetchTime();
  String tempString;

  switch(getDay()) {
    case 0: tempString = "Sunday";
    case 1: tempString = "Monday";
    case 2: tempString = "Tuesday";
    case 3: tempString = "Wednesday";
    case 4: tempString = "Thursday";
    case 5: tempString = "Friday";
    case 6: tempString = "Saturday";
  }

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDayString(int n) {
  fetchTime();
  String tempString;

  if(n == 3) {
    switch(getDay()) {
      case 0: tempString = "Sun";
      case 1: tempString = "Mon";
      case 2: tempString = "Tue";
      case 3: tempString = "Wed";
      case 4: tempString = "Thu";
      case 5: tempString = "Fri";
      case 6: tempString = "Sat";
    }
  }

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDateDayString() {
  fetchTime();
  String tempString = getDateString();
  tempString += getDayString();

  return tempString;
}

//========================================================================//

String ISL1208_RTC::getDateDayString(int n) {
  fetchTime();
  String tempString = getDateString();
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
    Serial.print(bcdToDec(hourValue & B00011111)); //convert the read BCD to DEC
    Serial.print(':');
    Serial.print(bcdToDec(minuteValue));
    Serial.print(':');
    Serial.print(bcdToDec(secondValue));
    Serial.print(' ');

    if((hourValue & B00100000) == 0) { //check HR21 bit (AM/PM)
      periodValue = 0;
      Serial.print(F("AM, "));
    }
    else {
      periodValue = 1;
      Serial.print(F("PM, "));
    }

    Serial.print(bcdToDec(dateValue));
    Serial.print('-');
    Serial.print(bcdToDec(monthValue));
    Serial.print('-');
    Serial.println(bcdToDec(yearValue));

    return true;
  }

  else {
    Serial.println("Error: Could not read RTC.");
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
    Serial.print(bcdToDec(hourValue & B00011111));
    Serial.print(':');
    Serial.print(bcdToDec(minuteValueAlarm));
    Serial.print(':');
    Serial.print(bcdToDec(secondValueAlarm));
    Serial.print(' ');

    if((hourValueAlarm & B00100000) == 0) { //check HR21 bit (AM/PM)
      periodValueAlarm = 0;
      Serial.print(F("AM, "));
    }
    else {
      periodValueAlarm = 1;
      Serial.print(F("PM, "));
    }

    Serial.print(bcdToDec(dateValueAlarm));
    Serial.print('-');
    Serial.print(bcdToDec(monthValueAlarm));
    Serial.println(F(" Every year"));

    return true;
  }

  else {
    Serial.println(F("Error: Could not read RTC."));
  }
  return false;
}

//========================================================================//
