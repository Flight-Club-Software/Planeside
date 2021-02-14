
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <u-blox_config_keys.h>
#include <AirspeedSensor.h>
#include <MS5611.h>

SFE_UBLOX_GPS GPS; //GPS object
Adafruit_BNO055 BNO = Adafruit_BNO055(55,0x29); //Orientation sensor object (ID, address)
MS5611 barometer(0x77); //barometer object
AirspeedSensor ASI; //Airspeed indicator object
double refPressure; //variable to hold pressure shortly after sensor begins to calculate relative pressure



void setup() {

  //Initializing Serial 
 Serial.begin(115200);

  // wait for Serial to connect
 while (!Serial) yield();

  Wire.begin();
  
  //Catches an orientation sensor initialization failure  
  if(!BNO.begin()){
    Serial.println("No orientation sensor detected");
    while(1) delay(500);
  }

  //Catches GPS initialization failure 
  if(!GPS.begin()){
    Serial.println("No GPS sensor detected");
    while(1) delay(500);
  }

  GPS.setI2COutput(COM_TYPE_UBX);
  GPS.saveConfiguration();
   
  //Catches MS5611 barometer initialization failure 
  if(!barometer.begin())
  {
    Serial.println("No barometer detected");
    while(1) delay(500);
  }

  //sets reference pressure to the pressure at sensor startup
  refPressure = barometer.getPressure();
  Serial.println("All sensors working");
  
  //sets clockspeed of I2c to 400khz
  Wire.setClock(400000);
}

void loop() {
  
  print_orientation();
  print_altitude();
  print_coordinates(); 
  print_airspeed();
  delay(1000);
    
}

void print_orientation(){ 
   sensors_event_t event; // creating a sensor event
   BNO.getEvent(&event, Adafruit_BNO055::VECTOR_EULER); 
   Serial.print("{\"orientation\":{\"heading\":");
   Serial.print(event.gyro.y, 3);
   Serial.print(",\"pitch\":");
   Serial.print(event.gyro.x, 3);
   Serial.print(",\"roll\":"); 
   Serial.print(event.gyro.z, 3); 
   Serial.println("}}");
}

void print_altitude(){  
   float p = barometer.getPressure();
   //https://www.weather.gov/media/epz/wxcalc/pressureAltitude.pdf
   float altitude = ((1-pow(p / 1013.25f, 0.190284f)) * 145366.45f );
   Serial.print("{\"altitude\":");
   Serial.print(altitude);
   Serial.println('}');
}

//not sure how to pass both degrees and minutes, as the library only supports getting the number of degrees / 10000000
void print_coordinates(){
         
   float latitude = GPS.getLatitude();
   float longitude = GPS.getLongitude();
   latitude = latitude * 10000000;      
   longitude = longitude * 10000000; 
       
   Serial.print("{\"position\":{\"latDeg\":");
   Serial.print(latitude);
   Serial.print(",\"longDeg\":");   
   Serial.print(longitude);
   Serial.println("}}");
   
}

void print_airspeed(){

   float airspeed = ASI.read();
   Serial.print("{\"airspeed\":");
   Serial.print(airspeed);
   Serial.print("}");
}
