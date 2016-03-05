int MAX_LEVEL = 8;
int levels[] = {0,2,4,8,16,32,64,128,255};

int FIRST_PIN = 5;
int SECOND_PIN = 6;
int THIRD_PIN = 10;

int BUTTON = 3;

long buttonStart = 0;

long lastButtonStep = 0;

boolean buttonDown = false;

int levelIndex = 0;

long lastTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH); // connect internal pull-up

  Serial.println("Startng lamp");
}

boolean eventProcessed = false;

void loop() {
  eventProcessed = false;
  long t = millis();
  if (lastTime == t) {
     delay (1);
     return;
  }
  if ((lastTime == 0) || (lastTime > t)) {
    Serial.print("Reseting timers, last: ");
    Serial.print (lastTime);
    Serial.print (", time: ");
    Serial.println (t);
    lastTime = t;
    lastButtonStep = t;
    buttonStart = t;
    delay(10);
    return;
  }
  if (!buttonDown) {
    buttonStart = t;
    lastButtonStep = t;
  }

  boolean newButtonDown = (digitalRead(BUTTON) == LOW);
  // Button released, check length
  if (!newButtonDown && buttonDown) {
    Serial.print("Button released, time ");
    Serial.println((t - buttonStart));
    if ((t - buttonStart) > 30 && (t-buttonStart) < 300) {
      if (levelIndex == 0) {
        switchOn();
      } else {
        brighten();
      }
    }
  } else if (buttonDown) { // Holding button
    if (t - lastButtonStep > 500) {
      lastButtonStep = t;
      dim();
    }
  }
  buttonDown = newButtonDown;
  if (lastTime != t) { // Change color only if at least a milli has passed
    shine(t);
  }
  lastTime = t;
}

void switchOn() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  Serial.println("Switching on");
  levelIndex = 1;
  eventProcessed = true;
}

void dim() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  levelIndex--;
  if (levelIndex < 0) {
    levelIndex = 0;
  }
  Serial.print("Dimming, new level ");
  Serial.println(levelIndex);
  eventProcessed = true;
}

void brighten() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  levelIndex++;
  if (levelIndex > MAX_LEVEL) {
    levelIndex = MAX_LEVEL;
  }
  Serial.print("Brightening, new level ");
  Serial.println(levelIndex);
  eventProcessed = true;
}

void shine(long time) {
  int firstLevel = levels[levelIndex];
  int secondLevel = levels[levelIndex];
  int thirdLevel = levels[levelIndex];
  analogWrite(FIRST_PIN, firstLevel);
  analogWrite(SECOND_PIN, secondLevel);
  analogWrite(THIRD_PIN, thirdLevel);
}
