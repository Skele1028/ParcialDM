#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(10, 11);   // Definimos los pines RX y TX del Arduino conectados al Bluetooth

bool calibration;
int enable1 = 6;
int enable2 = 7;

int motor1I = 11; //Motor Horizontal Izquierda
int motor1D = 10; //Motor Horizontal Derecha
int motor2U = 9; //Motor Vertical Arriba
int motor2D = 8; //Motor Vertical Abajo

int final1 = A0; //Izq
int final2 = A1; //Der
int final3 = A2; //Up
int final4 = A3; //Down

//Valores normales para lectura maxima
int valorFinal1 = 30540; //Izq
int valorFinal2 = 30540; //Der
int valorFinal3 = 30540; //Up
int valorFinal4 = 30540; //Down

int tx;
int ty;

int x;
int y;

void setup() {
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  Serial.begin(9600);   // Inicializamos  el puerto serie

  pinMode(enable1, OUTPUT);
  pinMode(enable2, OUTPUT);

  pinMode(motor1I, OUTPUT);
  pinMode(motor1D, OUTPUT);
  pinMode(motor2U, OUTPUT);
  pinMode(motor2D, OUTPUT);

  pinMode(final1, INPUT);
  pinMode(final2, INPUT);
  pinMode(final3, INPUT);
  pinMode(final4, INPUT);

  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);

  Serial.begin(9600);

  calibration = calibrationMotor();

  Serial.println("Todo listo");
}

bool calibrationMotor() {
  //Bajamos motores
  digitalWrite(motor1I, LOW);
  digitalWrite(motor1D, LOW);
  digitalWrite(motor2U, LOW);
  digitalWrite(motor2D, LOW);

  //
  tx = 0;
  ty = 0;

  //Llevamos los carros hasta el final
  int valor = 0;
  while (valor < valorFinal2) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final2);
    }
    Serial.print(valor);
    Serial.println("Buscando final Derecha");
    digitalWrite(motor1D, HIGH);
  }
  digitalWrite(motor1D, LOW);

  valor = 0;
  while (valor < valorFinal4) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final4);
    }
    Serial.print(analogRead(final4));
    Serial.println("Buscando final Abajo");
    digitalWrite(motor2D, HIGH);
  }
  digitalWrite(motor2D, LOW);

  //Leemos finales 1
  int temp = millis();
  valor = 0;
  while (valor < valorFinal1) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final1);
    }
    Serial.print(analogRead(final1));
    Serial.println("Buscando final Izquierda");
    digitalWrite(motor1I, HIGH);
  }
  digitalWrite(motor1I, LOW);
  int tx = millis() - temp;

  //Leemos finales 2
  temp = millis();
  valor = 0;
  while (valor < valorFinal3) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final3);
    }
    Serial.print(analogRead(final3));
    Serial.println("Buscando final Arriba");
    digitalWrite(motor2U, HIGH);
  }
  digitalWrite(motor2U, LOW);
  int ty = millis() - temp;

  Serial.print("tx = ");
  Serial.println(tx);

  Serial.print("ty = ");
  Serial.println(ty);

  Serial.println("Calibración terminada");

  //Verificamos calculos:
  if (tx > ty) {
    return true;
  }

  return false;
}

String readBluetooth() {
  // Si llega un dato por el puerto BT se envía al monitor serial
  if (BT.available()) {
    String coordenada = BT.readString();

    // Leemos longitud
    Serial.println(coordenada);
    int longitud = coordenada.length() + 1 ;

    // Mostramos longitud
    Serial.println(longitud);

    //Creamos arreglo y pasamos caracteres
    char cadCoor[longitud];
    coordenada.toCharArray(cadCoor, longitud);

    //Asignación de valores
    x = atol(cadCoor[0]);
    y = atol(cadCoor[1]);

    //Verificamos coor
    Serial.println("coordeada num");
    Serial.println(coordenada);

    //Mostrar Arreglo
    for (int i = 0; i < longitud; i++) {
      Serial.println(cadCoor[i]);
    }

    //Imprimimos posiciones finales
    Serial.println("El valor de: " + x);
    Serial.println("El valor de: " + y);
  }

  //Retornamos el valor del Bluetooth
  return "Hecho";
}

void writeBluetooth(int str) {
  BT.write(str);
}

void moveMotors() {
  if (calibration) {
    // Verificamos valores antes de mover
    if (x < 5 && y < 4) {
      // Inicio
      int start = millis();

      //Movemos motor X
      int now = 0;
      while (now - start < tx / x) {
        digitalWrite(motor1D, HIGH);
        now = millis();
      }

      // Inicio
      start = millis();

      //Movemos motor Y
      now = 0;
      while (now - start < ty / y) {
        digitalWrite(motor2D, HIGH);
        now = millis();
      }

      delay(5000);
      calibration = calibrationMotor();w
    }
  }
}

void loop() {
}
