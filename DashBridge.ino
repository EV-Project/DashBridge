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
const uint32_t dashID2 = wheel[2].dashID;    //telemetry messages from the wheel manager
const uint32_t dashID3 = wheel[3].dashID;   //telemetry messages from the wheel manager
const uint32_t dashID4 = wheel[4].dashID;   //telemetry messages from the wheel manager
const uint32_t pedalID = pedalID;           //telemetry messages from the wheel manager

FlexCAN CANbus(1000000);
CANcallbacks canbus(&CANbus);




struct WheelStats{
  uint16_t RPM;
  uint8_t voltage;
  uint8_t current;
  uint8_t throttle;
  uint8_t brake;
  uint8_t telemBits;
  bool redLight;
  bool greenLight;
};  

WheelStats wheelstats[nWheels] = {0};

//varaibles to store the average value
uint32_t avgRPM;
uint16_t avgVoltage;
uint16_t avgCurrent;
uint16_t avgThrottle;
uint16_t avgBrake;

bool wheelnProcessMessage(int n, CAN_message_t &message){
  wheelstats[n].RPM = ((uint16_t)message.buf[0])<<8;
  wheelstats[n].RPM |= ((uint16_t)message.buf[1]);
  wheelstats[n].voltage = message.buf[2];
  wheelstats[n].current = message.buf[3];
  wheelstats[n].throttle = message.buf[4];
  wheelstats[n].brake = message.buf[5];
  wheelstats[n].telemBits =message.buf[6];
  return true;
}

bool wheel1ProcessMessage (CAN_message_t &message){
  return wheelnProcessMessage(1, message);
}
bool wheel2ProcessMessage (CAN_message_t &message){
  return wheelnProcessMessage(2, message);
}
bool wheel3ProcessMessage (CAN_message_t &message){
  return wheelnProcessMessage(3, message);
}
bool wheel4ProcessMessage (CAN_message_t &message){
  return wheelnProcessMessage(4, message);
}
bool pedalProcessMessage (CAN_message_t &message){
  //placeholder
  return true;
}


// a function to get the data returned by each wheel and do some averaging


void setup() {
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
  //let the can callbacks handle unpacking the messages
  while(canbus.receive(message)) {
  }

  avgRPM = 0;
  avgVoltage = 0;
  avgCurrent = 0;
  avgThrottle = 0;
  avgBrake = 0;

  for(int i=0; i<nWheels; i++){
    avgRPM      += wheelstats[i].RPM;
    avgVoltage  += wheelstats[i].voltage;
    avgCurrent  += wheelstats[i].current;
    avgThrottle += wheelstats[i].throttle;
    avgBrake    += wheelstats[i].brake;
  }

  avgRPM /= 4;
  avgVoltage /= 4;
  avgCurrent /= 4;
  avgThrottle /= 4;
  avgBrake /= 4;

  Serial.print(avgRPM);
  Serial.print(",");
  Serial.print(avgVoltage);
  Serial.print(",");
  Serial.print(avgCurrent);
  Serial.print(",");
  Serial.print(avgThrottle);
  Serial.print(",");
  Serial.print(avgBrake);

  //Serial.print(",");
  Serial.println("");

}

    
  
  


