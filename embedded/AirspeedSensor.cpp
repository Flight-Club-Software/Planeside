#include <Wire.h>


#include "AirspeedSensor.h"

bool AirspeedSensor :: init(){
  return true;
}

bool AirspeedSensor :: avail(){
  return true;
}

float AirspeedSensor :: read(){
    Wire.requestFrom(0x28, 4); 
    int val[4] = {Wire.read(), Wire.read(), Wire.read(), Wire.read()};
    int16_t rawDP= 0;
    rawDP =  ((byte1 & 0b00111111) << 8) | byte2;

    float diff_press_PSI = -((rawDP - 0.1f * 16383) * (P_max - P_min) / (0.8f * 16383) + P_min);
    float diff_press_pa_raw = diff_press_PSI * PSI_to_Pa;

    float v = sqrt(2.0f * diff_press_pa_raw / 1.1839);
    return v;
}

void AirspeedSensor :: printAirspeed(float velocity){
  Serial.print(velocity);
}
