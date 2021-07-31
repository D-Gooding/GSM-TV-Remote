#include <IRremote.h>
#include <string.h>

//----------------------------------------------------------------------------------------
IRsend irsend;

//-------------------

#include <SoftwareSerial.h> //allows connection between the two boards

//sms is 160 max

char buffer[256]; //stops overflow of arduino

SoftwareSerial GSM(7, 8); //RX and TX 

int chlNum = 0; 

byte channelHexCode[] = {0x910,0x10,0x810,0x410,0xC10,0x210,0xA10,0x610,0xE10,0x110};

void setup() {
  pinMode(3, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("--------------------------------------------------------");
  Serial.println("--  Sony GSM IR Remote v1.00 20/06/2020-----------------");
  Serial.println("--  Created By Dean Gooding  --");
  Serial.println("--------------------------------------------------------");

  //--------------------------------------------------------------
  GSM.begin(9600); //software interface
  SIM900power();
  Serial.print(F("waiting for device connection"));
  for(int i = 0; i<10; i++){
    Serial.print(F("."));
    delay(1000);
  }
  Serial.println("");

  //send Config commands to set the board up

  //query NET Provider (VODA)
  Serial.println(F("Connecting To NET"));
  GSM.print("AT+COPS?\r");
  readGSMBuffer();
  Serial.println(buffer);
  
  //SET SMS mode to text
  Serial.println(F("Setting SMS message to text"));
  GSM.print("AT+CMGF=1\r");
  readGSMBuffer();
  Serial.println(buffer);

  //Send SMS message to Arduino via serial
  Serial.println(F("Forwarding SMS commands to serial"));
  GSM.print("AT+CNMI=2,2,0,0,0\r");
  readGSMBuffer();
  Serial.println(buffer);

  //get NUM
  Serial.println(F("Attempt to get number"));
  GSM.print("AT+CNUM\r");
  readGSMBuffer();
  Serial.println(buffer);

  // Caller ID setup
  Serial.println(F("Enabling Caller ID on incoming calls"));
  GSM.print("AT+CLIP=1\r");
  readGSMBuffer();
  Serial.println(buffer);

  //done
  Serial.print("Done");
  
}

/*
 * This reads the GSM module buffer
 */

void readGSMBuffer(){
  //clear the buffer
  memset(buffer, '\0', sizeof(buffer));
  //The max time for a messsage to be sent
  unsigned int timeout = 5000;
  //the max time in ms for two characters in a message before timeout
  unsigned int chartimeout = 1000;
  //num character to be read
  int i = 0;
  //time of first character of message
  unsigned long timeStart = millis();
  //time at which most recent character of message was recived
  unsigned long timeLastCharReceived = 0;

  while(true){
    while(GSM.available()){
      //read next
      char c = GSM.read();
      timeLastCharReceived = millis();
      buffer[i++] = c;
      if(i >= sizeof(buffer)){
        break;
      }
    }
    //if max char read then stop
    if(i >= sizeof(buffer)){
      break;
    }
    //if the total time taken to receive this messsage exceed the time out,STOP
    if((unsigned long)(millis() - timeStart) > timeout){
      break;
    }
    //if the time since the last char was received exceed the time out Stop
    if((timeLastCharReceived > 0)  &&  ((unsigned long)(millis() - timeLastCharReceived) > chartimeout)) {
      break;
    }

    
  }
}

/*
 * If a text is recived, turn the data into a Readable string
 */
bool receiveSMS(char *message, char *phone, char *datetime){
  //if SMS has been received buffer will contain data in the format
  // +CMT : "+44***********","","date","time"

  //counter
  int i = 0;
  char *p,*p2,*s;
  //search for "+CMT:"
  if((s = strstr(buffer, "+CMT:")) != NULL){
    //get phone num string
    p = strstr(s,"\"");
    //p2 is the next char which is the start of the phone number
    p2 = p + 1;
    p = strstr((char *)(p2), "\"");
    //if phone number found, enclosed with ""
    if(p != NULL){
      i = 0;
      while (p2 < p){
        phone[i++] = *(p2++);
      }
      phone[i] = '\0';
      
    }
    //get date time string
    p = strstr((char *)(p2),",");
    p2 = p + 1;
    p = strstr((char *)(p2),",");
    p2 = p + 2; //we are in the first date time char
    p = strstr((char *)(p2), "\"");
    if(p != NULL){
      i = 0;
      while(p2 < p){
        datetime[i++] = *(p2++);
      }
      datetime[i] = '\0';
    }
    //Extract messsage
    if((s = strstr(s,"\r\n")) != NULL){
      i = 0;
      p = s + 2;
      //sms messages are 160 max
      while((*p != '\r')&&(i  < 159)){
        message[i++] = *(p++);
      }
      message[i] = '\0';
    }
    return true;
    
  }
  //if no CMT was found 
  return false;

  
}

/*
 * Receives a Call
 */

bool receiveCall(char *phone){
  //gets data from buffer and outputs
  int i = 0;
  //tokenise
  char *p, *p2, *s;
  if((s= strstr(buffer, "RING")) != NULL){
    //gets number
    p = strstr(s,"\"");
    p2 = p + 1;
    p = strstr((char *)(p2), "\"");
    if(p !=NULL){
      i = 0;
      while (p2 < p){
        phone[i++] = *(p2++);
      }
      phone[i] = '\0';
    }
    return true;
  }
  return false;

  
}

/*
 * Used to power up the GSM board along with the Arduino 
 */
void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}

