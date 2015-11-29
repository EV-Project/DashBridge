/*This program is listning to the CANBus 
 *and sends the data over the serial port to processing 
 */

// including required liberaries 
//#include <KellyCAN.h>
#include <FlexCAN.h>
#include <CANcallbacks.h>
#include <ChallengerEV.h>

//wheels are indexed 1-4
//const int wheelnumber = 3;




//const int wheelnum = wheelnumber - 1;

//pull the constants out of ChallengerEV.h

const uint32_t dashID1 = wheel[1].dashID;   //telemetry messages from the wheel manager
const uint32_t dashID2= wheel[2].dashID;    //telemetry messages from the wheel manager
const uint32_t dashID3 = wheel[3].dashID;   //telemetry messages from the wheel manager
const uint32_t dashID4 = wheel[4].dashID;   //telemetry messages from the wheel manager
const uint32_t pedalID = pedalID;           //telemetry messages from the wheel manager

// I D K
  FlexCAN CANbus(1000000);
  CANcallbacks canbus(&CANbus);


  bool redLightBit = false;
  bool greenLightBit = false;
  // variables for wheel 1
  uint16_t RPM1;
  uint8_t voltage1;
  uint8_t current1;
  uint8_t throttle1;
  uint8_t brake1;
  uint8_t telemBits1;
  // variables for wheel 2
  uint16_t RPM2;
  uint8_t voltage2;
  uint8_t current2;
  uint8_t throttle2;
  uint8_t brake2;
  uint8_t telemBits2;
  // variables for wheel 3
  uint16_t RPM3;
  uint8_t voltage3;
  uint8_t current3;
  uint8_t throttle3;
  uint8_t brake3;
  uint8_t telemBits3;
  //variables for wheel 4
  uint16_t RPM4;
  uint8_t voltage4;
  uint8_t current4;
  uint8_t throttle4;
  uint8_t brake4;
  uint8_t telemBits4;

  //varaibles to store the average value
  uint32_t aRPM;
  uint16_t avoltage;
  uint16_t acurrent;
  uint16_t athrottle;
  uint16_t abrake;
  uint16_t atelemBits;
  
bool wheel1ProcessMessage (CAN_message_t &message){
     RPM1 = ((uint16_t)message.buf[0])<<8;
     RPM1 |= ((uint16_t)message.buf[1]);
     voltage1 = message.buf[2];
     current1 = message.buf[3];
     throttle1 = message.buf[4];
     brake1 = message.buf[5];
     telemBits1 =message.buf[6];
}

bool wheel2ProcessMessage (CAN_message_t &message){
     RPM2 = ((uint16_t)message.buf[0])<<8;
     RPM2 |= ((uint16_t)message.buf[1]);
     voltage2 = message.buf[2];
     current2 = message.buf[3];
     throttle2 = message.buf[4];
     brake2 = message.buf[5];
     telemBits2 = message.buf[6];
  
}
bool wheel3ProcessMessage (CAN_message_t &message){
     RPM3 = ((uint16_t)message.buf[0])<<8;
     RPM3 |= ((uint16_t)message.buf[1]);
     voltage3 = message.buf[2];
     current3 = message.buf[3];
     throttle3 = message.buf[4];
     brake3 = message.buf[5];
     telemBits3 = message.buf[6];
  
}
bool wheel4ProcessMessage (CAN_message_t &message){
     RPM4 = ((uint16_t)message.buf[0])<<8;
     RPM4 |= ((uint16_t)message.buf[1]);
     voltage4 = message.buf[2];
     current4 = message.buf[3];
     throttle4 = message.buf[4];
     brake4 = message.buf[5];
     telemBits4 = message.buf[6];
  
}
bool pedalProcessMessage (CAN_message_t &message){

  
}


// a function to get the data returned by each wheel and do some averaging


void setup() {
  // 

  Serial.begin(9600);
  Serial.println("Dash Display");

  CANbus.begin();

  CAN_filter_t dashFilter1;
  dashFilter1.rtr = 0;
  dashFilter1.ext = 0;
  dashFilter1.id = dashID1;

  CAN_filter_t dashFilter2;
  dashFilter2.rtr = 0;
  dashFilter2.ext = 0;
  dashFilter2.id = dashID2;

  CAN_filter_t dashFilter3;
  dashFilter3.rtr = 0;
  dashFilter3.ext = 0;
  dashFilter3.id = dashID3;

  CAN_filter_t dashFilter4;
  dashFilter4.rtr = 0;
  dashFilter4.ext = 0;
  dashFilter4.id = dashID4;

  CAN_filter_t pedalFilter;
  pedalFilter.rtr = 0;
  pedalFilter.ext = 0;
  pedalFilter.id = pedalID;
  

  CANbus.setFilter(dashFilter1,0);
  CANbus.setFilter(dashFilter2,0);
  CANbus.setFilter(dashFilter3,0);
  CANbus.setFilter(dashFilter4,0);
  CANbus.setFilter(pedalFilter,0);

//fill the remaining filters to prevent ack.
  for (int i = 5; i < 8; ++i)
  {
    CANbus.setFilter(dashFilter1,i);
  }

  canbus.set_callback(dashFilter1.id, &wheel1ProcessMessage);
  canbus.set_callback(dashFilter2.id, &wheel2ProcessMessage);
  canbus.set_callback(dashFilter3.id, &wheel3ProcessMessage);
  canbus.set_callback(dashFilter4.id, &wheel4ProcessMessage);
  canbus.set_callback(pedalFilter.id, &pedalProcessMessage);        
}

void loop() {
  // check for new message
  CAN_message_t message;

  while(canbus.receive(message)) {

  }

  aRPM =      (RPM1+RPM2+RPM3+RPM4)/4;
  avoltage =  (voltage1+voltage2+voltage3+voltage4)/4;
  acurrent =  (current1+current2+current3+current4)/4;
  athrottle = (throttle1+throttle2+throttle3+throttle4)/4;
  abrake =    (brake1+brake2+brake3+brake4)/4;
  atelemBits =(telemBits1+telemBits2+telemBits3+telemBits4)/4;
   

       
}

    
  
  


