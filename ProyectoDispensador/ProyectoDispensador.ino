// Dispensador Automatizado de Agua 
// Hecho por: Nicolás Rivera Smith-Palliser   &   Jefferson Vargas Reynosa                          * Ver referencia al final del código
///////////////////////////////////////////////////////////

//Incluir la librería del motor servo
#include <Servo.h> //librería para la librería servo
//Se declara el pin del motor
const int servo_pin = 9;
Servo servo;

// Incluimos la libreria de la pantalla LCD
#include <LiquidCrystal.h>
// Definimos las constantes
#define COLS 16 // Columnas del LCD
#define ROWS 2 // Filas del LCD
// Lo primero es inicializar la librería indicando los pines de la interfaz
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // (RS, EN, D4, D5, D6, D7)

//Se declaran los pines del sensor ultrasónico
int trigger = 11;           
int echo = 10;          
#define MAX_DISTANCE 200
float tiempo_de_espera,distancia; //Se crea una flotante para dar una distancia con decimales.

int Buzzerpin = 8; // pin del buzzer

int led = 12;   // Pin del LED

int boton = 13;   // Pin del botón pulsador
int estado = 0;
int salida = 0;
int estado_anterior = 0;    // Variables necesarias para covertir el pulsador en un interruptor

bool ind_parpadeo = 0;  // Indicador booleano para el parpadeo del LED

void setup() {  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin (9600);            // Se inicia el monitor serial
  pinMode (trigger, OUTPUT);      // declarmos el pin 12 como salida
  pinMode (echo, INPUT);          // declaramos el 11 como entrada
  pinMode (Buzzerpin, OUTPUT);    // Se declara el pin del buzzer como salida
  pinMode (led, OUTPUT);          // Se declara el pin del led como salida
  pinMode (boton, INPUT);         // Pin del boton como entrada
  servo.attach(servo_pin);        // el motor estará instalado en un punto en particular  
  lcd.begin(COLS, ROWS);          // Configuramos las filas y columnas del LCD
}

void loop() {  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  estado = digitalRead(boton);

  if (estado == HIGH && estado_anterior == LOW){
    salida = 1 - salida;
    delay (25);
  }
  estado_anterior = estado;

  if (salida == HIGH) {

    //Función para la detección de distancia del sensor.
    delay (200);
    digitalWrite (trigger,LOW); 
    delayMicroseconds(2);
    digitalWrite (trigger, HIGH);
    delayMicroseconds (10);    
    digitalWrite (trigger, LOW); 
    tiempo_de_espera = pulseIn (echo,HIGH); 

    //Función para dar la distancia en el monitor serial.
    distancia =(tiempo_de_espera/2)/29.15; // formula para hallar la distancia
    Serial.print (distancia);    // imprimimos la distancia en cm
    Serial.println ("cm");
    delay (100);
    
    if (distancia > 10){ //Si la distancia es mayor a 10cm todo el circuito estará apagado
      ind_parpadeo = 0;
      servo.write(0); //grados
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Aproxime el vaso");
      lcd.setCursor(0,1);
      lcd.print("al sensor");
      parpadeo (ind_parpadeo);
      delay(500);

    }
    else if((distancia >= 4) && (distancia <= 10)){ //Si la distancia está entre 4cm y 10cm el motor girará y el led se encenderá
      lcd.clear();
      lcd.setCursor(0,0);   // Situamos el cursor en la columna 0 fila 0
      ind_parpadeo = 1;
      servo.write(80); //grados
      lcd.print("Sirviendo");        // Escribimos en la pantalla
      parpadeo (ind_parpadeo);
      animacion_puntos ();
    }
    else if(distancia < 4){ //Si la distancia es menor a 4cm el motor se apagará y el buzzer sonará como una alarma que dejará de caer agua.
      ind_parpadeo = 0;
      servo.write(0); //grados
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Retire el vaso");
      delay(500);
      alarma();
      parpadeo (ind_parpadeo);
      delay(500);
    }
  }
  else {
    digitalWrite(led, LOW);
    servo.write(0); //grados
    lcd.clear();
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void parpadeo (bool ind_parpadeo) {   // Función para el parpadeo del LED.
  if (ind_parpadeo == 1){
    digitalWrite(led, HIGH);
    delay (500);
    digitalWrite(led, LOW);
  }
  else if (ind_parpadeo == 0) {
    digitalWrite(led, HIGH);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void alarma (){ //Esta es la función de la alarma que sonará cuando se detenga el motor.
  digitalWrite(Buzzerpin,HIGH);
  delay(250);
  digitalWrite(Buzzerpin,LOW);
  delay(20);
  digitalWrite(Buzzerpin,HIGH);
  delay(100);
  digitalWrite(Buzzerpin,LOW);
  delay(20);
  digitalWrite(Buzzerpin,HIGH);
  delay(250);
  digitalWrite(Buzzerpin,LOW);
}

void animacion_puntos () {
  lcd.setCursor(9,0);   // Situamos el cursor 
  lcd.print(".");        // Escribimos
  delay (300);
  lcd.setCursor(10,0);   // Situamos el cursor 
  lcd.print(".");        // Escribimos
  delay (300);
  lcd.setCursor(11,0);   // Situamos el cursor 
  lcd.print(".");        // Escribimos
  delay (300);
  lcd.setCursor(9,0);   // Situamos el cursor 
  lcd.print("   ");        // Escribimos
}

// Código adaptado para los requerimientos del proyecto. Código original proveniente de: E. Rivas, “Automatizador De Dispensador De Agua,” Instructables, 2020. https://www.instructables.com/Automatizador-De-Dispensador-De-Agua/