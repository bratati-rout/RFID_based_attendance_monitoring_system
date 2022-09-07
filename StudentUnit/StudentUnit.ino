#include <RFID.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h> 

#define ErrorLED 8 // An LED Used to indicate an error.
#define SuccessLED 7 // An LED used to indicate all successful transactions.

#define RFID_SS_PIN 10 // Slave Select pin for RFID Reader module in SPI Communication.
#define RFID_RST_PIN 9 // Reset pin for RFID Reader module in SPI Communication.

RFID rfid(RFID_SS_PIN, RFID_RST_PIN); // Initialise an instance of RFID with the appropriate values.

String rfidCard; // Stores the UID of the Card that is read from the reader.
const String classCode = "AEIE_4"; // Indicates the classroom in which the Student Unit is placed (Value should be in the format - Dept_Year).
String ClStId; // A string variable to store the class and student unique ID.
String EndOfAttendance = classCode + ".END";

String readRFIDCardInfo(); // Function to read UID from the RFID card reader.
String readLoRaData(); // Function to Read LoRa data.
boolean checkReceivedData(String actual, String expected); // Function to check LoRa data.
void logAttendance();// Function to initiate the actual attendance logging process.
boolean confirmAttendanceLog(); // Function to confirm Successful logging of attendance.

void setup() 
{
  pinMode(ErrorLED,OUTPUT);
  pinMode(SuccessLED, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Student Unit");

  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!");
    digitalWrite(ErrorLED, HIGH);
    while (1);
  }
  Serial.println("Starting the RFID Reader...");
  SPI.begin();
  
  rfid.init();
}

void loop()
{
  String data;
  if(LoRa.parsePacket())
  {
    while(LoRa.available())
    {
      data.concat((char)LoRa.read());
    }
    if(checkReceivedData(data,classCode))
    {
      logAttendance();
    }
    else
    {
      LoRa.sleep();
    }
  }
}

/*Function to initiate the actual attendance logging process*/

void logAttendance()
{
  String uId=readRFIDCardInfo();
  ClStId = String((String(classCode) + "." + uId));
  LoRa.beginPacket();
  LoRa.print(ClStId);
  LoRa.endPacket();
  if(confirmAttendanceLog())
  {
    digitalWrite(SuccessLED,HIGH);
  }
  else
  {
    digitalWrite(ErrorLED,HIGH);
  }
  delay(5000);
  digitalWrite(SuccessLED,LOW);
  digitalWrite(ErrorLED,LOW);
  LoRa.sleep();
}

/* Function to Read LoRa data */

String readLoRaData()
{
  String temp=" ";
  if(LoRa.parsePacket())
  {
    while(LoRa.available())
    {
      temp.concat((char)LoRa.read());
    }
  }
  return temp;
}

/* Function to check LoRa data */

boolean checkReceivedData(String actual, String expected)
{
  return actual.equals(expected);
}

/* Function to read UID from the RFID reader */

String readRFIDCardInfo() 
{
  rfidCard=" ";
  if (rfid.isCard()) 
  {
    if (rfid.readCardSerial()) 
    {
      rfidCard = String(rfid.serNum[0]) + String(rfid.serNum[1]) + String(rfid.serNum[2]) + String(rfid.serNum[3]);
      Serial.println(rfidCard);
    }
    rfid.halt();
  }
  return rfidCard;
}
boolean confirmAttendanceLog()
{
  String incomingData;
  while(true)
  {
    incomingData=readLoRaData();
    if(checkReceivedData(incomingData, ClStId))
    {
      return true;
    }
    else if( checkReceivedData(incomingData, EndOfAttendance))
    {
      return false;
    }
    else
      continue;
  }
  
  return false;
}
