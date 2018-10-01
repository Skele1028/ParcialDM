#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(4, 5);   // Definimos los pines RX y TX del Arduino conectados al Bluetooth

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

int fragmentoX;
int fragmentoY;

int tx;
int ty;

int x;
int y;

int nX;
int nY;

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

  fragmentoX = tx / 4;
  fragmentoY = ty / 2;

  Serial.println("Iniciando Calibración");

  calibration = calibrationMotor();

  Serial.println("Todo listo");
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
  while (valor < valorFinal2) {
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
  while (valor < valorFinal4) {
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
  while (valor < valorFinal1) {
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
  while (valor < valorFinal3) {
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
  if (tx > ty) {
    Serial.println("Calibración terminada");
    return true;
  }

  Serial.println("Calibración fallida");
  return false;
}

String readBluetooth() {
  // Si llega un dato por el puerto BT se envía al monitor serial
  if (BT.available()) {
    Serial.println("Lectura realizada");
    String coordenada = BT.readString();

    // Leemos longitud
    Serial.print("Entrada - ");
    Serial.println(coordenada);

    if (coordenada == "calibrar") {
      //Calubración del sistema
      calibrationUser();
      return  "";
    }

    int longitud = coordenada.length() + 1 ;

    // Mostramos longitud
    Serial.print("Longitud - ");
    Serial.println(longitud);

    //Creamos arreglo y pasamos caracteres
    char cadCoor[longitud];
    coordenada.toCharArray(cadCoor, longitud);

    //Asignación de valores
    nX = atol(cadCoor[0]);
    nY = atol(cadCoor[1]);

    //Verificamos coor
    Serial.println("Coordeada num - ");
    Serial.print(coordenada);

    //Mostrar Arreglo
    for (int i = 0; i < longitud; i++) {
      Serial.println(cadCoor[i]);
    }

    //Imprimimos posiciones finales
    Serial.println("El valor de: " + nX);
    Serial.println("El valor de: " + nY);
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
    if (nX < 5 && nY < 3) {
      // Calculamos el tipo de momiviento del motor
      if (nX < x) {
        motorX(false, (fragmentoX * x) - (fragmentoX - nX));
      } else {
        motorX(false, (fragmentoX * nX) - (fragmentoX - x));
      }

      //Calculamos el tipo de movimiento del motor
      if (nY < y) {
        Serial.println();
        motorY(false, (fragmentoY * y) - (fragmentoY - nY));
      } else {
        Serial.println();
        motorY(true, (fragmentoY * nY) - (fragmentoY - y));
      }
    }
  }
}

//True derecha - Abajo
//False izquierda - Arriba
void motorX(bool direction, int time) {
  int start = millis();
  int now = 0;
  if (direction) {
    while (now - start < time) {
      digitalWrite(motor1I, HIGH);
      now = millis();
    }
    digitalWrite(motor1I, LOW);
  } else {
    while (now - start < time) {
      digitalWrite(motor1D, HIGH);
      now = millis();
    }
    digitalWrite(motor1D, LOW);
  }
  x = nX;
}

void motorY(bool direction, int time) {
  int start = millis();
  int now = 0;
  if (direction) {
    while (now - start < time) {
      digitalWrite(motor2D, HIGH);
      now = millis();
    }
    digitalWrite(motor2D, LOW);
  } else {
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
  calibration = calibrationMotor();
}

void loop() {
  readBluetooth();  
}
