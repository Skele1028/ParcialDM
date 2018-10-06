#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(12, 13);   // Definimos los pines TX y RX del Arduino conectados al Bluetooth

bool calibration;
int enable1 = 6; //Entrada de velocidad
int enable2 = 7; //Entrada de velocidad

int motor1I = 11; //Motor Horizontal Izquierda
int motor1D = 10; //Motor Horizontal Derecha
int motor2U = 9; //Motor Vertical Arriba
int motor2D = 8; //Motor Vertical Abajo

int final1 = A0; //Izq
int final2 = A1; //Der
int final3 = A2; //Up
int final4 = A3; //Down

int panicB = A4; //Entrada del boton

//Valores normales para lectura maxima
int maximoFinal1 = 30540; //Izq
int maximoFinal2 = 30540; //Der
int maximoFinal3 = 30540; //Up
int maximoFinal4 = 30540; //Down
int maximoPanic = 30540; //Down

int fragmentoX;
int fragmentoY;

int tx;
int ty;

int x;
int y;

int nX;
int nY;

bool panic;

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

  x = 0;
  y = 0;

  panic = false;

  Serial.println("Maquina iniciada");
}

bool calibrationMotor() {
  //Bajamos motores
  digitalWrite(motor1I, LOW);
  digitalWrite(motor1D, LOW);
  digitalWrite(motor2U, LOW);
  digitalWrite(motor2D, LOW);

  //Medidas de la mesa
  tx = 0;
  ty = 0;

  //Llevamos los carros hasta el final
  int valor = 0;
  while (valor < maximoFinal2) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final2);
    }
    Serial.print("Buscando final Derecha - ");
    Serial.println(valor);
    digitalWrite(motor1D, HIGH);
  }
  digitalWrite(motor1D, LOW);

  valor = 0;
  while (valor < maximoFinal4) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final4);
    }
    Serial.print("Buscando final Abajo - ");
    Serial.println(valor);
    digitalWrite(motor2D, HIGH);
  }
  digitalWrite(motor2D, LOW);

  //Leemos finales 1
  int temp = millis();
  valor = 0;
  while (valor < maximoFinal1) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final1);
    }
    Serial.print("Buscando final Izquierda - ");
    Serial.println(valor);
    digitalWrite(motor1I, HIGH);
  }
  digitalWrite(motor1I, LOW);

  int tx = millis() - temp;

  //Leemos finales 2
  temp = millis();
  valor = 0;
  while (valor < maximoFinal3) {
    valor = 0;
    for (int i = 0; i < 30; i++) {
      valor += analogRead(final3);
    }
    Serial.print("Buscando final Arriba - ");
    Serial.println(valor);
    digitalWrite(motor2U, HIGH);
  }
  digitalWrite(motor2U, LOW);
  int ty = millis() - temp;

  Serial.print("tx = ");
  Serial.println(tx);

  Serial.print("ty = ");
  Serial.println(ty);

  //Verificamos calculos:
  if (tx != 0 && ty != 0) {
    Serial.println("Calibración terminada");
    x = 0;
    y = 0;
    fragmentoX = tx / 4;
    fragmentoY = ty / 2;
    Serial.print("Fragmento X - ");
    Serial.println(fragmentoX);
    Serial.print("Fragmento Y - ");
    Serial.println(fragmentoY);
    writeBluetooth(82);
    return true;
  }

  Serial.println("Calibración fallida");
  return false;
}