/*
 * This is a check alive text that can be done to check if the system is online.
 */

void sendSMS()
{
  GSM.print("AT+CMGS=\"+***********\"\r\n");
  delay(3000);
  GSM.print("GSM System Online");
  GSM.print((char)26);
  GSM.print("\r\n");
}

void loop() {
  readGSMBuffer();
  //if buffer not empty
  uint32_t keyCode = 0;
  if(buffer[0] != '\0'){
    Serial.println(buffer);
    //these variables will be populated by the data given

    char phone[16];
    char datetime[24];
    char message [160];

    //string commands get turned into actions
    if(receiveSMS(message, phone, datetime)){
      Serial.print("Incoming SMS from number: ");
      Serial.println(phone);
      Serial.print("Datetime: ");
      Serial.println(datetime);
      Serial.print("Received Message: ");
      Serial.println(message);
      if((strncasecmp(message, "TV ON", 5) == 0) ||(strncasecmp(message, "TV OFF", 5) == 0)){
         Serial.print("TV ON");
          for (int i = 0; i < 4; i++) {
            irsend.sendSony(0xa90, 12);
            delay(40);
            }
      }
      else if(strncasecmp(message, "TV TEXT", 7) == 0){
        sendSMS();
      }
      else if((strncasecmp(message, "TV CHL", 6) == 0)){
        Serial.print("channel change");
        for (int j = 0; j < (strlen(message)-6); j++){ //loops for the number of number we have for the channel
          delay(60);
          int chlNum = (message[7+j]); //adds j to the position and minus 48 to go from char to int. 
          chlNum = chlNum-48;
          Serial.println(chlNum);
          
          for (int i = 0; i < 4; i++) {
              irsend.sendSony(channelHexCode[chlNum], 12);
              delay(30);
            }
          irsend.sendSony(0xFFFFFFF, 12);   
        }

      }
      if((strncasecmp(message, "TV VOL", 6) == 0)){
        Serial.print("Volume Change");
        for (int j = 0; j < (strlen(message)-6); j++){ 
          delay(60);
          int VolChange = (message[7+j]);
          VolChange = VolChange-48;
          Serial.println(VolChange);
          if(VolChange > 0){
            for (int i = 0; i <= VolChange; i++) {
                irsend.sendSony(0x490, 12);
                delay(30);
              } 
          }
          else{
            for (int i = 0; i <= VolChange; i--) {
                irsend.sendSony(0xC90, 12);
                delay(30);
              }
          }
          irsend.sendSony(0xFFFFFFF, 12);
        }
      }
      
    }
    else if(receiveCall(phone)){
      Serial.print("Call from: ");
      Serial.println(phone);
      if( (strcmp(phone, "***********") == 0) || (strcmp(phone, "***********") == 0) || (strcmp(phone, "***********") == 0)){
        Serial.println("Answering call");
        GSM.println("ATA");
        GSM.println("AT+CEXTERNTONE = 1");
      }
      else{
        Serial.println("Unknown caller, hanging up");
        GSM.println("ATH");
      }
    }
  }

}
