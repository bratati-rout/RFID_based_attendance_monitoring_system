#include <Keyboard.h>
#define analogPin 0

char keys[] = {'1','2','3','4','5','6','7','8','9','*','0','#'};
int keyVals[] = {1016,1013,990,920,923,903,851,846,831,789,782,769};
int threshold = 700;
int range = 2;
unsigned long lastValue=millis();
int debounceDelay = 450;

void setup(){
  Serial.begin(9600);
  while(!Serial);
}

void loop(){
  
  int val = analogRead(analogPin);
  /*if(val>0 && (millis()-lastValue >= debounceDelay)){
      for(int i=0;i<12;i++){
        if(val>= keyVals[i]-range && val<=keyVals[i]+range){
          Serial.print(keys[i]);
          lastValue = millis();
          break;
        }
      }
    }*/
    if(val>threshold){
    Serial.println(val);
    delay(100);
    }
}
