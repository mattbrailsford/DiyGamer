#include <Gamer.h>
#include <SoftwareSerial.h>

Gamer gamer;

int score = 0;

// Frog array
int frog[3] = {0, 3, 7};

// Frog array helpers
int FROG_DIR = 0;
int FROG_X = 1;
int FROG_Y = 2;

// Car matrix
int cars[6][5] = {
  {1,1,3,random(0,8)},   // Lane 6
  {2,1,3,random(0,8)},   // Lane 5
  {1,2,2,random(0,8)},   // Lane 4
  {2,2,3,random(0,8)},   // Lane 3
  {1,3,2,random(0,8)},   // Lane 2
  {2,3,2,random(0,8)}    // Lane 1d
};

float carCounters[6] = {0};

// Car matrix helpers
int CAR_DIR = 0;
int CAR_SPEED = 1;
int CAR_LENGTH = 2;
int CAR_X = 3;

byte numbers[10][2][8];
byte n1[8],n1b[8],n2[8],n2b[8],n3[8],n3b[8],n4[8],n4b[8],n5[8],n5b[8],n6[8],n6b[8],n7[8],n7b[8],n8[8],n8b[8],n9[8],n9b[8],n0[8],n0b[8];

void setup()
{
  Serial.begin(9600);
  gamer.begin();
  setupNumbers();
}

void loop()
{
  // Clear the display
  for(int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      gamer.display[x][y] = LOW;
    }
  }
  
  // Update actors
  updateCars();
  updateFrog();
  
  // Wait
  delay(150);
  
  // Update display
  gamer.updateDisplay();
}

void updateFrog()
{
  // Get button presses
  if(gamer.isPressed(UP))          frog[FROG_DIR] = 1;
  else if(gamer.isPressed(RIGHT))  frog[FROG_DIR] = 2;
  else if(gamer.isPressed(DOWN))   frog[FROG_DIR] = 3;
  else if(gamer.isPressed(LEFT))   frog[FROG_DIR] = 4;
  else frog[FROG_DIR] = 0;
  
  // If the frog reaches the top, increase score and reset
  if(frog[FROG_Y] <= 0)
  {
    score += 1;
    resetFrog();
  }
  
  // Handle button presses
  if(frog[FROG_DIR] == 1) { // Up
    frog[FROG_Y]--;
    if(frog[FROG_Y] < 0) frog[FROG_Y] = 0;
  } else if(frog[FROG_DIR] == 2) {  // Right
    frog[FROG_X]++;
    if(frog[FROG_X] > 7) frog[FROG_X] = 7;
  } else if(frog[FROG_DIR] == 3) {  // Down
    frog[FROG_Y]++;
    if(frog[FROG_Y] > 7) frog[FROG_Y] = 7;
  } else if(frog[FROG_DIR] == 4) {  // Left
    frog[FROG_X]--;
    if(frog[FROG_X] < 0) frog[FROG_X] = 0;
  }
  
  // Draw frog
  if(gamer.display[frog[FROG_X]][frog[FROG_Y]] == HIGH){
    
    // Let it render for a second
    delay(150);
    
    // Colision
    gamer.clear();
    delay(20);
    
    // Show score
    int dig2 = score % 10;  //split score into two digits (eg 10 -> 1 and 0)
    int dig1 = (score-(score%10))/10;
    showScore(dig1,dig2);
    delay(600);
          
    // Reset game
    reset();
  } else {
    // Ok
    gamer.display[frog[FROG_X]][frog[FROG_Y]] = HIGH;
  }
}

