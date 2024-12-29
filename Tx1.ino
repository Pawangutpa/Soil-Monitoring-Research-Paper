#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
const int soilMoisturePin = A0;
const int numReadings = 20;
float moistureReadings[numReadings];
float moisturePercentage=0;
RF24 radio(D1,D2);  // CE and CSN pins
//const uint64_t pipe1 = 0xF0F0F0F0AA;
const uint64_t pipe1 = 0xF0F0F0F065;
const uint8_t channel = 100;  // Set the desired RF channel
struct sensor
{
  int soil1;
  byte  sensorNum;
};
sensor soilreading1;
void setup() {
  pinMode(soilMoisturePin,INPUT);
  radio.begin();
  radio.openWritingPipe(pipe1);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_MAX);  // Adjust power level if needed
  radio.setDataRate(RF24_250KBPS);
  Serial.begin(9600);
  soilreading1.sensorNum = 1;
  Serial.println("Transmitter1 setup complete");
}
void loop() {
   //delay(500);
  float totalMoisture = 0;
  // Read and accumulate multiple readings
  for (int i = 0; i < numReadings; i++) {
    moistureReadings[i] = analogRead(soilMoisturePin);
    totalMoisture += moistureReadings[i];
    delay(50); // Add a short delay between readings
  }
  Serial.println(analogRead(soilMoisturePin));
  // Calculate the average
  float averageMoisture = totalMoisture / numReadings;
  // Map the average to a percentage
   moisturePercentage = map(averageMoisture,661,1024, 100, 0);
   soilreading1.soil1=moisturePercentage ;
    radio.write(&soilreading1, sizeof(soilreading1));
 Serial.print("Average Soil Moisture1: ");
  Serial.print(moisturePercentage);
  Serial.println("%");

   
}

