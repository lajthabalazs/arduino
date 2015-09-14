// For rotary encoder

volatile boolean TurnDetected = false;
volatile boolean up = false;

const int PinCLK = 7;                 // Used for generating interrupts using CLK signal
const int PinDT = 5;                  // Used for reading DT signal
const int PinSW = 6;                  // Used for the push button switch
long lastPressed = 0;
long now = 0;

void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
  Serial.println("Interrupt");
  if (digitalRead(PinCLK))
    up = digitalRead(PinDT);
  else
    up = !digitalRead(PinDT);
  TurnDetected = true;
}

// Main stuff
void setup() { // initialize the buttons' inputs:

  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(PinSW, INPUT);
  
  digitalWrite(PinSW, HIGH);
  attachInterrupt (4, isr, FALLING); // interrupt 0 is always connected to pin 2 on Arduino UNO
  Serial.begin (9600);
  Serial.println("Start");
  // initialize mouse control:
  Mouse.begin();
  now = millis();
}

long virtualPosition = 0;

void loop() {
  // Check rotary click
  if (!(digitalRead(PinSW)) && ((millis() - lastPressed) > 50)) {
    lastPressed = now;
    Serial.println ("Click");
    Mouse.click();
  }

  if (TurnDetected)  {		    // do this only if rotation was detected
    if (up) {
      virtualPosition++;
      Mouse.move(0, 0, -1);
    } else {
      virtualPosition--;
      Mouse.move(0, 0, 1);
    }
    TurnDetected = false;
    Serial.print ("Count = ");
    Serial.println (virtualPosition);
  }
  delay(1);
}

