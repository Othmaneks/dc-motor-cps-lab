// STAGE 4 - test direction + vitesse
// Cycle : avant lent/moyen/rapide, stop, arriere lent/moyen/rapide, stop.
// Affiche le compteur encodeur apres chaque etape.
//
// DRIVER  : pin 7 -> INA, pin 8 -> INB, pin 9 -> PWM, 5V -> VDD, GND -> GND
// ENCODEUR: jaune -> 2, blanc -> 3, bleu -> 5V, vert -> GND
// PUISSANCE: 12V -> VIN/GND, moteur rouge/noir -> OUTA/OUTB

const int PWM = 9;
const int INA = 7;
const int INB = 8;

const int encA = 2;
const int encB = 3;
volatile long encoderCount = 0;

void forward(int speed) {        // speed : 0-255
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  analogWrite(PWM, speed);
}

void reverse(int speed) {
  digitalWrite(INA, LOW);
  digitalWrite(INB, HIGH);
  analogWrite(PWM, speed);
}

void stopMotor() {
  analogWrite(PWM, 0);
}

void printCount() {
  Serial.print("   count = ");
  Serial.println(encoderCount);
}

void countPulse() {
  if (digitalRead(encA) != digitalRead(encB)) encoderCount++;
  else encoderCount--;
}

void setup() {
  Serial.begin(9600);
  pinMode(PWM, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encA), countPulse, CHANGE);
  Serial.println("=== Stage 4 : direction + vitesse ===");
}

void loop() {
  Serial.println(">> avant lent");
  forward(80);   delay(2000);   printCount();

  Serial.println(">> avant moyen");
  forward(150);  delay(2000);   printCount();

  Serial.println(">> avant rapide");
  forward(230);  delay(2000);   printCount();

  Serial.println(">> stop");
  stopMotor();   delay(1500);   printCount();

  Serial.println(">> arriere lent");
  reverse(80);   delay(2000);   printCount();

  Serial.println(">> arriere moyen");
  reverse(150);  delay(2000);   printCount();

  Serial.println(">> arriere rapide");
  reverse(230);  delay(2000);   printCount();

  Serial.println(">> stop");
  stopMotor();   delay(1500);   printCount();

  Serial.println("--- cycle termine, on recommence ---");
  Serial.println();
}
