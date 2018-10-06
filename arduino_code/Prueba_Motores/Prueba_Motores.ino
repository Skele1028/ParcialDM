void setup() {
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
}

void loop() {
  digitalWrite(11, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
  digitalWrite(8, LOW);
  delay(3000);

  digitalWrite(11, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(9, LOW);
  digitalWrite(8, HIGH);
  delay(3000);
}
