#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
RF24 radio(D1,D2);  // CE and CSN pins
const uint64_t pipe1 = 0xF0F0F0F065;
const uint64_t pipe2 = 0xF0F0F0F066;
const uint64_t pipe3 = 0xF0F0F0F0AA;
const uint64_t pipe4 = 0xF0F0F0F0A5;
const uint8_t channel = 100;  // Set the desired RF channel
float soilreading1=0;
float soilreading2=0;
float soilreading3=0;
float soilreading4=0;
float avg=0;
int motor=D0;

//blynk
// #define BLYNK_TEMPLATE_ID "TMPL3qFe28FqU"
// #define BLYNK_TEMPLATE_NAME "single"
//#define BLYNK_AUTH_TOKEN "cwsMWLMwGbP3KXJDxZ0oGIJCLj9Vl7JD"
#define BLYNK_TEMPLATE_ID "TMPL3wLBHFZvV"
#define BLYNK_TEMPLATE_NAME "soil moisture"
#define BLYNK_AUTH_TOKEN "trH3syjbp5tWPbaudtRPCJxBwAOpcGdn"
#include<ESP8266WiFi.h>
#include<BlynkSimpleEsp8266.h>
char auth[]=BLYNK_AUTH_TOKEN;
char wifi[]="One Plus 11R";
char wifipass[]="shivam12345"; 
BlynkTimer timer;
BLYNK_WRITE (V1){
soilreading1 = param.asInt( ) ;
}
BLYNK_WRITE (V2){
soilreading2 = param.asInt( ) ;
}
BLYNK_WRITE (V3){
soilreading3 = param.asInt( ) ;
}
BLYNK_WRITE (V4){
soilreading4 = param.asInt( ) ;
}
BLYNK_WRITE (V5){
avg = param.asInt( ) ;
}
BLYNK_WRITE (V0){
 int value = param.asInt( ) ;
 if(value==1)
 {
 digitalWrite(motor,HIGH);
 }
 else{
  digitalWrite(motor,LOW);
 }
}
void mytimer(){
  Blynk.virtualWrite(V1,soilreading1);
  Blynk.virtualWrite(V2,soilreading2);
   Blynk.virtualWrite(V3,soilreading3);
  Blynk.virtualWrite(V4,soilreading4);
    Blynk.virtualWrite(V5,avg);
     Blynk.virtualWrite(V6,avg);
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
     radio.openReadingPipe(3, pipe3);
      radio.openReadingPipe(4, pipe4);
  radio.setChannel(channel);   // Set the RF channel
  radio.setPALevel(RF24_PA_MAX);  // Adjust power level if needed
   radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  Serial.begin(9600);
   Blynk.begin(auth,wifi,wifipass);
    timer.setInterval(100L, mytimer);
    pinMode(motor,OUTPUT);
  Serial.println("Receiver setup complete");
}

void loop() {
  if(radio.available())
  {
    radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
    delay(50);
    radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
    delay(50);
     radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
    delay(50);
     radio.read(&sensorData, sizeof(sensorData));
    disp_sensor_data();
    delay(50);
  }

    if(soilreading1!=0 and soilreading2!=0 and soilreading3!=0 and soilreading4!=0  ){
    avg=(soilreading1+soilreading2+soilreading3+soilreading4)/4;
  Serial.print("avg=  ");
  Serial.println(avg);
    }
   Blynk.run();
    timer.run();
    delay(100);
    if(avg<40){
       digitalWrite(motor,1);
    }
    if(avg >60){
      digitalWrite(motor,0);
    }
}
void disp_sensor_data()
{
  if(sensorData.sensorNum == 1)
  {
    soilreading1=sensorData.soil;
 Serial.print("1 soil data=  ");
    Serial.println(sensorData.soil);
  }
  if(sensorData.sensorNum == 2)
  {
    soilreading2=sensorData.soil;
   Serial.print("2 soil data=  ");
   Serial.println(sensorData.soil);
  }
   if(sensorData.sensorNum == 3)
  {
    soilreading3=sensorData.soil;
   Serial.print("3 soil data=  ");
   Serial.println(sensorData.soil);
  }
  if(sensorData.sensorNum == 4)
  {
    soilreading4=sensorData.soil;
   Serial.print("4 soil data=  ");
   Serial.println(sensorData.soil);
  }
}