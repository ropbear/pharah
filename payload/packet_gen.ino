#include <TimeLib.h>

#define TIME_HEADER 'T' //Header tag for serial time sync message
#define TIME_REQUEST 7 //ASCII bell character requests time sync message
#define LED 13

void setup() {
  //Set up serial connections with 9600 baud
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  setSyncProvider(requestSync);
}

void loop() {
  if (timeStatus() == timeNotSet) {
    processSyncMessage(); //Wait on connect from PC
  }
  digitalWrite(13,timeStatus() == timeSet);

  //Remove USB from PC
  if (timeStatus() == timeSet) {
  String payload_data = ""; // Get payload output data from Rx pin (pin 0)
  char c = 0;
  int i = 0;
  //recieve payload data
  while (Serial.available()) {
    c = Serial.read();
    payload_data.concat(c); 
  }

  int minutes = 0;
  int seconds = 0;
  
  //begin timestamp
  payload_data.concat(hour());
  payload_data.concat(":");
  //I know I should make a function for this, but I want it to be in line. Much easier.
  minutes = minute();
  if(minutes < 10) {
    payload_data.concat("0");
  }
  payload_data.concat(minutes);
  payload_data.concat(":");
  seconds = second();
  if(seconds < 10){
     payload_data.concat("0");
  }
  payload_data.concat(seconds);
  payload_data.concat(" ");
  payload_data.concat(day());
  payload_data.concat(" ");
  payload_data.concat(month());
  payload_data.concat(" ");
  payload_data.concat(year());
  payload_data.concat(" ");
  payload_data.concat("GMT");
  //end timestamp

  //build CSP packet and send over serial, 9600 baud Big Endian
  forward_csp(payload_data);

  } //arduino must be synced for packets to be sent
  
  delay(10000); //send every 10 seconds
}

void forward_csp(String payload) {
  String packet = "";
  //CSP bit strings, converted to an hex value for the header.
  //First "half"
  // "01" priority 1
  // "10100" Source addr 20
  // "01000" Dest addr is 8 (Ground Station)
  //Second "half"
  // "010101" Source port is 21 (0-7 reserved, 8-47 subsystem services)
  // "010101" Dest port is 21
  // "0000" Reserved
  // "0000" Checksum flags
  String first_half = String(688,HEX);
  String second_half = String(55500,HEX);
  packet += first_half+second_half;
  
  //CSP data, big endian
  packet += payload;
  packet += '\n'; //FOR SERIAL MONITORING ONLY. DO NOT DEPLOY WITH THIS UNCOMMENTED.
  Serial.print(packet);
}

//Repurposed examples from the Time.h library

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1514764800; //00:00:00 1 Jan, 2018
  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST); // This is the non-printable character that shows up on start.
  return 0; // the time will be sent later in response to serial mesg
}

//END example functions from Time.h library
