#include <SoftwareSerial.h>   // Incluimos la librería  SoftwareSerial  
SoftwareSerial BT(12, 13);   // Definimos los pines TX y RX del Arduino conectados al Bluetooth

void setup() {
  BT.begin(9600);       // Inicializamos el puerto serie BT que hemos creado
  Serial.begin(9600);   // Inicializamos  el puerto serie
}

void loop() {
  readBluetooth();
}

void readBluetooth() {
  if (BT.available()) {
    String str = "";
    while (BT.available())  // Si llega un dato por el puerto BT se envía al monitor serial
    {
      str += BT.read();
    }
    Serial.println("Lectura realizada");
    Serial.println(str);

    if (str == "6565") {
      //Calibración del sistema
      //calibrationUser();
      Serial.println("Calibración del sistema");
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
    int nX = ((cadCoor[0] - '0') * 10) + (cadCoor[1] - '0') - 48;
    int nY = ((cadCoor[2] - '0') * 10) + (cadCoor[3] - '0') - 48;

    //Verificamos coor
    Serial.print("Coordeada: ");
    Serial.print(nX);
    Serial.print(",");
    Serial.println(nY);

    //Retornamos el valor del Bluetooth
    return "Hecho";    
  }
}

void writeBluetooth(int str) {
  BT.write(str);
}
