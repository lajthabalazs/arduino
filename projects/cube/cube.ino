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

int data[9][9];
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
  showTestPattern();
  ICSC.begin(3, 115200);
  ICSC.registerCommand('R', &receivePacket);
}

void loop() {
  ICSC.process();
  for (int layer = 0; layer < 9; layer++) {
  clear();
    for (int base = 0; base < 9; base++) {
      if (data[base][layer] != 0) {
        digitalWrite(bases[base], LOW);
        digitalWrite(layers[layer],HIGH);
      }
    }
    delay(1);
  }
}
  
void clear () {
  for (int base = 0; base < 9; base++) {
    digitalWrite(bases[base], HIGH);
  }
  for (int layer = 0; layer < 9; layer++) {
    digitalWrite(layers[layer], LOW);
  }
}

void receivePacket(unsigned char source, char command, unsigned char length, char *data) {
  // TODO do stuff
}

void showTestPattern() {
  data[0][0] = 1;
  data[1][0] = 1;
  data[2][0] = 1;

  data[3][1] = 1;
  data[4][1] = 1;
  data[5][1] = 1;

  data[6][2] = 1;
  data[7][2] = 1;
  data[8][2] = 1;

  data[0][3] = 1;
  data[3][3] = 1;
  data[6][3] = 1;

  data[1][4] = 1;
  data[4][4] = 1;
  data[7][4] = 1;

  data[2][5] = 1;
  data[5][5] = 1;
  data[8][5] = 1;

  data[0][6] = 1;
  data[1][6] = 1;
  data[2][6] = 1;

  data[3][7] = 1;
  data[4][7] = 1;
  data[5][7] = 1;

  data[6][8] = 1;
  data[7][8] = 1;
  data[8][8] = 1;  
}
