#include <AutoPID.h>
#include <SoftwareSerial.h>
//motores
#define ENA 3 //izquierdo
#define IN1 5 // HIGH
#define IN2 6
#define IN3 7
#define IN4 8 // HIGH
#define ENB 9 //derecho

#define velBase 71

//modulo bluetooth

SoftwareSerial BT(10, 11);

//Constructor PID
  double PIDinput ;
  double PIDsetpoint = 780;
  double PIDoutput ;
  double outputMin = -60;
  double outputMax = 30;
//valores proporcionales--------------------------------------------------
  double     Kp =   10  ;//+-0.1
  double     Ki =   0.45  ;//+-0.001
  double     Kd =   8    ; //+-1
  //valores proporcionales--------------------------------------------------
 AutoPID pid(&PIDinput, &PIDsetpoint, &PIDoutput, 
          outputMin, outputMax, Kp, Ki, Kd);


//setup---------------------------------------------------------------------
void setup() {
         
          Serial.begin(9600);
            digitalWrite(5, HIGH);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            digitalWrite(8, HIGH);
            
  
   PIDsetpoint = (analogRead(3)+analogRead(4)-10)/2;
  AutoPID pid(&PIDinput, &PIDsetpoint, &PIDoutput, //Constructor PID
          outputMin, outputMax, Kp, Ki, Kd);
  pid.setTimeStep(10);

         
          
          }
  


void loop(){
  int aux=0;
 if (BT.available()){
  // if text arrived in from BT serial...
    if(aux==0){
    a=(BT.read());
    Kp=a;
    }else if(aux==1){
      a=(BT.read());
    Kd=a;
    }else{
      a=(BT.read());
    Ki=a;
    }
    aux++;
   }
//while(1){
 // Serial.print("1-: ");
  //Serial.print(analogRead(0));
  //Serial.print(" 2-: ");
  //Serial.print(analogRead(1));
  //Serial.print(" 3-: ");
  //Serial.print(analogRead(2));
  //Serial.print(" 4-: ");
  //Serial.println(analogRead(3));
//}
  if(analogRead(0)>analogRead(2)){//carril exterior
   // Serial.print("carril exterior");
   
 if(analogRead(0)<650 || analogRead(1)<600){//giro extremo izquierda
  analogWrite(3,0);
  analogWrite(10, 130);//der
  
  
}

else if(analogRead(3)<650 || analogRead(2)<650){//giro extremo derecha
  analogWrite(3, 110);//izq
  analogWrite(10, 0);//der
}else{
 PIDinput = (analogRead(1)+analogRead(2))/2;
 pid.run();
  //Serial.print(PIDoutput);
  //Serial.print("  -----/   ");


 if(PIDoutput >= 0){
  analogWrite(3, velBase-PIDoutput);
  analogWrite(10, velBase+PIDoutput);
  //Serial.print("pid>0, motor izq:");
  //Serial.print(velBase+PIDoutput);
  //Serial.print("pid>0, motor derecho:");
  //Serial.println(velBase-PIDoutput);
 }else{
  analogWrite(3, velBase-PIDoutput);
  analogWrite(10, velBase+PIDoutput);
  //Serial.println(velBase-PIDoutput);
 }

}
 }else{
    Serial.print("carril interior");

 if(analogRead(3)<640){//giro extremo izquierda
  analogWrite(3, 0);//izq
  analogWrite(10, 120);//der
}else if(analogRead(0)<600 ){//giro extremo derecha
  analogWrite(3, 120);//izq
  analogWrite(10, 0);//der
}else{
    
   PIDinput = (analogRead(1)+analogRead(2))/2;
 pid.run();
 Serial.print("1-: ");
  Serial.print(analogRead(0));
  Serial.print(" 2-: ");
  Serial.print(analogRead(1));
  Serial.print(" 3-: ");
  Serial.print(analogRead(2));
  Serial.print(" 4-: ");

  

Serial.print(PIDoutput);
Serial.print("  -----/   ");
 if(PIDoutput >= 0){
   analogWrite(3, velBase);
  analogWrite(10, velBase+PIDoutput);
  Serial.println(velBase+PIDoutput);
 }else{
 analogWrite(3, velBase+PIDoutput);
  analogWrite(10, velBase);
  Serial.println(velBase-PIDinput);
 }

 }
 }

}