void updateCars()
{
  for(int c = 0; c < 6; c++) {
    
    // Incerement the car counter
    carCounters[c] += 1.00/cars[c][CAR_SPEED];
    
    Serial.println(carCounters[c]);
    
    // Check to see if the car should move
    if(floor(carCounters[c]) >= 1)
    {
      // Move the car
      if(cars[c][CAR_DIR] == 1) cars[c][CAR_X]--;
      if(cars[c][CAR_DIR] == 2) cars[c][CAR_X]++;
    
      // Reset the car counter
      carCounters[c] = 0; 
    }
    
    // Just wrap the car for now
    if(cars[c][CAR_X] < cars[c][CAR_LENGTH] * -1) cars[c][CAR_X] = 7;
    if(cars[c][CAR_X] > 7) cars[c][CAR_X] = cars[c][CAR_LENGTH] * -1;
    
    for(int l = cars[c][CAR_X]; l < cars[c][CAR_X] + cars[c][CAR_LENGTH]; l++)
    {
      if(l >= 0 && l <= 7){
        gamer.display[l][c + 1] = HIGH;
      }
    }
  }
}

void showScore(int dig1,int dig2) {
  byte result[8];
  for(int p=0;p<8;p++) {
    result[p]=numbers[dig1][0][p]^numbers[dig2][1][p];
  }
  gamer.printImage(result);
}

void reset()
{
  score = 0;
  resetCars();
  resetFrog();
}

void resetFrog()
{
  frog[FROG_X] = 3;
  frog[FROG_Y] = 7;
}

void resetCars()
{
  for(int c = 0; c < 6; c++) {
     cars[c][CAR_X] = random(0,8);
     carCounters[c] = 0.00;
  }
}

