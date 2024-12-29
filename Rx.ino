#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
RF24 radio(D1,D2);  // CE and CSN pins
const uint64_t pipe1 = 0xF0F0F0F0AA;
const uint64_t pipe2 = 0xF0F0F0F066;
const uint8_t channel = 100;  // Set the desired RF channel
float soilreading1=22;
float soilreading2=22;
float avg=0;
const int numReadings1 = 20;
float moistureReadings1[numReadings1];
const int numReadings2 = 20;
float moistureReadings2[numReadings2];
float totalavg=0;
float averageMoisture1=0;
float averageMoisture2=0;
//blynk
#define BLYNK_TEMPLATE_ID "TMPL3qFe28FqU"
#define BLYNK_TEMPLATE_NAME "single"
#define BLYNK_AUTH_TOKEN "cwsMWLMwGbP3KXJDxZ0oGIJCLj9Vl7JD"
#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
char auth[]=BLYNK_AUTH_TOKEN;
char wifi[]="One Plus 11R";
char wifipass[]="shivam12345"; 
BlynkTimer timer;
BLYNK_WRITE (V5){
soilreading1 = param.asInt( ) ;
}
BLYNK_WRITE (V4){
soilreading2 = param.asInt( ) ;
}
BLYNK_WRITE (V6){
totalavg = param.asInt( ) ;
}
void mytimer(){
  Blynk.virtualWrite(V5,soilreading1);
  Blynk.virtualWrite(V4,soilreading2);
    Blynk.virtualWrite(V6,totalavg);
}
struct sensor
{
  int soil;
  byte  sensorNum;
};
sensor sensorData;
void setup() {
  radio.begin();
  radio.openReadingPipe(1, pipe1);
    radio.openReadingPipe(2, pipe2);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_MAX);  // Adjust power level if needed
   radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  Serial.begin(9600);
   Blynk.begin(auth,wifi,wifipass);
    timer.setInterval(1000L, mytimer);
    pinMode(D0,OUTPUT);
  Serial.println("Receiver setup complete");
}

void loop() {
  if(radio.available())
  {
    radio.read(&sensorData, sizeof(sensorData));
   // disp_sensor_data();
    radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
  }
   Blynk.run();
    timer.run();
    avg=(soilreading1+soilreading2)/2;
  Serial.print("avg=  ");
  Serial.println(avg);
 
  // //for second
   
  //  totalavg=(averageMoisture2+averageMoisture1)/2;
  // Serial.print("total avg is = ");
  // Serial.println(totalavg);
  delay(1000);
}
void disp_sensor_data()
{
  if(sensorData.sensorNum == 1)
  {
 Serial.print("1 soil data=  ");
    Serial.println(averageMoisture1);
  }
  if(sensorData.sensorNum == 2)
  {
    soilreading2=sensorData.soil;
   Serial.print("2 soil data=  ");
   Serial.println(averageMoisture2);
  }
}
