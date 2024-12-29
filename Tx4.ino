#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
const int soilMoisturePin = A0;
const int numReadings = 20;
float moistureReadings[numReadings];
float moisturePercentage=0;
RF24 radio(D1,D2);  // CE and CSN pins
const uint64_t pipe4 = 0xF0F0F0F0A5;
const uint8_t channel = 100;  // Set the desired RF channel
struct sensor
{
  int soil1;
  byte  sensorNum;
};
sensor soilreading4;
void setup() {
    pinMode(soilMoisturePin,INPUT);
  radio.begin();
  radio.openWritingPipe(pipe4);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_MAX);  // Adjust power level if needed
  radio.setDataRate(RF24_250KBPS);//RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  Serial.begin(9600);
  soilreading4.sensorNum =4;
  Serial.println("Transmitter4 setup complete");

}
void loop() {
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
   moisturePercentage = map(averageMoisture,640,1024, 100, 0);
   soilreading4.soil1=moisturePercentage ;
    radio.write(&soilreading4, sizeof(soilreading4));
 Serial.print("Average Soil Moisture4: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
   
}

