#include <SparkFun_MLX90632_Arduino_Library.h>


#include <Wire.h> // I2C library, required for MLX90614
#include <SparkFunMLX90614.h> // SparkFunMLX90632 Arduino library

#define trigPin 2  //defines the trigger pin of the ultrasonic sensor
#define echoPin 3  //defines the echo pin of the ultrasonic sensor
#define IRpin 0    //defines the analog output of the IR distance sensor

IRTherm therm; // Create an IRTherm object to interact with throughout

void setup() {
  Serial.begin(9600);         // Initialize Serial to log output
  pinMode(trigPin, OUTPUT);   //Sets the trigger pin to an output
  pinMode(echoPin, INPUT);    //Sets the echo pin to an intput

  therm.begin();           // Initialize thermal IR sensor
  therm.setUnit(TEMP_F);   // Set the library's units to Farenheit
  // Alternatively, TEMP_F can be replaced with TEMP_C for Celsius or
  // TEMP_K for Kelvin.
}

void loop() {
  printultrasonic(pulseultrasonic()); //calls the function to read out the ultrasonic sensor
  printIRdistance(IRdistance());      //calls the function to read out the IR distance sensor
  tempprint();                        //calls the function to readout the IR temperature sensor
  Serial.println();                   //adds a line between measurements
  delay(1000);                        //delays each measurement by time in ms
}



void tempprint() {
  if (therm.read()) {                           //If the temperature sensor is reading
    float ambienttemp = therm.ambient();        //take the ambient temp
    float objecttemp = therm.object();          //take the object temp
    float deltatemp = objecttemp - ambienttemp; //subtract the two
    if (deltatemp > 3) {
      Serial.println("Presence Detected");
    } else {
      Serial.println("Presence Not Detected");
    }
  }
}

float pulseultrasonic() {                     //tells the ultrasonic sensor to pulse
  float duration;
  float distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1 / 2.54;
  return distance;                            //reads back the ultrasonic distance in inches
}

void printultrasonic(float distance) {         //prints out the ultrasonic height
  if (distance >= 4000 || distance <= 0) {
    Serial.println("Out of range");
  }
  else {
    Serial.print("Desk height: ");
    Serial.print(distance);
    Serial.println(" in");
  }
}

float IRdistance() {                              //tells the IR distance sensor to read data
  float sensorValue, inches;
  sensorValue = analogRead(IRpin);
  inches = 4192.936 * pow(sensorValue, -0.935) - 3.937;
  return inches;                                  //reads back the IR distance in inches
}

void printIRdistance(float distance) {            //prints out the IR distance
  Serial.print("Monitor height: ");
  Serial.print(distance);
  Serial.println(" in");
}
