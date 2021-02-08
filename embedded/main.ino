
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <SparkFun_Ublox_Arduino_Library.h>
#include <u-blox_config_keys.h>
#include <MS5611.h>
#include <AirspeedSensor.h>

SFE_UBLOX_GPS GPS; //GPS object
Adafruit_BNO055 BNO = Adafruit_BNO055(55,0x29); //Orientation sensor object (ID, address)
MS5611 barometer; //barometer object
AirspeedSensor ASI;
double refPressure; //variable to hold pressure shortly after sensor begins to calculate relative pressure
 // I2C address of Pitot Tube pressure sensor
//SoftwareSerial mySerial(10, 11); // (RX, TX) pins 10 and 11 on arduino board


void setup() {

//Initializing Serial 
Serial.begin(38400);

// wait for Serial to connect
while(!Serial){
  ;
}
  
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


//Catches MS5611 barometer initialization failure 
  if(!barometer.begin())
  {
    Serial.println("No barometer detected");
    while(1) delay(500);
  }

  //sets reference pressure to the pressure at sensor startup
  refPressure = barometer.readPressure();
  Serial.println("All sensors working");
  
  //sets clockspeed of I2c to 400khz
  Wire.setClock(400000);

}

void loop() {
  
  print_orientation();
  print_pressure();
  print_coordinates(); 
  print_airspeed();
  delay(1000);

}

void print_orientation(){
  
  if (BNO.isFullyCalibrated()){        
    sensors_event_t event; // creating a sensor event
    BNO.getEvent(&event); 
    Serial.print(event.orientation.x, 3); //x GPS event data 
    Serial.print(event.orientation.y, 3); //y GPS event data
    Serial.print(event.orientation.z, 3); //z GPS event data
  }

}

void print_pressure(){  
  //Barometer
  //double trueTemperature = barometer.readTemperature();
  long truePressure = barometer.readPressure();
  Serial.print(truePressure);
}

void print_coordinates(){
  
    if(GPS.begin() == true){      
    GPS.setI2COutput(COM_TYPE_UBX);
    GPS.saveConfiguration();

    float latitude = GPS.getLatitude();
    latitude = latitude / 10000000;
    Serial.print(latitude);
        
    float longitude = GPS.getLongitude();
    longitude = longitude / 10000000;     
    Serial.print(longitude);
    
   }
}

void print_airspeed(){
    float airspeed = ASI.read();
    Serial.print(airspeed);
}
