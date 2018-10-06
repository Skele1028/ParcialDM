#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(2, 3);   // Definimos los pines RX y TX del Arduino conectados al Bluetooth

void setup() {
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  Serial.begin(9600);   // Inicializamos  el puerto serie
}
  
void loop() {
  readBluetooth();
  writeBluetooth(1);
}

String readBluetooth() {
  // Si llega un dato por el puerto BT se envía al monitor serial
  Serial.println(BT.available());
  if (BT.available()) {
    Serial.println("Lectura realizada");
    String coordenada = BT.readString();

    // Leemos longitud
    Serial.print("Entrada - ");
    Serial.println(coordenada);

    if (coordenada == "Calibrar") {
      //Calibración del sistema
      //calibrationUser();
      Serial.println("Calibración del sistema");
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
    //nX = atol(cadCoor[0]);
    //nY = atol(cadCoor[1]);
    Serial.println(atol(cadCoor[0]));
    Serial.println(atol(cadCoor[1]));

    //Verificamos coor
    Serial.println("Coordeada num - ");
    Serial.print(coordenada);

    //Mostrar Arreglo
    for (int i = 0; i < longitud; i++) {
      Serial.println(cadCoor[i]);
    }

    //Imprimimos posiciones finales
    //Serial.println("El valor de: " + nX);
    //Serial.println("El valor de: " + nY);

    //Retornamos el valor del Bluetooth
    return "Hecho";
  }
}

void writeBluetooth(int str) {
  BT.write(str);
}
