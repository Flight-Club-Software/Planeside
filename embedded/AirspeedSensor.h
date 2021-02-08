#ifndef AIRSPEEDSENSOR_H
#define AIRSPEEDSENSOR_H

#include <Arduino.h>
class AirspeedSensor{
  public: 
        constexpr static float P_min = -1.0f;
        constexpr static float P_max = 1.0f;
        constexpr static float PSI_to_Pa = 6894.757f;
  public: 
        void printAirspeed(float alt);
        bool init();
        bool avail();
        float read();
};

#endif