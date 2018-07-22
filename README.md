# ISL1208-RTC-Library
An Arduino compatible library for Intersil ISL11208 real-time clock IC available in MSOP-8 package. Following are its features.
- Real Time Clock/Calendar
  - Tracks Time in Hours, Minutes, and Seconds
  - Day of the Week, Day, Month, and Year
- 15 Selectable Frequency Outputs
- Single Alarm
   - Settable to the Second, Minute, Hour, Day of the Week, Day, or Month
   - Single Event or Pulse Interrupt Mode
- Automatic Backup to Battery or Super Capacitor
- Power Failure Detection
- On-Chip Oscillator Compensation
- 2 Bytes Battery-Backed User SRAM
- I2C Interface
   - 400kHz Data Transfer Rate
- 400nA Battery Supply Current
- Same Pin Out as ST M41Txx and Maxim DS13xx Devices
- Small Package Options
   - 8 Ld MSOP and SOIC Packages
   - 8 Ld TDFN Package
- Pb-Free Available (RoHS Compliant)

Link to datasheet : https://www.intersil.com/content/dam/intersil/documents/isl1/isl1208.pdf

# Library
### Dependancies

```sh
1. stdint.h
2. Arduino.h
3. Wire.h
```

### Constants

All the constants are defined inside the main header file. It includes the RTC's I2C address and the internal register addresses.
```sh
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
```

### Variables
```sh
byte yearValue;     //least significant digits of a year (eg. 18 for 2018, range is from 00 to 99)
byte monthValue;    //month (eg. 01 for January, range is 01 to 12)
byte dateValue;     //date (eg. 24, range is 01 to 31)
byte hourValue;     //hours (eg. 06, range is 01 to 12 for 12 hour format)
byte minuteValue;   //minutes (eg. 55, range is 00 to 59)
byte secondValue;   //seconds (eg. 30, range is 00 to 59)
byte periodValue;   //period of the day for 12 hour format (0 = AM, 1 = PM)

byte monthValueAlarm;   //same as time values
byte dateValueAlarm;
byte hourValueAlarm;
byte minuteValueAlarm;
byte secondValueAlarm;
byte periodValueAlarm;
```

All are public and so you can access them using the object.

### Classes
```sh
1. ISL1208_RTC
```
The main class with variables and functions.

### Functions
```sh
1. void begin(); //alternate initializer
```
This is same as the default constructor. Use this to explicitly initialize the object. It resets all the time variables.

```sh
2. bool isRtcActive(); //checks if the RTC is available on the I2C bus
```
This checks if the RTC is available on the I2C bus by reading the ACK signal.

```sh
3. bool updateTime(); //update time registers from variables
```
This updates the RTC time registers with the values present on the time variables available in the class. So if you want to set time, first save the values to the variables and then call this function.

```sh
4. bool updateTime(String); //updates time registers from a formatted time string
```
This updates the time from a single formatted time string. Useful updating the time in a single command, for example from serial monitor.
``` TYYMMDDhhmmssp# ``` is the format for time string, where,
- T = indicates time information
- YY = least significant digits of a year (eg. 18 for 2018, range is from 00 to 99)
- MM = month (eg. 01 for January, range is 01 to 12)
- DD = date (eg. 24, range is 01 to 31)
- hh = hours (eg. 06, range is 01 to 12 for 12 hour format)
- mm = minutes (eg. 55, range is 00 to 59)
- ss = seconds (eg. 30, range is 00 to 59)
- p = period of the day for 12 hour format (0 = AM, 1 = PM)
- \# = delimiter

For example, to set the time and date "08:35:12 AM, 05-01-2018", we should send: ``` T1801050835120# ``` , where

- T = indicates time information
- 18 = the year 2018
- 01 = month January
- 05 = date
- 08 = hours
- 35 = minutes
- 12 = seconds
- 0 = AM
- \# = delimiter

```sh
5. bool updateAlarmTime(); //updates alarm registers from variables
```
Updates the alarm registers with the variable values.

```sh
6. bool updateAlarmTime(String); //updates alarm registers from a formatted alarm time string
```

Updates the alarm registers with a formatted string like we saw before. Format is ``` AMMDDhhmmssp# ``` where,

- A = indicates alarm information
- MM = month
- DD = date
- hh = hours
- mm = minutes
- ss = seconds
- p = time period (0 = AM, 1 = PM)
- \# = delimiter

```sh
7. bool fetchTime(); //reads RTC time and alarm registers and updates the variables
```

This is function reads the RTC registers and updates all the variables including the alarm values.

```sh
8. bool printTime(); //prints time to the serial monitor
```

Prints a formatted time string just after fetching the current time.

```sh
9. bool printAlarmTime(); //prints the alarm time to serial monitor
```

Prints alarm time.

```sh
10. byte bcdToDec(byte); //converts a BCD value to DEC
```

The RTC registers save values in BCD format. So we need to convert to and from BCD when we read or write the alarm registers. This function converts BCD values to DEC.

```sh
11. byte decToBcd(byte); //converts a DEC value to BCD
```

This does the opposite.

Finally, here's the register map of the RTC - https://hackster.imgix.net/uploads/attachments/405702/isl1208_2_f51GNl47GW.png



