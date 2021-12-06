
#define FOLD_MTR_1      16
#define FOLD_MTR_2      17

#define ANT_MTR_1       14
#define ANT_MTR_2       15
void setup() {
  // put your setup code here, to run once:

  pinMode(FOLD_MTR_1, OUTPUT);
  pinMode(FOLD_MTR_2, OUTPUT);

  pinMode(ANT_MTR_1, OUTPUT);
  pinMode(ANT_MTR_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(ANT_MTR_1, HIGH);
  digitalWrite(ANT_MTR_2, LOW);
  delay(5000);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, HIGH);
  delay(5000);
  digitalWrite(ANT_MTR_1, LOW);
  digitalWrite(ANT_MTR_2, LOW);
  delay(1000);
  
  digitalWrite(FOLD_MTR_1, HIGH);
  digitalWrite(FOLD_MTR_2, LOW);
  delay(5000);
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, HIGH);
  delay(5000);
  digitalWrite(FOLD_MTR_1, LOW);
  digitalWrite(FOLD_MTR_2, LOW);
  delay(1000);
}
