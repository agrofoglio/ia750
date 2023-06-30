#include "arduino_secrets.h"
#include "thingProperties.h"
#include <Adafruit_BMP085.h>
#include <DHT_U.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>
#define DHTTYPE DHT11
#define DHTPIN  0
#define pir 15
#define tempoEnvio 30000
WiFiUDP udp;
NTPClient ntp(udp,"a.st1.ntp.br",-3*3600,60000);
DHT_Unified dht(DHTPIN,DHTTYPE);
Adafruit_BMP085 bmp;
unsigned long cronometro=millis();
void setup(){
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(pir,INPUT);
  dht.begin();
  if(!bmp.begin()){
    digitalWrite(LED_BUILTIN,1);
    while(1){}
  }
  else
    digitalWrite(LED_BUILTIN,0);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  ntp.begin();
  ntp.forceUpdate();
  ArduinoOTA.setHostname("");
  ArduinoOTA.setPassword("");
  ArduinoOTA.begin();
}
void loop(){
  ArduinoOTA.handle();
  ArduinoCloud.update();
  if(ArduinoCloud.connected()){
    presenca=digitalRead(pir);
    if(presenca)
      digitalWrite(LED_BUILTIN,0);
    else
      digitalWrite(LED_BUILTIN,1);
  }
  else{
    digitalWrite(LED_BUILTIN,1);
    if(millis()-cronometro>=60000)
      ESP.restart();
  }
  if(millis()-cronometro>=tempoEnvio){
    ntp.forceUpdate();
    hora=ntp.getFormattedTime();
    digitalWrite(LED_BUILTIN,1);
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    dht.humidity().getEvent(&event);
    umidade11=event.relative_humidity;
    if(isnan(umidade11)){
      digitalWrite(LED_BUILTIN,1);
      cronometro=millis();
      return;
    }
    else
      digitalWrite(LED_BUILTIN,0);
    temperatura180=bmp.readTemperature();
    pressao180=bmp.readPressure();
    digitalWrite(LED_BUILTIN,0);
    cronometro=millis();
  }
}