void setupNumbers() {
  n1[0] = B10000000;
  n1[1] = B10000000;
  n1[2] = B10000000;
  n1[3] = B10000000;
  n1[4] = B10000000;
  n1[5] = B10000000;
  n1[6] = B10000000;
  n1[7] = B10000000;

  n1b[0] = B00000100;
  n1b[1] = B00000100;
  n1b[2] = B00000100;
  n1b[3] = B00000100;
  n1b[4] = B00000100;
  n1b[5] = B00000100;
  n1b[6] = B00000100;
  n1b[7] = B00000100;

  n2[0] = B11100000;
  n2[1] = B00100000;
  n2[2] = B00100000;
  n2[3] = B11100000;
  n2[4] = B10000000;
  n2[5] = B10000000;
  n2[6] = B10000000;
  n2[7] = B11100000;

  n2b[0] = B00000111;
  n2b[1] = B00000001;
  n2b[2] = B00000001;
  n2b[3] = B00000111;
  n2b[4] = B00000100;
  n2b[5] = B00000100;
  n2b[6] = B00000100;
  n2b[7] = B00000111;

  n3[0] = B11100000;
  n3[1] = B00100000;
  n3[2] = B00100000;
  n3[3] = B01100000;
  n3[4] = B00100000;
  n3[5] = B00100000;
  n3[6] = B00100000;
  n3[7] = B11100000;

  n3b[0] = B00000111;
  n3b[1] = B00000001;
  n3b[2] = B00000001;
  n3b[3] = B00000011;
  n3b[4] = B00000001;
  n3b[5] = B00000001;
  n3b[6] = B00000001;
  n3b[7] = B00000111;

  n4[0] = B10100000;
  n4[1] = B10100000;
  n4[2] = B10100000;
  n4[3] = B11100000;
  n4[4] = B00100000;
  n4[5] = B00100000;
  n4[6] = B00100000;
  n4[7] = B00100000;

  n4b[0] = B00000101;
  n4b[1] = B00000101;
  n4b[2] = B00000101;
  n4b[3] = B00000111;
  n4b[4] = B00000001;
  n4b[5] = B00000001;
  n4b[6] = B00000001;
  n4b[7] = B00000001;

  n5[0] = B11100000;
  n5[1] = B10000000;
  n5[2] = B10000000;
  n5[3] = B11100000;
  n5[4] = B00100000;
  n5[5] = B00100000;
  n5[6] = B00100000;
  n5[7] = B11100000;

  n5b[0] = B00000111;
  n5b[1] = B00000100;
  n5b[2] = B00000100;
  n5b[3] = B00000111;
  n5b[4] = B00000001;
  n5b[5] = B00000001;
  n5b[6] = B00000001;
  n5b[7] = B00000111;

  n6[0] = B11100000;
  n6[1] = B10000000;
  n6[2] = B10000000;
  n6[3] = B11100000;
  n6[4] = B10100000;
  n6[5] = B10100000;
  n6[6] = B10100000;
  n6[7] = B11100000;

  n6b[0] = B00000111;
  n6b[1] = B00000100;
  n6b[2] = B00000100;
  n6b[3] = B00000111;
  n6b[4] = B00000101;
  n6b[5] = B00000101;
  n6b[6] = B00000101;
  n6b[7] = B00000111;

  n7[0] = B11100000;
  n7[1] = B00100000;
  n7[2] = B00100000;
  n7[3] = B00100000;
  n7[4] = B00100000;
  n7[5] = B00100000;
  n7[6] = B00100000;
  n7[7] = B00100000;

  n7b[0] = B00000111;
  n7b[1] = B00000001;
  n7b[2] = B00000001;
  n7b[3] = B00000001;
  n7b[4] = B00000001;
  n7b[5] = B00000001;
  n7b[6] = B00000001;
  n7b[7] = B00000001;

  n8[0] = B11100000;
  n8[1] = B10100000;
  n8[2] = B10100000;
  n8[3] = B11100000;
  n8[4] = B10100000;
  n8[5] = B10100000;
  n8[6] = B10100000;
  n8[7] = B11100000;

  n8b[0] = B00000111;
  n8b[1] = B00000101;
  n8b[2] = B00000101;
  n8b[3] = B00000111;
  n8b[4] = B00000101;
  n8b[5] = B00000101;
  n8b[6] = B00000101;
  n8b[7] = B00000111;

  n9[0] = B11100000;
  n9[1] = B10100000;
  n9[2] = B10100000;
  n9[3] = B11100000;
  n9[4] = B00100000;
  n9[5] = B00100000;
  n9[6] = B00100000;
  n9[7] = B11100000;

  n9b[0] = B00000111;
  n9b[1] = B00000101;
  n9b[2] = B00000101;
  n9b[3] = B00000111;
  n9b[4] = B00000001;
  n9b[5] = B00000001;
  n9b[6] = B00000001;
  n9b[7] = B00000111;

  n0[0] = B11100000;
  n0[1] = B10100000;
  n0[2] = B10100000;
  n0[3] = B10100000;
  n0[4] = B10100000;
  n0[5] = B10100000;
  n0[6] = B10100000;
  n0[7] = B11100000;

  n0b[0] = B00000111;
  n0b[1] = B00000101;
  n0b[2] = B00000101;
  n0b[3] = B00000101;
  n0b[4] = B00000101;
  n0b[5] = B00000101;
  n0b[6] = B00000101;
  n0b[7] = B00000111;

  for(int x=0;x<8;x++) {
    numbers[0][0][x] = n0[x];
    numbers[1][0][x] = n1[x];
    numbers[2][0][x] = n2[x];
    numbers[3][0][x] = n3[x];
    numbers[4][0][x] = n4[x];
    numbers[5][0][x] = n5[x];
    numbers[6][0][x] = n6[x];
    numbers[7][0][x] = n7[x];
    numbers[8][0][x] = n8[x];
    numbers[9][0][x] = n9[x];
    numbers[0][1][x] = n0b[x];
    numbers[1][1][x] = n1b[x];
    numbers[2][1][x] = n2b[x];
    numbers[3][1][x] = n3b[x];
    numbers[4][1][x] = n4b[x];
    numbers[5][1][x] = n5b[x];
    numbers[6][1][x] = n6b[x];
    numbers[7][1][x] = n7b[x];
    numbers[8][1][x] = n8b[x];
    numbers[9][1][x] = n9b[x];
  }
}
