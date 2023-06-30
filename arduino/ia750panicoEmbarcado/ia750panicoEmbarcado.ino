#include "arduino_secrets.h"
#include "thingProperties.h"
byte controle=0;
void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,1);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(0);
  ArduinoCloud.printDebugInfo();
  botao=true;
}
void loop(){
  ArduinoCloud.update();
  if(botao==false&&controle==0){
    if(ArduinoCloud.connected()==1){
      botao=true;
      controle=1;
      digitalWrite(LED_BUILTIN,0);
    }
  }
}
void onBotaoChange(){
  if(botao==false&&controle==1){
    controle=0;
    digitalWrite(LED_BUILTIN,1);
    ESP.deepSleep(0);
  }
}
