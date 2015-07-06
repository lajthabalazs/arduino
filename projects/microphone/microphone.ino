int ledPins[] = {2,3,4,5,6,7,8,9,10,11};

void setup() {
  for (int i = 0; i < 10; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  Serial.begin(115200);
}

int value = 0;
int maxValue = 0;
int p1 = 0;
int maxP1 = 0;
int p2 = 0;
int maxP2 = 0;

int cycle = 0;
void loop(){
  
  value = max(abs(analogRead(A0) - 878), 3) - 3;
  p1 = analogRead(A1);
  p2 = analogRead(A2);
  if (maxValue < value) {
    maxValue = value;
    if (maxValue > 3) {
      Serial.print("A ");
      Serial.println(maxValue);
    }
  }
  if (maxP1 < p1) {
    maxP1 = p1;
    if (maxP1 > 20) {
      Serial.print("P1 ");
      Serial.println(maxP1);
    }
  }
  if (maxP2 < p2) {
    maxP2 = p2;
    if (maxP2 > 20) {
      Serial.print("P2 ");
      Serial.println(maxP2);
    }
  }
  cycle++;
  if (cycle == 10) {
    cycle = 0;
    maxValue = maxValue * 92 / 100;
    maxP1 = maxP1 * 95 / 100;
    maxP2 = maxP1 * 95 / 100;    
  }
  for (int i = 0; i < 10; i++) {
    if (maxValue > i) {
      digitalWrite(ledPins[i], HIGH);
    } else {
          digitalWrite(ledPins[i], LOW);
    } 
  }
}

