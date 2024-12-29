#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
const int soilMoisturePin = A0;
const int numReadings = 20;
float moistureReadings[numReadings];
float moisturePercentage=0;
RF24 radio(D1,D2);  // CE and CSN pins
const uint64_t pipe3 = 0xF0F0F0F0AA;
const uint8_t channel = 100;  // Set the desired RF channel
struct sensor
{
  int soil1;
  byte  sensorNum;
};
sensor soilreading3;
void setup() {
  pinMode(soilMoisturePin,INPUT);
  radio.begin();
  radio.openWritingPipe(pipe3);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_MAX); // Adjust power level if needed
  radio.setDataRate(RF24_250KBPS);//RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  Serial.begin(9600);
  soilreading3.sensorNum =3;
  Serial.println("Transmitter3 setup complete");

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
   moisturePercentage = map(averageMoisture,800,1024, 100, 0);
   soilreading3.soil1=moisturePercentage ;
    radio.write(&soilreading3, sizeof(soilreading3));
 Serial.print("Average Soil Moisture3: ");
  Serial.print(moisturePercentage);
  Serial.println("%");
   
}

