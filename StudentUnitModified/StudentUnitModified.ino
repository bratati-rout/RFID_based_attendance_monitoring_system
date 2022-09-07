#include <RFID.h>
#include <LoRa.h>
#include <SPI.h>
#include <Wire.h> 

#define LORA_SS_PIN 8 // Slave Select pin for LoRa module in SPI Communication.
#define LORA_RST_PIN 7 // Reset pin for LoRa module in SPI Communication.

#define RFID_SS_PIN 6 // Slave Select pin for RFID Reader module in SPI Communication.
#define RFID_RST_PIN 5 // Reset pin for RFID Reader module in SPI Communication.

#define ErrorLED 10 // An LED Used to indicate an error.
#define SuccessLED 9 // An LED used to indicate all successful transactions.

RFID rfid(RFID_SS_PIN, RFID_RST_PIN); // Initialise an instance of RFID with the appropriate values.

String rfidCard; // Stores the UID of the Card that is read from the reader.
const String classCode = "AEIE_8"; // Indicates the classroom in which the Student Unit is placed (Value should be in the format - Dept_Year).
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
  Serial.println("Inside Setup");
  Serial.println("Student Unit");

  LoRa.setPins(LORA_SS_PIN,LORA_RST_PIN,2);

  if (!LoRa.begin(433E6)) 
  {
    Serial.println("Starting LoRa failed!\n");
    digitalWrite(ErrorLED, HIGH);
    while (1);
  }
  Serial.println("Starting the RFID Reader...\n");
  SPI.begin();
  
  rfid.init();
}

void loop()
{
  while(!Serial);
  Serial.println("Inside Loop");
  String data;
  if(LoRa.parsePacket())
  {
    while(LoRa.available())
    {
      data.concat((char)LoRa.read());
    }Serial.println(data);
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
  Serial.println("Inside Log Attendance");
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
  Serial.println("Inside Read Lora Data");
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
  Serial.println("Inside Check received data");
  return actual.equals(expected);
}

/* Function to read UID from the RFID reader */
String readRFIDCardInfo() 
{
  Serial.println("Inside read rfid card info");
  while(!rfid.isCard());
  String rfidCard = "";
  if (rfid.readCardSerial()) 
    {
      String rfidCard = String(rfid.serNum[0])+String(rfid.serNum[1])+String(rfid.serNum[2])+String(rfid.serNum[3]);
      Serial.println(rfidCard);
      //while(true);
    }
    return rfidCard;
}

/*Function to confirm that the attendance has been successfully logged*/
boolean confirmAttendanceLog()
{
  Serial.println("Inside confirm attendance log");
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
