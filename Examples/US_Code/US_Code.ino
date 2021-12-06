
const int pingPin = 10; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 11; // Echo Pin of Ultrasonic Sensor

void setup() {
  Serial.begin(115200); // Starting Serial Terminal
  pinMode(pingPin, OUTPUT);
}

void loop(){
  US_Get_Distance();
}

long US_Get_Distance() {
  long duration, inches, cm = 0;
  for (int i = 0 ; i < 10 ; i++) {
    digitalWrite(pingPin, LOW);
    delayMicroseconds(5);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(20);
    digitalWrite(pingPin, LOW);
    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    //inches = microsecondsToInches(duration);
    cm = cm + microsecondsToCentimeters(duration);
  }
  cm /= 10;
  //Serial.print(inches);
  //Serial.print("in, ");
  Serial.print(cm);
  Serial.println("cm");
  return cm;
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
