// LED stuff

int MAX_LEVEL = 8;
int levels[] = {0,2,4,8,16,32,64,128,255};

int RED_PIN = 10;
int GREEN_PIN = 5;
int BLUE_PIN = 6;

int BACK_BUTTON = A5;
int FORWARD_BUTTON = A4;

long backButtonStart = 0;
long forwardButtonStart = 0;

long lastBackButtonStep = 0;
long lastForwardButtonStep = 0;

boolean backButtonDown = false;
boolean forwardButtonDown = false;

int levelIndex = 0;

long lastTime = 0;

void setup() {
  Serial.begin(115200);

  // Setting up LEDs
  
  pinMode(BACK_BUTTON, INPUT);
  digitalWrite(BACK_BUTTON, HIGH); // connect internal pull-up

  pinMode(FORWARD_BUTTON, INPUT);
  digitalWrite(FORWARD_BUTTON, HIGH); // connect internal pull-up
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
    lastBackButtonStep = t;
    lastForwardButtonStep = t;
    backButtonStart = t;
    forwardButtonStart = t;
    delay(10);
    return;
  }
  if (!backButtonDown) {
    backButtonStart = t;
    lastBackButtonStep = t;
  }
  if (!forwardButtonDown) {
    forwardButtonStart = t;
    lastForwardButtonStep = t;
  }
  boolean newBackButtonDown = (digitalRead(BACK_BUTTON) == LOW);
  boolean newForwardButtonDown = (digitalRead(FORWARD_BUTTON) == LOW);
  // Button released, check length
  if (!newBackButtonDown && backButtonDown) {
    Serial.print("Back button released, time ");
    Serial.println((t - backButtonStart));
    if ((t - backButtonStart) > 30 && (t-backButtonStart) < 300) {
      if (levelIndex == 0) {
        switchOn();
      } else {
        previousEffect();
      }
    }
  } else if (backButtonDown) { // Holding button
    if (t - lastBackButtonStep > 500) {
      lastBackButtonStep = t;
      dim();
    }
  }
  if (!newForwardButtonDown && forwardButtonDown) {
    Serial.print("Forward button released, time ");
    Serial.println((t - forwardButtonStart));
    if ((t - forwardButtonStart) > 30 && (t-forwardButtonStart) < 300) {
      if (levelIndex == 0) {
        switchOn();
      } else {
        nextEffect();
      }
    }
  } else if (forwardButtonDown) { // Holding button
    if (t - lastForwardButtonStep > 500) {
      lastForwardButtonStep = t;
      brighten();
    }
  }
  forwardButtonDown = newForwardButtonDown;
  backButtonDown = newBackButtonDown;
  if (lastTime != t) { // Change color only if at least a milli has passed
    setColor(t);
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

long red = 0;
long green = 0;
long blue = 0;

int index = 0;
int maxIndex = 7;

int reds[] =   {255,0,0,  255,255,0,  255};
int greens[] = {0,255,0,  255,0,255,  255};
int blues[] =  {0,0,255,  0,255,255,  255};

int pulseIndex = 0;

void nextEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  index++;
  if (index > maxIndex) {
    index = 0;
  }
  Serial.print("Next effect, effect index ");
  Serial.println(index);
  eventProcessed = true;
}

void previousEffect() {
  if (eventProcessed) {
    Serial.println("Multiple events");
    return;
  }
  index--;
  if (index < 0) {
    index = maxIndex;
  }
  Serial.print("Previous effect, effect index ");
  Serial.println(index);
  eventProcessed = true;
}

int lastIntensity = 0;

void setColor(long time) {
  if (index == maxIndex) {
    // Determine color based on time
    long intensity = (time / 4) % 512;
    if (intensity == lastIntensity) {
      return;
    }
    lastIntensity = intensity;
    if (intensity == 0) {
      pulseIndex++;
      Serial.print("Next pulse color, index ");
      if (pulseIndex == maxIndex) {
        pulseIndex = 0;
      }
      Serial.println(pulseIndex);
    }
    if (intensity > 255) {
      intensity = 511 - intensity;
    }
    red = (reds[pulseIndex] * intensity) / 255;
    green = (greens[pulseIndex] * intensity) / 255;
    blue = (blues[pulseIndex] * intensity) / 255;
  } else {
    red = reds[index];
    green = greens[index];
    blue = blues[index];
  }
  analogWrite(RED_PIN, (int)(red * levels[levelIndex] / 255));
  analogWrite(GREEN_PIN, (int)(green * levels[levelIndex] / 255));
  analogWrite(BLUE_PIN, (int)(blue * levels[levelIndex] / 255));
}
