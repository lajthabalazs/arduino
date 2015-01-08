int led = 13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  int signal[] = {1,1,1,3,3,3,1,1,1};
  int pause[] = {1,1,3,1,1,3,1,1,7};
  for (int i = 0; i < 9; i++) {
    playSignal(signal[i], pause[i]);
  }
}

void playSignal(int signal, int pause) {
  digitalWrite(led, HIGH);
  delay(100 * signal);
  digitalWrite(led, LOW);
  delay(100 * pause);
}
