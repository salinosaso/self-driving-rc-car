//Código 29/5/19
//con los ENA y ENB modificamos la velocidad con PWM, los pines son arbitrarios, puestos
//como ejemplo para este código.
#define ENA 3
#define IN1 11 // HIGH sentido de giro
#define IN2 10 // LOW "
#define IN3  9//LOW "
#define IN4 8 // HIGH "
#define ENB 5 //derecho pwm

//echo - 6
//trigger - 7

// defines pins numbers ----------------------------------------------------------------------

int inputUltrasonido = A0;
int outputUltrasonido = A5;
int distanciaUltrasonido = 0;

/* SENSOR ------------------------------------------------------------------------------------*/
//Tenemos un sensor de pololu qtr8A que nos deja ver la diferencia en blanco y negro de la pista
//se recomienda tenerlos a un maximo de 1cm de altura para valores exactos.
// cuanto mas negro mas cercano a 0, y cuando mas blanco mas cercano a 1023
#define GIRO_EXTREMO_DERECHA_THRESHOLD 700 // 700
#define GIRO_EXTREMO_IZQUIERDA_THRESHOLD 500 // 500
#define GIRO_EXTREMO_DERECHA_THRESHOLD_WHILE 715 // 715
#define SALIR_POR_LA_DERECHA_SENSOR_0_THRESHOLD 470 // 500
#define SALIR_POR_LA_DERECHA_SENSOR_4_THRESHOLD 810 // 835
#define SALIR_POR_LA_DERECHA_FUERTE_SENSOR_4_THRESHOLD 850 // 870
#define SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_THRESHOLD 725 // 725
#define SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_FUERTE_THRESHOLD 710 // 710

#define INT_GIRO_EXTREMO_IZQUIERDA_THRESHOLD 665
#define INT_GIRO_EXTREMO_DERECHA_THRESHOLD 354
#define INT_SALIR_POR_LA_IZQUIERDA_SENSOR_0_THRESHOLD 710 // 500
#define INT_SALIR_POR_LA_IZQUIERDA_SENSOR_4_THRESHOLD 710 // 835
#define INT_SALIR_POR_LA_IZQUIERDA_FUERTE_SENSOR_4_THRESHOLD 759 // 870
#define INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_SENSOR_0_THRESHOLD 540 // 725
#define INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_FUERTE_SENSOR_0_THRESHOLD 505 // 710
#define INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_SENSOR_4_THRESHOLD 370 // 725
#define INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_FUERTE_SENSOR_4_THRESHOLD 365 // 710

/* MOTOR ---------------------------------------------------------------------------------------*/
// A través del pwm le damos mayor o menor potencia a los motores
#define GIRO_EXTREMO_DERECHA_A 70 // 90
#define GIRO_EXTREMO_DERECHA_B 25 // 40
#define GIRO_EXTREMO_IZQUIERDA_A 25 // 20
#define GIRO_EXTREMO_IZQUIERDA_B 75 // 80

#define GIRO_IZQUIERDA_A 25 // 20
#define GIRO_IZQUIERDA_B 55 // 55 (70 nuevo)

#define GIRO_IZQUIERDA_FUERTE_A 15 // 10
#define GIRO_IZQUIERDA_FUERTE_B 55// 85

#define GIRO_DERECHA_A 55 // 50
#define GIRO_DERECHA_B 25 // 10

#define GIRO_DERECHA_FUERTE_A 60 // 68
#define GIRO_DERECHA_FUERTE_B 30 // 10

#define RECTO_A 50 // 45 (45-50)
#define RECTO_B 50// 50

#define DEBUG false
//sensores------------------------------------------------------------------------------------------
int sensor[6];

void lee_Sensor(int*);
void escribe_Sensor(int*);
void mantener_Carril_Dextrogiro(int*);
//bool ultraSound();

void setup() {
  // put your setup code here, to run once:
  /*pinMode(inputUltrasonido, INPUT);
  pinMode(outputUltrasonido, OUTPUT);*/
  
  pinMode (ENA, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  pinMode (IN3, OUTPUT);
  pinMode (IN4, OUTPUT);
  pinMode (ENB, OUTPUT);


  Serial.begin(9600);
  
}

void loop() {
  //los IN1,IN2,IN3, IN4 nos indican la polaridad del motor, al invertirla, irá en la dirección contraria
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);
  lee_Sensor(sensor);
  #if DEBUG
    if(Serial.read()) {
      escribe_Sensor(sensor);
    }
  #else

   if( (sensor[0] <= GIRO_EXTREMO_DERECHA_THRESHOLD) || (sensor[1] <= GIRO_EXTREMO_DERECHA_THRESHOLD)){
      int a0 = sensor[0];
      int a1 = sensor[1];
       //Serial.println("Giro extremo derecha");
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
          if((a0 <= GIRO_EXTREMO_DERECHA_THRESHOLD_WHILE ) || (a1 <= GIRO_EXTREMO_DERECHA_THRESHOLD_WHILE)){
            a0 = analogRead(0);
            a1 = analogRead(1);
            analogWrite(ENA, GIRO_EXTREMO_DERECHA_A);
            analogWrite(ENB, GIRO_EXTREMO_DERECHA_B);
          }
        digitalWrite(IN3, 1);
        digitalWrite(IN4, 0);
    }
    else if((sensor[3] <= GIRO_EXTREMO_IZQUIERDA_THRESHOLD) ){
          int a3 = sensor[3];
          digitalWrite(IN1, HIGH);
          digitalWrite(IN2, LOW);
          //Serial.println("Giro extremo izquierda");
          if(a3 < GIRO_EXTREMO_IZQUIERDA_THRESHOLD){
            a3 = analogRead(3);
            analogWrite(ENA, GIRO_EXTREMO_IZQUIERDA_A);
            analogWrite(ENB, GIRO_EXTREMO_IZQUIERDA_B);
          }
          digitalWrite(IN1, LOW);
          digitalWrite(IN2, HIGH); 
    }  else{
      mantener_Carril_Dextrogiro(sensor);
    }
    
  #endif
}


