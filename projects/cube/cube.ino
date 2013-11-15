#include <ICSC.h>

#define B_0_0 2
#define B_0_1 3
#define B_0_2 4
#define B_1_0 5
#define B_1_1 6
#define B_1_2 7
#define B_2_0 8
#define B_2_1 9
#define B_2_2 10

#define L_0_R 11
#define L_0_G 12
#define L_0_B 13
#define L_1_R 14
#define L_1_G 15
#define L_1_B 16
#define L_2_R 17
#define L_2_G 18
#define L_2_B 19

int pixelData[9][9];
int bases[9];
int layers[9];

void setup() {
  pinMode(B_0_0, OUTPUT);
  pinMode(B_0_1, OUTPUT);
  pinMode(B_0_2, OUTPUT);
  pinMode(B_1_0, OUTPUT);
  pinMode(B_1_1, OUTPUT);
  pinMode(B_1_2, OUTPUT);
  pinMode(B_2_0, OUTPUT);
  pinMode(B_2_1, OUTPUT);
  pinMode(B_2_2, OUTPUT);

  pinMode(L_0_R, OUTPUT);
  pinMode(L_0_G, OUTPUT);
  pinMode(L_0_B, OUTPUT);
  pinMode(L_1_R, OUTPUT);
  pinMode(L_1_G, OUTPUT);
  pinMode(L_1_B, OUTPUT);
  pinMode(L_2_R, OUTPUT);
  pinMode(L_2_G, OUTPUT);
  pinMode(L_2_B, OUTPUT);

  layers[0] = L_0_R;
  layers[1] = L_0_G;  
  layers[2] = L_0_B;
  layers[3] = L_1_R;
  layers[4] = L_1_G;  
  layers[5] = L_1_B;
  layers[6] = L_2_R;
  layers[7] = L_2_G;  
  layers[8] = L_2_B;
  
  bases[0] = B_0_0;
  bases[1] = B_0_1;
  bases[2] = B_0_2;  
  bases[3] = B_1_0;
  bases[4] = B_1_1;
  bases[5] = B_1_2;  
  bases[6] = B_2_0;
  bases[7] = B_2_1;
  bases[8] = B_2_2;
  ICSC.begin(3, 115200);
  ICSC.registerCommand('T', &receiveTestPatternCommand);
  ICSC.registerCommand('C', &receiveClearCommand);
  ICSC.registerCommand('D', &receiveDataCommand);
  ICSC.registerCommand('R', &receiveRandomCommand);
  runInitSequence();
}

void loop() {
  ICSC.process();
  refresh();
}

void resetData () {
  for (int base = 0; base < 9; base++) {
    for (int layer = 0; layer < 9; layer++) {
      pixelData[layer][base] = 0;
    }
  }
  refresh();  
}

void mDelay(int forDelay) {
  long start = millis();
  while (start + forDelay > millis()) {
    refresh();
  }
}

void refresh() {
  for (int layer = 0; layer < 9; layer++) {
    clearLeds();
    digitalWrite(layers[layer],HIGH);
    for (int base = 0; base < 9; base++) {
      if (pixelData[layer][base] != 0) {
        digitalWrite(bases[base], LOW);
      }
    }
    for (int i = 0; i < 100; i++) {
    }
  }
}
  
void clearLeds () {
  for (int base = 0; base < 9; base++) {
    digitalWrite(bases[base], HIGH);
  }
  for (int layer = 0; layer < 9; layer++) {
    digitalWrite(layers[layer], LOW);
  }
}

void receiveTestPatternCommand(unsigned char source, char command, unsigned char length, char *data) {
  showTestPattern();
}

void receiveClearCommand(unsigned char source, char command, unsigned char length, char *data) {
  resetData();
}

void receiveDataCommand(unsigned char source, char command, unsigned char length, char *data) {
  if (length > 0) {
    for (int i = 0; i < 27 && i < length; i++) {
      char color = data[i];
      int layer = (i / 9) * 3;
      int base = i - layer * 3;
      pixelData[layer][base] = 0;
      pixelData[layer + 1][base] = 0;
      pixelData[layer + 1][base] = 0;
      if (color == 'R') {
        pixelData[layer][base] = 1;
      } else if (color == 'G') {
        pixelData[layer + 1][base] = 1;
      } else if (color == 'B') {
        pixelData[layer + 2][base] = 1;
      }
    }
  }
}

void runInitSequence() {
  showTestPattern();
  mDelay(1000);
  resetData();
  mDelay(200);
  for (int i = 0; i < 5; i++) {
    showTestPattern();
    mDelay(200);
    resetData();
    mDelay(200);
  }
  for (int i = 0; i < 30; i++) {
    lightUpRandom(5);
    mDelay(50);
  }
  for (int i = 0; i < 30; i++) {
    switchOffRandom(5);
    mDelay(50);
  }
  resetData();
}

void receiveRandomCommand(unsigned char source, char command, unsigned char length, char *data) {
  if (length > 0) {
    int count = data[0];
    flickRandom(abs(count));
  }
}

void showTestPattern() {
  resetData();
  pixelData[0][0] = 1;
  pixelData[0][1] = 1;
  pixelData[0][2] = 1;

  pixelData[1][3] = 1;
  pixelData[1][4] = 1;
  pixelData[1][5] = 1;

  pixelData[2][6] = 1;
  pixelData[2][7] = 1;
  pixelData[2][8] = 1;

  pixelData[3][0] = 1;
  pixelData[3][3] = 1;
  pixelData[3][6] = 1;

  pixelData[4][1] = 1;
  pixelData[4][4] = 1;
  pixelData[4][7] = 1;

  pixelData[5][2] = 1;
  pixelData[5][5] = 1;
  pixelData[5][8] = 1;

  pixelData[6][0] = 1;
  pixelData[6][1] = 1;
  pixelData[6][2] = 1;

  pixelData[7][3] = 1;
  pixelData[7][4] = 1;
  pixelData[7][5] = 1;

  pixelData[8][6] = 1;
  pixelData[8][7] = 1;
  pixelData[8][8] = 1; 
  refresh();  
}

void flickRandom(int number) {
  for (int i =0; i < number; i++) {
    int layer = random(0,9);
    int base = random(0,9);
    if (pixelData[layer][base] == 0) {
      pixelData[layer][base] = 1;
    } else {
      pixelData[layer][base] = 0;
    }
  }
  refresh();
}

void lightUpRandom(int number) {
  for (int i =0; i < number; i++) {
    int layer = random(0,9);
    int base = random(0,9);
    pixelData[layer][base] = 1;
  }
  refresh();
}

void switchOffRandom(int number) {
  for (int i =0; i < number; i++) {
    int layer = random(0,9);
    int base = random(0,9);
    pixelData[layer][base] = 0;
  }
  refresh();
}
