int inByte = '0';

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    inByte = (Serial.read());
    Serial.print("inByte "); 
    Serial.println(inByte);
    if (inByte == '0') {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    if (inByte == '9') {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}