void lee_Sensor(int * sensor){
      sensor[0] = analogRead(1);
      sensor[1] = analogRead(2);
      sensor[2] = analogRead(3);
      sensor[3] = analogRead(4);
      //sensor[4] = analogRead(4);
     // sensor[5] = analogRead(5);
}

void mantener_Carril_Dextrogiro(int* sensor){
  if(analogRead(0)>analogRead(4)+70) {
          // CARRIL INTERIOR
          
          if((sensor[3] > INT_SALIR_POR_LA_IZQUIERDA_SENSOR_4_THRESHOLD) || (sensor[0] > INT_SALIR_POR_LA_IZQUIERDA_SENSOR_0_THRESHOLD)){//estamos saliendo por la izquierda(rojo y blanco)
          analogWrite(ENA,GIRO_DERECHA_A);
          analogWrite(ENB,GIRO_DERECHA_B);
        //Serial.println("Giro derecha");
          if(sensor[3] > INT_SALIR_POR_LA_IZQUIERDA_FUERTE_SENSOR_4_THRESHOLD){
            //Serial.println("Giro derecha f");
            analogWrite(ENA,GIRO_DERECHA_FUERTE_A);
            analogWrite(ENB,GIRO_DERECHA_FUERTE_B);
          }
        }
        else if((sensor[0] <= INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_SENSOR_0_THRESHOLD) || (sensor[4] <= INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_SENSOR_4_THRESHOLD)){//estamos saliendo por la derecha(otro carril)
          
          analogWrite(ENA,GIRO_IZQUIERDA_A);
          analogWrite(ENB,GIRO_IZQUIERDA_B);
         //Serial.println("Giro izquierda");
          if((sensor[0] <= INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_FUERTE_SENSOR_0_THRESHOLD) || (sensor[4] <= INT_SALIR_POR_LA_DERECHA_OTRO_CARRIL_FUERTE_SENSOR_4_THRESHOLD)){
            analogWrite(ENA,GIRO_IZQUIERDA_FUERTE_A);
            analogWrite(ENB,GIRO_IZQUIERDA_FUERTE_B);
           //Serial.println("Giro izquierda f");
          }
        }
        else{ // estamos dentro del carril
          analogWrite(ENA, RECTO_A);
          analogWrite(ENB, RECTO_B);
          //Serial.println("recto");
        }
  } else {
          // CARRIL EXTERIOR
          if((sensor[4] > SALIR_POR_LA_DERECHA_SENSOR_4_THRESHOLD) || (sensor[0] <= SALIR_POR_LA_DERECHA_SENSOR_0_THRESHOLD)){//estamos saliendo por la derecha(rojo y blanco)
          analogWrite(ENA,GIRO_IZQUIERDA_A);
          analogWrite(ENB,GIRO_IZQUIERDA_B);
          //Serial.println("Giro izquierda");
          if(sensor[4] > SALIR_POR_LA_DERECHA_FUERTE_SENSOR_4_THRESHOLD){
            //Serial.println("Giro izquierda f");
            analogWrite(ENA,GIRO_IZQUIERDA_FUERTE_A);
            analogWrite(ENB,GIRO_IZQUIERDA_FUERTE_B);
          }
        }
        else if((sensor[0] <= SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_THRESHOLD) || (sensor[1] <= SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_THRESHOLD-117)){//estamos saliendo por la izquierda(otro carril)
          analogWrite(ENA,GIRO_DERECHA_A);
          analogWrite(ENB,GIRO_DERECHA_B);
          //Serial.println("Giro derecha");
          if((sensor[0] <= SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_FUERTE_THRESHOLD) || (sensor[1] <= SALIR_POR_LA_IZQUIERDA_OTRO_CARRIL_FUERTE_THRESHOLD-117)){
            analogWrite(ENA,GIRO_DERECHA_FUERTE_A);
            analogWrite(ENB,GIRO_DERECHA_FUERTE_B);
           //Serial.println("Giro derecha f");
          }
        }
        else{ // estamos dentro del carril
          analogWrite(ENA, RECTO_A);
          analogWrite(ENB, RECTO_B);
          //Serial.println("recto");
        }
  }
  
}

void escribe_Sensor(int*sensor){
  //Serial.println("");
 /* if(analogRead(0)>analogRead(4)+70) {
    digitalWrite(13,HIGH);
    Serial.println("interior");
  } else {
    digitalWrite(13,LOW);
    Serial.println("exterior");
  }*/
  /*Serial.println(analogRead(0)>analogRead(4)+70);
  for(int i = 0; i<6; i++){
      Serial.print(sensor[i]);
      Serial.print("-");
    }*/
  delay(1000);
}