String readBluetooth() {
  if (BT.available()) {
    String str = "";
    while (BT.available())  // Si llega un dato por el puerto BT se envía al monitor serial
    {
      str += BT.read();
    }
    Serial.println("Lectura realizada");
    Serial.println(str);

    if (str == "6767") {
      //Calibración del sistema
      Serial.println("Calibración del sistema");
      calibrationUser();
      return  "";
    }

    if (str == "8383") {
      //Calibración del sistema
      Serial.println("Iniciando");
      panic = false;
      return  "";
    }

    if (str == "6868") {
      //Calibración del sistema
      Serial.println("Deteniendo");
      panic = true;
      return  "";
    }

    int longitud = str.length() + 1 ;

    // Mostramos longitud
    Serial.print("Longitud - ");
    Serial.println(longitud);

    //Creamos arreglo y pasamos caracteres
    char cadCoor[longitud];
    str.toCharArray(cadCoor, longitud);

    //Asignación de valores
    nX = ((cadCoor[0] - '0') * 10) + (cadCoor[1] - '0') - 48;
    nY = ((cadCoor[2] - '0') * 10) + (cadCoor[3] - '0') - 48;

    //Verificamos coor
    Serial.print("Coordeada: ");
    Serial.print(nX);
    Serial.print(",");
    Serial.println(nY);

    //Retornamos el valor del Bluetooth
    return "Hecho";
  }

  return "";
}

void writeBluetooth(int str) {
  BT.write(str);
}

// Mover motor
void moveMotors() {
  if (calibration) {
    // Verificamos valores antes de mover
    Serial.println("Coordenadas: ");
    Serial.print("x = ");
    Serial.println(x);
    Serial.print("y = ");
    Serial.println(y);
    Serial.print("nX = ");
    Serial.println(nX);
    Serial.print("nY = ");
    Serial.println(nY);

    if (nX < 5 && nX >= 0  && nY < 3 && nY >= 0 ) {
      // Calculamos el tipo de momiviento del motor
      if (nX < x) {
        Serial.println("Moviendo motor hacia Izq");
        motorX(true, (fragmentoX * x) - (fragmentoX * nX));
      } else {
        Serial.println("Moviendo motor hacia Der");
        motorX(false, (fragmentoX * nX) - (fragmentoX * x));
      }

      //Calculamos el tipo de movimiento del motor
      if (nY < y) {
        Serial.println("Moviendo motor hacia Arriba");
        motorY(false, (fragmentoY * y) - (fragmentoY * nY));
      } else {
        Serial.println("Moviendo motor hacia Abajo");
        motorY(true, (fragmentoY * nY) - (fragmentoY * y));
      }
    }
  }
}

//True Izquierda
//False Derecha
void motorX(bool direction, int time) {
  Serial.print("Moviendo moto H ");
  Serial.print(time);
  Serial.println("mS");

  int start = millis();
  int now = millis();
  if (direction) {
    Serial.println("Moviendo moto a la izquierda...");
    Serial.println(now - start);
    while (now - start < time) {
      digitalWrite(motor1I, HIGH);
      now = millis();
    }
    digitalWrite(motor1I, LOW);
  } else {
    Serial.println("Moviendo...");
    while (now - start < time) {
      digitalWrite(motor1D, HIGH);
      now = millis();
    }
    digitalWrite(motor1D, LOW);
  }
  x = nX;
}

//True Abajo
//False Arriba
void motorY(bool direction, int time) {
  Serial.print("Moviendo moto V ");
  Serial.print(time);
  Serial.println("mS");

  int start = millis();
  int now = millis();
  if (direction) {
    Serial.println("Moviendo moto a la abajo...");
    while (now - start < time) {
      digitalWrite(motor2D, HIGH);
      now = millis();
    }
    digitalWrite(motor2D, LOW);
  } else {
    Serial.println("Moviendo moto a la arriba...");
    while (now - start < time) {
      digitalWrite(motor2U, HIGH);
      now = millis();
    }
    digitalWrite(motor2U, LOW);
  }
  y = nY;
}

void calibrationUser() {
  Serial.println("Calibración solicitada por el usuario");
  Serial.println("Iniciando Calibración");

  calibration = calibrationMotor();

  Serial.println("Todo listo");
}

void panicBoton() {
  int valor = 0;
  for (int i = 0; i < 30; i++) {
    valor += analogRead(panicB);
  }

  if (valor >= maximoPanic ) {
    panic = true;
    Serial.println("PANICOOOOO");
  }
}

void loop() {
  panicBoton();

  String str = readBluetooth();

  if (panic) {
    return;
  }

  if (str == "Hecho") {
    Serial.println("Moviendo motores");
    moveMotors();
  }
}
