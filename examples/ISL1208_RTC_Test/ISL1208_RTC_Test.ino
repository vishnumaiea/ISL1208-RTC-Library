
//========================================================================//
//                                                                        //
//  ## ISL1208-RTC-Library Arduino Example ##                             //
//                                                                        //
//  ISL1208 is an RTC from Intersil. This is an Arduino compatible        //
//  library for ISL1208                                                   //
//                                                                        //
//  Filename : ISL1208_RTC_Test.ino                                       //
//  Description : Example Arduino sketch.                                 //
//  Library version : 1.4.6                                               //
//  Author : Vishnu M Aiea                                                //
//  Source : https://github.com/vishnumaiea/ISL1208-RTC-Library           //
//  Author's website : www.vishnumaiea.in                                 //
//  Initial release : IST 11:49:42 AM, 27-05-2018, Sunday                 //
//  License : MIT                                                         //
//                                                                        //
//  File last modified : IST 11:06 AM 25-05-2019, Saturday                //
//                                                                        //
//========================================================================//

#include <ISL1208_RTC.h>

ISL1208_RTC myRtc = ISL1208_RTC();  //create a new object

//========================================================================//
//Arduino setup function executes once

void setup() {
  Serial.begin(115200);  //to print debug info
  Wire.begin();  //initialize I2C
  myRtc.begin();  //initialize RTC
  // myRtc.updateTime("T1801050835120#");  //send the time update string
  Serial.println();
  Serial.println("## ISL1208 RTC Example ##");
  Serial.println("Author : Vishnu M Aiea (@vishnumaiea)");
  Serial.println("=====================================");
  Serial.println();

  if(myRtc.isRtcActive()) {
    Serial.println("RTC found on the bus.");
    Serial.println();
  }
}

//========================================================================//
//infinite loop

void loop() {
  String inputString = "";
  String commandString = "";
  String firstParam = "";
  String secondParam = "";
  String thirdParam = "";

  //send commands and parameters for each operation
  //items are separated by single whitespace
  //you can send up to 3 parameters
  if(Serial.available()) {  //monitor the serial interface
    inputString = Serial.readString();  //read the contents of serial buffer as string
    Serial.println();
    Serial.print("Input String : ");
    Serial.println(inputString);

    //-------------------------------------------------------------------------//
    //the follwing loop extracts the commands and parameters separated by whitespace

    uint8_t posCount = 0;  //the position token of each whitespace
    int indexOfSpace = 0;  //locations of the whitespaces

    while(inputString.indexOf(" ") != -1) { //loop until all whitespace chars are found
      indexOfSpace = inputString.indexOf(" ");  //get the position of first whitespace
      if(indexOfSpace != -1) {  //if a whitespace is found
        if(posCount == 0) //the first one will be command string
          commandString = inputString.substring(0, indexOfSpace); //end char is exclusive
        if(posCount == 1) //second will be second param
          firstParam = inputString.substring(0, indexOfSpace);
        if(posCount == 2) //and so on
          secondParam = inputString.substring(0, indexOfSpace);
        else if(posCount == 3)
          thirdParam = inputString.substring(0, indexOfSpace);
        inputString = inputString.substring(indexOfSpace+1);  //trim the input string
        posCount++;
      }
    }

    //saves the last part of the string if no more whitespace is found
    if(posCount == 0) //means there's just the command
      commandString = inputString;
    if(posCount == 1)
      firstParam = inputString;
    if(posCount == 2)
      secondParam = inputString;
    if(posCount == 3)
      thirdParam = inputString;

    //-------------------------------------------------------------------------//
    //separate and print the received command and parameters

    Serial.print("Command string = ");
    Serial.println(commandString);

    if(firstParam.length() > 0) {  //only print if there's a valid first parameter
      Serial.print("First param = ");
      Serial.println(firstParam);
    }

    if(secondParam.length() > 0) {  //same for other parameters
      Serial.print("Second param = ");
      Serial.println(secondParam);
    }

    if(thirdParam.length() > 0) {
      Serial.print("Third param = ");
      Serial.println(thirdParam);
    }

    Serial.println();

    //-------------------------------------------------------------------------//
    //prints the time

    if(commandString == "printtime") {
      myRtc.printTime();
    }

    //-------------------------------------------------------------------------//
    //prints the alarm time

    else if(commandString == "printalarmtime") {
      myRtc.printAlarmTime();
    }

    //-------------------------------------------------------------------------//
    //send a time string to set time

    else if(commandString == "settime") {
      myRtc.setTime(firstParam);  //first param should be time string
    }

    //-------------------------------------------------------------------------//
    //send a time string to set alarm

    else if(commandString == "setalarm") {
      myRtc.setAlarmTime(firstParam);  //first param should be time string
    }

    //-------------------------------------------------------------------------//
    //prints formatted date

    else if(commandString == "printdate") {
      Serial.println(myRtc.getDateString());
    }

    //-------------------------------------------------------------------------//
    //prints date and day

    else if(commandString == "printdateday") {
      Serial.println(myRtc.getDateDayString());
    }

    //-------------------------------------------------------------------------//
    //prints day

    else if(commandString == "printday") {
      Serial.println(myRtc.getDayString());
    }

    //-------------------------------------------------------------------------//
    //prints formatted time and date

    else if(commandString == "printtimedate") {
      Serial.println(myRtc.getTimeDateString());
    }

    //-------------------------------------------------------------------------//
    //prints formatted time, data and day

    else if(commandString == "printtimedateday") {
      Serial.println(myRtc.getTimeDateDayString());
    }

    //-------------------------------------------------------------------------//
    //prints formatted time, data and day

    else {
      Serial.println("Unknown command.");
      Serial.println();
    }
  }
}

//========================================================================//
