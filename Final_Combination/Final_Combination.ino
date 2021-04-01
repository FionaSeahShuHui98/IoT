//Adding Library
#include <SPI.h>
#include "DHT.h"
#include <LoRa.h>

//Defining Pins
#define DHTPIN A0   //Temperature Sensor - Data
#define trigPin A5  //Ultrasonic Sensor - Trigger
#define echoPin A4  //Ultrasonic Sensor - Echo
#define DHTTYPE DHT11   // DHT 11

//Declaration
float duration;
float distance;

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  //Declaring the pin mode - input or output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  dht.begin();

  //Setting LoRa Frequency
  if (!LoRa.begin(916E6)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() 
{
  // Reading of humidity sensor
  float humidity = dht.readHumidity();
  // Reading of temperature sensor in celcisus
  float temperature = dht.readTemperature();

  // To check if any reading fails
  if (isnan(humidity) || isnan(temperature)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Ultrasonic Sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*3.43)/2;
  
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("C ");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm");
  Serial.print('\n');
  
  //Starting to transmit data
  LoRa.beginPacket();
  LoRa.print("Humidity: ");
  LoRa.print(humidity);
  LoRa.println("%");
  LoRa.print("Temperature: ");
  LoRa.print(temperature);
  LoRa.println("C ");
  LoRa.print("Distance: ");
  LoRa.print(distance);
  LoRa.print(" cm");
  LoRa.print('\n');
  LoRa.endPacket();

  // Wait a few seconds between measurements.
  delay(2000);
}
