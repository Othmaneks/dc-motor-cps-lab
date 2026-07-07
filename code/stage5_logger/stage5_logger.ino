// STAGE 5 - reponse au creneau + acquisition (baseline S0)
// PWM alterne 150/0 toutes les 2s, 5 cycles, sens avant uniquement.
// Sortie CSV "t_ms,pwm,count" a 100 Hz, 115200 bauds.
// Moniteur serie : regler 115200 sinon caracteres illisibles.

const int PWM = 9, INA = 7, INB = 8;
const int encA = 2, encB = 3;
volatile long encoderCount = 0;

const int PWM_HIGH = 150;
const unsigned long HALF_PERIOD_MS = 2000;  // 2s ON, 2s OFF
const int N_CYCLES = 5;
const unsigned long SAMPLE_MS = 10;         // 100 Hz

void countPulse() {
  if (digitalRead(encA) != digitalRead(encB)) encoderCount++;
  else encoderCount--;
}

void setup() {
  Serial.begin(115200);
  pinMode(PWM, OUTPUT); pinMode(INA, OUTPUT); pinMode(INB, OUTPUT);
  pinMode(encA, INPUT_PULLUP); pinMode(encB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encA), countPulse, CHANGE);
  digitalWrite(INA, HIGH);   // avant seulement pour ce premier jeu de donnees
  digitalWrite(INB, LOW);

  Serial.println("t_ms,pwm,count");

  unsigned long t0 = millis();
  unsigned long totalMs = 2UL * HALF_PERIOD_MS * N_CYCLES;
  unsigned long nextSample = 0;

  while (millis() - t0 < totalMs) {
    unsigned long t = millis() - t0;

    // creneau : ON pendant la premiere moitie de chaque periode
    int pwmVal = ((t / HALF_PERIOD_MS) % 2 == 0) ? PWM_HIGH : 0;
    analogWrite(PWM, pwmVal);

    if (t >= nextSample) {
      noInterrupts();              // lecture atomique du compteur (long = 4 octets)
      long c = encoderCount;
      interrupts();
      Serial.print(t);   Serial.print(',');
      Serial.print(pwmVal); Serial.print(',');
      Serial.println(c);
      nextSample += SAMPLE_MS;
    }
  }
  analogWrite(PWM, 0);
  Serial.println("# fin - copier tout ce qui precede dans un fichier .csv");
}

void loop() {}
