int final1 = A0;
int final2 = A1;
int final3 = A2;
int final4 = A3;

void setup() {
  Serial.begin(9600);
  pinMode(final1, INPUT);
  pinMode(final2, INPUT);
  pinMode(final3, INPUT);
  pinMode(final4, INPUT);

  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void loop() {

  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);

  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);

  
  Serial.println("___________________-");

  Serial.print("Izq ");
  Serial.println(analogRead(final1));

  Serial.print("Der ");
  Serial.println(analogRead(final2));
  
  Serial.print("Up ");
  Serial.println(analogRead(final3));

  Serial.print("Down ");
  Serial.println(analogRead(final4));
  delay(500);
}
